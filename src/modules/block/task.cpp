#include <thread>
#include "task.hpp"
#include "core/op_core.h"
#include "timesync/chrono.hpp"

namespace openperf::block::worker
{

typedef timesync::chrono::realtime realtime;

struct operation_config {
    int fd;
    size_t f_size;
    size_t block_size;
    uint8_t* buffer;
    off_t offset;
    int (*queue_aio_op)(aiocb *aiocb);
};

static off_t get_first_block(size_t, size_t)
{
    return 0;
}

static off_t get_last_block(size_t file_size, size_t io_size)
{
    return static_cast<off_t>(file_size / io_size);
}

static int submit_aio_op(const operation_config& op_config, operation_state *op_state) {
    struct aiocb *aio = &op_state->aiocb;
    *aio = ((aiocb) {
            .aio_fildes = op_config.fd,
            .aio_offset = static_cast<off_t>(op_config.block_size) * op_config.offset,
            .aio_buf = op_config.buffer,
            .aio_nbytes = op_config.block_size,
            .aio_reqprio = 0,
            .aio_sigevent.sigev_notify = SIGEV_NONE,
    });

    /* Reset stat related variables */
    op_state->state = PENDING;
    op_state->start_ns = 0;//icp_timestamp_now();
    op_state->stop_ns = 0;
    op_state->io_bytes = 0;

    /* Submit operation to the kernel */
    if (op_config.queue_aio_op(aio) == -1) {
        if (errno == EAGAIN) {
            OP_LOG(OP_LOG_WARNING, "AIO queue is full!\n");
            op_state->state = IDLE;
        } else {
            OP_LOG(OP_LOG_ERROR, "Could not queue AIO operation: %s\n",
                    strerror(errno));
        }
        return -errno;
    }
    return 0;
}

static int wait_for_aio_ops(operation_state *aio_ops,
                             size_t nb_ops) {

    const aiocb *aiocblist[nb_ops];
    size_t nb_cbs = 0;
    /*
     * Loop through all of our aio cb's and build a list with all pending
     * operations.
     */
    for (size_t i = 0; i < nb_ops; ++i) {
        if (aio_ops[i].state == PENDING) {
            /* add it to our list */
            aiocblist[nb_cbs++] = &aio_ops[i].aiocb;
        }
    }

    int error = 0;
    if (nb_cbs) {
        /*
         * Wait for one of our outstanding requests to finish.  The one
         * second timeout is a failsafe to prevent hanging here indefinitely.
         * It should (hopefully?) be high enough to prevent interrupting valid
         * block requests...
         */
        struct timespec timeout = { .tv_sec = 1, .tv_nsec = 0 };
        if (aio_suspend(aiocblist, nb_cbs, &timeout) == -1) {
            error = -errno;
            OP_LOG(OP_LOG_ERROR, "aio_suspend failed: %s\n", strerror(errno));
        }
    }

    return (error);
}

static int complete_aio_op(struct operation_state *aio_op) {
int err = 0;
    struct aiocb *aiocb = &aio_op->aiocb;

    if (aio_op->state != PENDING)
        return (-1);

    switch((err = aio_error(aiocb))) {
    case 0: {
        /* AIO operation completed */
        ssize_t nb_bytes = aio_return(aiocb);
        aio_op->stop_ns = 0;//icp_timestamp_now();
        aio_op->io_bytes = nb_bytes;
        aio_op->state = COMPLETE;
        break;
    }
    case EINPROGRESS:
        /* Still waiting */
        break;
    case ECANCELED:
    default:
        /* could be canceled or error; we don't make a distinction */
        aio_return(aiocb);  /* free resources */
        aio_op->start_ns = 0;
        aio_op->io_bytes = 0;
        aio_op->state = FAILED;
        err = 0;
    }

    return (err);
}

block_task::block_task()
    : aio_ops(0)
    , buf(0)
    , read_timestamp(0)
    , write_timestamp(0)
    , pause_timestamp(0)
{

}

block_task::~block_task()
{
    if (buf)
        delete buf;
    if (aio_ops)
        delete aio_ops;
}


size_t block_task::worker_spin(int (*queue_aio_op)(aiocb *aiocb))
{
    int32_t total_ops = 0;
    int32_t pending_ops = 0;
    auto op_conf = (operation_config){
        task_config.fd,
        task_config.f_size,
        task_config.read_size,
        buf,
        pattern.generate(),
        queue_aio_op
    };
    for (int32_t i = 0; i < task_config.queue_depth; ++i) {
        auto aio_op = &aio_ops[i];
        if (submit_aio_op(op_conf, aio_op) == 0) {
            pending_ops++;
        } else if (aio_op->state == FAILED) {
            total_ops++;
            //atomic_inc(&stats->operations, 1);
            //atomic_inc(&stats->errors, 1);
        } else {
            /* temporary queueing error */
            break;
        }
    }
    while (pending_ops) {
        if (wait_for_aio_ops(aio_ops, task_config.queue_depth) != 0) {
            /*
             * Eek!  Waiting failed, so cancel pending operations.
             * The aio_cancel function only has two documented failure modes:
             * 1) bad file descriptor
             * 2) aio_cancel not supported
             * We consider either of these conditions to be fatal.
             */
            if (aio_cancel(task_config.fd, NULL) == -1) {
                //icp_exit("Could not cancel pending AIO operatons: %s\n", strerror(errno));
            }
        }
        /*
         * Find the completed op and fire off another one to
         * take it's place.
         */
        for (int32_t i = 0; i < task_config.queue_depth; ++i) {
            auto aio_op = &aio_ops[i];
            if (complete_aio_op(aio_op) == 0) {
                /* found it; update stats */
                total_ops++;
                switch (aio_op->state)
                {
                case COMPLETE:
                {
                    //atomic_inc(&stats->operations, 1);
                    //atomic_inc(&stats->bytes, aio_op->io_bytes);

                    /*uint64_t op_ns = aio_op->stop_ns - aio_op->start_ns;
                    atomic_inc(&stats->ns_total, op_ns);
                    if (op_ns < atomic_get(&stats->ns_min)) {
                        atomic_set(&stats->ns_min, op_ns);
                    }
                    if (op_ns > atomic_get(&stats->ns_max)) {
                        atomic_set(&stats->ns_max, op_ns);
                    }*/
                    break;
                }
                case FAILED:
                    //atomic_inc(&stats->operations, 1);
                    //atomic_inc(&stats->errors, 1);
                    break;
                default:
                    OP_LOG(OP_LOG_WARNING, "Completed AIO operation in %d state?\n",
                            aio_op->state);
                    break;
                }

                /* if we haven't hit our total or deadline, then fire off another op */
                if ((total_ops + pending_ops) >= task_config.queue_depth
                    //|| icp_timestamp_now() >= deadline
                    || submit_aio_op(op_conf, aio_op) != 0) {
                    // if any condition is true, we have one less pending op
                    pending_ops--;
                    if (aio_op->state == FAILED) {
                        total_ops++;
                        //atomic_inc(&stats->operations, 1);
                        //atomic_inc(&stats->errors, 1);
                    }
                }
            }
        }
    }
    return (total_ops);
}

void block_task::config(const task_config_t& p_config)
{
    task_config = p_config;

    buf = (uint8_t*)realloc(buf, task_config.queue_depth * std::max(task_config.read_size, task_config.write_size));
    aio_ops = (operation_state*)realloc(aio_ops, task_config.queue_depth * sizeof(operation_state));
    pattern.reset(get_first_block(task_config.f_size, task_config.read_size), get_last_block(task_config.f_size, task_config.read_size), task_config.pattern);
}

task_config_t block_task::config() const
{
    return task_config;
}

void block_task::resume()
{
    read_timestamp += realtime::now().time_since_epoch().count() - pause_timestamp;
    write_timestamp += realtime::now().time_since_epoch().count() - pause_timestamp;
}

void block_task::pause()
{
    pause_timestamp = realtime::now().time_since_epoch().count();
}

void block_task::spin()
{
    if (read_timestamp < write_timestamp) {
        worker_spin(aio_read);
        read_timestamp += std::nano::den / task_config.reads_per_sec;
    } else {
        worker_spin(aio_write);
        write_timestamp += std::nano::den / task_config.writes_per_sec;
    }
    auto next_ts = std::min(read_timestamp, write_timestamp);
    auto now = realtime::now().time_since_epoch().count();
    if (next_ts > now)
        std::this_thread::sleep_for(std::chrono::nanoseconds(next_ts - now));
}

} // namespace openperf::block::worker