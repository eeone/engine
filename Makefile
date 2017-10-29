EXECUTABLE=program
SRC_DIR=./src
OBJ_DIR=./obj
INCLUDE_DIR=./include
OUTPUT_DIR=./bin

SOURCES=$(wildcard $(SRC_DIR)/*.c)
OBJECTS=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

CC=gcc
CFLAGS=-I$(INCLUDE_DIR) -Wall -g

LIBRARIES=-lSDL2 -lSDL2_image
LDFLAGS=$(LIBRARIES)

all: $(OBJECTS)
	@mkdir -p $(OUTPUT_DIR)
	$(CC) $^ -o $(OUTPUT_DIR)/$(EXECUTABLE) $(LDFLAGS)

$(OBJECTS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -rf $(OUTPUT_DIR) $(OBJ_DIR)
