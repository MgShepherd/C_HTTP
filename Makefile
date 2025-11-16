CC := gcc
CFLAGS := -std=gnu23 -Wpedantic -Wall -Wextra -Werror
OUT_FOLDER := build

SANDBOX_NAME := $(OUT_FOLDER)/Sandbox
SANDBOX_SRC_FOLDER := sandbox/src/
SANDBOX_C_FILES := $(addprefix $(SANDBOX_SRC_FOLDER), main.c)
SANDBOX_O_FILES := $(patsubst $(SANDBOX_SRC_FOLDER)%.c, $(OUT_FOLDER)/%.o, $(SANDBOX_C_FILES))

LIB_NAME := http
LIB_PATH := $(OUT_FOLDER)/lib$(LIB_NAME).a
LIB_SRC_FOLDER := lib/src/
LIB_C_FILES := $(addprefix $(LIB_SRC_FOLDER), server.c client.c)
LIB_O_FILES := $(patsubst $(LIB_SRC_FOLDER)%.c, $(OUT_FOLDER)/%.o, $(LIB_C_FILES))

all: $(LIB_PATH) $(SANDBOX_NAME)

$(OUT_FOLDER):
	mkdir -p $(OUT_FOLDER)

$(OUT_FOLDER)/%.o: $(LIB_SRC_FOLDER)%.c | $(OUT_FOLDER)
	$(CC) $(CFLAGS) -I./lib/include -c -o $@ $<

$(OUT_FOLDER)/%.o: $(SANDBOX_SRC_FOLDER)%.c | $(OUT_FOLDER)
	$(CC) $(CFLAGS) -I./lib/include -c -o $@ $<

$(SANDBOX_NAME): $(SANDBOX_O_FILES) $(LIB_PATH)
	$(CC) -o $(SANDBOX_NAME) $(SANDBOX_O_FILES) -L./$(OUT_FOLDER) -l$(LIB_NAME) 

$(LIB_PATH): $(LIB_O_FILES)
	ar rcs $(LIB_PATH) $(LIB_O_FILES)

clean:
	rm -rf $(OUT_FOLDER)
