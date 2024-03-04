# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -Wextra

# Source files
SRCS = c_tokenizer2.l cmos.cpp

# Object files
OBJS = $(SRCS:.cpp)

# Executable
TARGET = cmos

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
