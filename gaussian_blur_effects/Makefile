# Compiler and flags
CC = gcc
CFLAGS = -Wall -O2 -w

# Source files and target executable
SRCS = matrix.c gb.c
OBJS = $(SRCS:.c=.o)
TARGET = a

# Build rule for the target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Build rule for object files
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Clean rule to remove object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)
