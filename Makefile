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

release: bin
	mkdir -p $(BUNDLE)/Contents 
	mkdir -p $(BUNDLE)/Contents/MacOS 
	mkdir -p $(BUNDLE)/Contents/Resources 
	mkdir -p $(BUNDLE)/Contents/Frameworks

	cp -RH /Library/Frameworks/SDL2.framework $(BUNDLE)/Contents/Frameworks/ 
	rm -fr $(BUNDLE)/Contents/Frameworks/SDL2.framework/Versions/A/Headers/ 
	rm -fr $(BUNDLE)/Contents/Frameworks/SDL2.framework/Headers 

	cp -RH /Library/Frameworks/SDL2_ttf.framework $(BUNDLE)/Contents/Frameworks/ 
	rm -fr $(BUNDLE)/Contents/Frameworks/SDL2_ttf.framework/Versions/A/Headers/ 
	rm -fr $(BUNDLE)/Contents/Frameworks/SDL2_ttf.framework/Headers 

	cp -RH /Library/Frameworks/SDL2_image.framework $(BUNDLE)/Contents/Frameworks/ 
	rm -fr $(BUNDLE)/Contents/Frameworks/SDL2_image.framework/Versions/A/Headers/ 
	rm -fr $(BUNDLE)/Contents/Frameworks/SDL2_image.framework/Headers

	cp -a assets/. $(BUNDLE)/Contents/Resources

	cp $(BIN_DIR)/$(PROGRAM_NAME) $(BUNDLE)/Contents/MacOS

clean:
	rm $(OBJ_DIR)/*.o
	rm $(BIN_DIR)/$(PROGRAM_NAME)
	rm -rf $(BUILD_DIR)/$(PROGRAM_NAME).app
