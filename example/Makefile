.PHONY: all

EXAMPLE_FILES := $(wildcard ./*.c)
ifeq ($(CONFIG_OS_LINUX), y)
TARGET_EXAMPLE := $(patsubst %.c, %, $(EXAMPLE_FILES))
endif
ifeq ($(CONFIG_OS_WIN), y)
TARGET_EXAMPLE := $(patsubst %.c, %.exe, $(EXAMPLE_FILES))
endif

all: $(TARGET_EXAMPLE)

%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $<

ifeq ($(CONFIG_OS_LINUX), y)
$(TARGET_EXAMPLE): %: %.o
	@$(CC) -o $@ $< $(JL_SDK_DIR)/$(TARGET_STATIC_LIB) $(LDFLAGS)
endif

ifeq ($(CONFIG_OS_WIN), y)
$(TARGET_EXAMPLE): %.exe: %.o
	@$(CC) -o $@ $< $(JL_SDK_DIR)/$(TARGET_STATIC_LIB) $(LDFLAGS)
endif

clean:
ifneq ($(wildcard *.o $(TARGET_EXAMPLE)),)
	@echo "Cleaning Example..."
	@rm -r *.o $(TARGET_EXAMPLE)
endif