# Compiler and flags
CC = icc
CFLAGS = -Wall -O2 -w

# Source files and target executable
SRCS = helpers.c matrix.c main.c
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
