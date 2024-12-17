# Compiler and flags
CC = gcc
CFLAGS = -g

# Object files
OBJS = main.o mechanics.o initialize.o

# Target executable
TARGET = program

# Default target to build the program
all: $(TARGET)

# Rule to create the executable from object files
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Rule for compiling main.c
main.o: main.c mechanics.h initialize.h
	$(CC) $(CFLAGS) -c main.c

# Rule for compiling mechanics.c
mechanics.o: mechanics.c mechanics.h
	$(CC) $(CFLAGS) -c mechanics.c

# Rule for compiling initialize.c
initialize.o: initialize.c initialize.h
	$(CC) $(CFLAGS) -c initialize.c

# Clean target to remove generated files
clean:
	rm -f $(OBJS) $(TARGET)
