.PHONY: all target_lib target_example target_tools cflags
#target_example target_tools
all: target_example target_tools target_lib

# Values definition
JL_SDK_DIR := $(abspath .)
SCRIPT_DIR := $(JL_SDK_DIR)/scripts
EXAMPLE_DIR := $(JL_SDK_DIR)/example
TOOLS_DIR := $(JL_SDK_DIR)/tools
SRC_DIRS := $(JL_SDK_DIR)/src
SRC_DIRS += $(JL_SDK_DIR)/src/drv_common
DRV_DIRS :=
INC_DIR := $(JL_SDK_DIR)/inc
DRIVER_DIR := $(JL_SDK_DIR)/src/driver
PORTABLE_DIR := $(JL_SDK_DIR)/src/portable
SRC_FILES :=

# Built objects
obj-y :=
sobj-y :=

# Build options
CFLAGS :=
CFLAGS_SO :=
LDFLAGS :=
LDFLAGS_SO :=
INCLUDE :=
INCLUDE += -I$(INC_DIR)
CFLAGS += -Wall -Werror -Wno-error=deprecated-declarations -Wextra $(INCLUDE)
# CFLAGS += -ffunction-sections -fdata-sections
# LDFLAGS += -Wl,-gc-sections


#Build procedure
$(if $(wildcard $(JL_SDK_DIR)/.config),, $(error No .config exists!))
$(if $(wildcard $(SCRIPT_DIR)/Makefile.conf),, $(error No Makefile.conf exists!))
$(if $(wildcard $(SCRIPT_DIR)/Makefile.userconf),, $(error No Makefile.userconf exists!))
$(if $(wildcard $(SCRIPT_DIR)/Makefile.build),, $(error No Makefile.build exists!))
$(if $(wildcard $(SCRIPT_DIR)/Makefile.libs),, $(error No Makefile.libs exists!))
include .config
include $(SCRIPT_DIR)/Makefile.conf
include $(SCRIPT_DIR)/Makefile.userconf
include $(SCRIPT_DIR)/Makefile.build
include $(SCRIPT_DIR)/Makefile.libs

export CC JL_SDK_DIR CONFIG_OS_LINUX CONFIG_OS_WIN TARGET_STATIC_LIB CFLAGS LDFLAGS

ifneq ($(CONFIG_RISCV), y)
target_example: target_lib
	$(MAKE) -C $(EXAMPLE_DIR)

target_tools: target_lib
	$(MAKE) -C $(TOOLS_DIR)
endif

clean: libs_clean
	$(MAKE) clean -C $(EXAMPLE_DIR)
	$(MAKE) clean -C $(TOOLS_DIR)
