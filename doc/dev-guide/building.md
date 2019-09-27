
# Building Inception


First, checkout the git folder

	git clone https://github.com/SpirentOrion/inception-core

Then download the sub-modules dependencies

	make -C inception-core deps

Then, build the docker image to get all tools installed (needed for C++17 compilation):

	docker build -t inception-builder inception-core

Then build the inception code from within the docker container:

	docker run -it --privileged -v ./inception-core:/project inception-builder:latest /bin/bash -c "make -C /project all"

If all is fine, you should see something like

	sudo -n /sbin/setcap CAP_IPC_LOCK,CAP_NET_RAW,CAP_SYS_ADMIN,CAP_DAC_OVERRIDE,CAP_SYS_PTRACE=epi /project/build/inception-linux-x86_64-testing/bin/inception
	make[1]: Leaving directory '/project/targets/inception'
	make[1]: Entering directory '/project/targets/libicp-shim'
	clang++ -flto=thin -shared -o /project/build/libicp-shim-linux-x86_64-testing/lib/libicp-shim.so -fuse-ld=lld -static-libstdc++ -static-libgcc -shared -L/project/build/libicp-shim-linux-x86_64-testing/lib /project/build/libicp-shim-linux-x86_64-testing/obj/icp-shim.o /project/build/libicp-shim-linux-x86_64-testing/obj/libc_wrapper.o -licp_socket_client -lrt -ldl
	make[1]: Leaving directory '/project/targets/libicp-shim'
	root@8f97d154f160:/project#
	root@8f97d154f160:/project#

Finally, run the tests

	docker run -it --privileged -v ./inception-core:/project inception-builder:latest /bin/bash -c "make -C /project test"

For verbose test, you can run the following command within the docker container: 

	cd /project/tests/aat;
	env/bin/mamba spec --format=documentation

You will be able to see the following logs:

	Modules,
	  list,
	    all,
	      ✓ it returns list of modules (1.3598 seconds)
	      unsupported method,
	        ✓ it returns 405
	  get,
	    known module,
	      packetio
	        ✓ it succeeds
	    non-existent module,
	      ✓ it returns 404
	    unsupported method,
	      ✓ it returns 405
	    invalid module name,
	      ✓ it returns 400	


