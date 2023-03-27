CC = clang
CFLAGS = -std=c99 -Wall -Wextra -pedantic
LDFLAGS = -lm -lncurses

BUILD_DIR = build
SRC_DIR = src

TARGET = juno
DEBUG_TARGET = juno_debug

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
DEBUG_OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/debug/%.o,$(SRCS))

.PHONY: all debug clean

all: $(BUILD_DIR)/$(TARGET)

debug: CFLAGS += -g
debug: $(BUILD_DIR)/$(DEBUG_TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/$(DEBUG_TARGET): $(DEBUG_OBJS)
	$(CC) $(CFLAGS) $(DEBUG_OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/debug/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -g -c $< -o $@

clean:
	rm -f $(OBJS) $(DEBUG_OBJS) $(BUILD_DIR)/$(TARGET) $(BUILD_DIR)/$(DEBUG_TARGET)
