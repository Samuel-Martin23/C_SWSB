CC := clang
FLAGS := -std=c17 -Wall -Wextra -Wconversion -Wunreachable-code -Wnewline-eof -Wno-error=unused-variable -Wshadow -Wfloat-equal -Wcovered-switch-default -Wunreachable-code-break

NAME := SWSB
BUILD := build
INCLUDE := include
OBJ := obj
SRC := src

SOURCES := $(wildcard $(SRC)/*.c)
OBJECTS := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))
HEADERS := $(wildcard $(INCLUDE)/*.h)

$(BUILD)/$(NAME): $(OBJECTS)
	$(CC) $^ -o $@ -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_ttf

$(OBJ)/%.o: $(SRC)/%.c $(HEADERS)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm $(BUILD)/$(NAME)
	rm $(OBJ)/*.o
