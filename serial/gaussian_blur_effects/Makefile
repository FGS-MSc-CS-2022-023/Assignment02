# Compiler and flags
CC = icc
CFLAGS = -Wall -O2 -w

# Source files and target executable
SRCS = convolutions.c matrix.c main.c helpers.c
OBJS = $(SRCS:.c=.o)
TARGET = gb

# Build rule for the target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Build rule for object files
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Clean rule to remove object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)