CC := clang
FLAGS := -std=c17 -Wall -Wextra -Wconversion -Wunreachable-code -Wnewline-eof -Wno-error=unused-variable -Wshadow -Wfloat-equal -Wcovered-switch-default -Wunreachable-code-break

BIN_DIR := bin
BUILD_DIR := build
INCLUDE_DIR := include
OBJ_DIR := obj
SRC_DIR := src

SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))
HEADERS := $(wildcard $(INCLUDE_DIR)/*.h)

ASSETS_PATH := ../assets/
PROGRAM_NAME := SWSB
BUNDLE := $(BUILD_DIR)/$(PROGRAM_NAME).app

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	mkdir -p $(@D)
	$(CC) $(FLAGS) -c $< -DASSETS_PATH=\"$(ASSETS_PATH)\" -o $@

bin: $(OBJECTS)
	mkdir -p $@
	$(CC) $^ -o $(BIN_DIR)/$(PROGRAM_NAME) -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_ttf

debug: FLAGS += -g
debug: bin

release: ASSETS_PATH := ../Resources/
release: bin
	mkdir -p $(BUNDLE)/Contents/
	mkdir -p $(BUNDLE)/Contents/MacOS/
	mkdir -p $(BUNDLE)/Contents/Resources/

	cp -a assets/. $(BUNDLE)/Contents/Resources/
	cp $(BIN_DIR)/$(PROGRAM_NAME) $(BUNDLE)/Contents/MacOS/
	mv $(BUNDLE)/Contents/Resources/Info.plist $(BUNDLE)/Contents/

clean:
	rm $(OBJ_DIR)/*.o
	rm $(BIN_DIR)/$(PROGRAM_NAME)
	rm -rf $(BUILD_DIR)/$(PROGRAM_NAME).app
