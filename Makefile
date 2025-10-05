CC := clang
CFLAGS := -Wall -Wextra -Werror -I./include
SRC_DIR := src
SRC_FILES := main.c client.c
OUT_DIR := build
TARGET := $(OUT_DIR)/C_HTTP

OBJECTS := $(patsubst %.c, $(OUT_DIR)/%.o, $(SRC_FILES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(OUT_DIR)/%.o: $(SRC_DIR)/%.c | $(OUT_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

clean:
	rm -rf $(OUT_DIR)
