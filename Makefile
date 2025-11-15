CC := gcc
CFLAGS := -std=c23 -Wpedantic -Wall -Wextra -Werror
OUT_FOLDER := build

SANDBOX_NAME := $(OUT_FOLDER)/Sandbox
SANDBOX_SRC_FOLDER := sandbox/src
SANDBOX_C_FILES := main.c
SANDBOX_O_FILES := $(patsubst %.c, $(OUT_FOLDER)/%.o, $(SANDBOX_C_FILES))

LIB_NAME := http
LIB_PATH := $(OUT_FOLDER)/lib$(LIB_NAME).a
LIB_SRC_FOLDER := lib/src
LIB_C_FILES := server.c
LIB_O_FILES := $(patsubst %.c, $(OUT_FOLDER)/%.o, $(LIB_C_FILES))

all: $(LIB_PATH) $(SANDBOX_NAME)

$(OUT_FOLDER):
	mkdir -p $(OUT_FOLDER)

#TODO: Can we combine these two rules for generating o files together
$(SANDBOX_O_FILES): $(SANDBOX_SRC_FOLDER)/$(SANDBOX_C_FILES) | $(OUT_FOLDER)
	$(CC) $(CFLAGS) -I./lib/include -c -o $@ $<

$(LIB_O_FILES): $(LIB_SRC_FOLDER)/$(LIB_C_FILES) | $(OUT_FOLDER)
	$(CC) $(CFLAGS) -I./lib/include -c -o $@ $<

$(SANDBOX_NAME): $(SANDBOX_O_FILES) $(LIB_PATH)
	$(CC) -o $(SANDBOX_NAME) $(SANDBOX_O_FILES) -L./$(OUT_FOLDER) -l$(LIB_NAME) 

$(LIB_PATH): $(LIB_O_FILES)
	ar rcs $(LIB_PATH) $(LIB_O_FILES)

clean:
	rm -rf $(OUT_FOLDER)
