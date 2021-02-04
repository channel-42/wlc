SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

INSTALL_PATH := ~/.local/bin/

OUT := $(BIN_DIR)/wlc
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CPPFLAGS := -Iinclude -MMD -MP
CFLAGS   := -Wall 
LDFLAGS  := -Llib
LDLIBS   := -lm -lcurl

.PHONY: all clean

all: $(OUT)

$(OUT): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

install:
	cp $(OUT) $(INSTALL_PATH)
clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

-include $(OBJ:.o=.d)
