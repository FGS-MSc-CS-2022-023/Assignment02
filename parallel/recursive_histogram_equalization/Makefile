# Compiler and flags
CC = icpc
CFLAGS = -Wall -O2 -w -lm -fopenmp

# Source files and target executable
SRCS = matrix.c helpers.c main.c
OBJS = $(SRCS:.c=.o)
TARGET = rehiste

# Build rule for the target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Build rule for object files
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Clean rule to remove object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)
	rm -f *Channel.txt
