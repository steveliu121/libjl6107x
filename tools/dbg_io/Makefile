.PHONY: all

TARGET := $(patsubst %.c, %, $(wildcard ./*.c))
TARGET_LINUX := $(TARGET)
TARGET_WIN := $(patsubst %, %.exe, $(TARGET))

ifeq ($(CONFIG_OS_LINUX), y)
TARGET = $(TARGET_LINUX)
endif
ifeq ($(CONFIG_OS_WIN), y)
TARGET = $(TARGET_WIN)
endif

all: $(TARGET)

%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $<

ifeq ($(CONFIG_OS_LINUX), y)
$(TARGET): %: %.o
	@$(CC) -o $@ $< $(JL_SDK_DIR)/$(TARGET_STATIC_LIB) $(LDFLAGS)
endif

ifeq ($(CONFIG_OS_WIN), y)
$(TARGET): %.exe: %.o
	@$(CC) -o $@ $< $(JL_SDK_DIR)/$(TARGET_STATIC_LIB) $(LDFLAGS)
endif

clean:
ifneq ($(wildcard *.o $(TARGET)),)
	@echo "Cleaning dbg_io..."
	@rm -r *.o $(TARGET)
endif
