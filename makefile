TARGET_EXEC ?= $(target).out

BUILD_DIR ?= ./build
SRC_SHARED ?= ./src
SRC_DIRS ?= ./src/$(target)

SHARED := $(shell find $(SRC_SHARED) -maxdepth 1 -name *.c)
OBJS_SHARED := $(SHARED:%=$(BUILD_DIR)/%.o)

SRCS := $(shell find $(SRC_DIRS) -name *.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

LDFLAGS := -lpthread

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))


$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS_SHARED) $(OBJS)
	$(CC) $(OBJS_SHARED) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p