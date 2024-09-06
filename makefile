CC = gcc -g # -Wall -Wextra
SRC_DIR = ./code
OBJ_DIR = ./obj
TARGET = ./a.out

# Find all the C source files in the source directory
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Define the object files corresponding to the source files
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Default target
all: $(TARGET)
	$(TARGET)

# Rule to link the object files into the final executable
$(TARGET): $(OBJS)
	$(CC) -o $@ $^

# Rule to compile each source file into an object file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
	rm .log

run:
	$(TARGET)

build: $(TARGET)

.PHONY: all clean run build