#
# Makefile component for DPDK builds
#

DPDK_REQ_VARS := \
	OP_ROOT \
	OP_BUILD_ROOT \
	ARCH \
	PLATFORM
$(call op_check_vars,$(DPDK_REQ_VARS))

DPDK_SRC_DIR := $(OP_ROOT)/deps/dpdk
DPDK_BLD_DIR := $(OP_BUILD_ROOT)/dpdk
DPDK_INC_DIR := $(DPDK_BLD_DIR)/include
DPDK_LIB_DIR := $(DPDK_BLD_DIR)/lib

ifeq ($(MODE),debug)
	DPDK_DEFTARGET :=	openperf-debug-$(ARCH)-default-$(PLATFORM)-clang
else
	DPDK_DEFTARGET :=	openperf-$(ARCH)-default-$(PLATFORM)-clang
endif

DPDK_DEFCONFIG := $(OP_ROOT)/conf/dpdk/defconfig_$(DPDK_DEFTARGET)

# DPDK target depends on the configuration we're building
DPDK_SHARED = $(shell grep "BUILD_SHARED_LIB=y" $(DPDK_DEFCONFIG))
ifneq (,$(DPDK_SHARED))
	DPDK_TARGET := $(DPDK_LIB_DIR)/libdpdk.so
else
	DPDK_TARGET := $(DPDK_LIB_DIR)/libdpdk.a
endif

# Generate appropriate DPDK_LDLIBS based on the dpdk defconfig
include $(OP_ROOT)/mk/dpdk_ldlibs.mk

# Generate appropriate DPDK_DEFINES based on the current compiler flags
include $(OP_ROOT)/mk/dpdk_defines.mk

# Update global build variables
OP_INC_DIRS += $(DPDK_INC_DIR)
OP_LIB_DIRS += $(DPDK_LIB_DIR)
OP_LDLIBS += $(DPDK_LDLIBS) -pthread -ldl
OP_DEFINES += $(DPDK_DEFINES)

###
# DPDK build rules
###

$(DPDK_BLD_DIR)/.config: $(DPDK_DEFCONFIG)
	cp $(DPDK_DEFCONFIG) $(DPDK_SRC_DIR)/config
	cd $(DPDK_SRC_DIR) && $(MAKE) config T=$(DPDK_DEFTARGET) O=$(DPDK_BLD_DIR)

$(DPDK_TARGET): $(DPDK_BLD_DIR)/.config
	cd $(DPDK_BLD_DIR) && $(MAKE) EXTRA_CFLAGS="$(strip $(OP_CSTD) $(OP_COPTS))"

.PHONY: dpdk
dpdk: $(DPDK_TARGET)

.PHONY: clean_dpdk
clean_dpdk:
	@rm -f $(DPDK_SRC_DIR)/config/$(notdir $(DPDK_DEFCONFIG))
	@rm -rf $(DPDK_BLD_DIR)
clean: clean_dpdk
