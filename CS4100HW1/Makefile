# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -Wextra

# Source files
SRCS = scanner.l cmos.cpp

# Executable
TARGET = cmos scanner

all: $(TARGET)

scanner: scanner.l
	flex scanner.l
	$(CC) -o scanner lex.yy.c

cmos: cmos.cpp
	$(CC) -o cmos cmos.cpp

test: all PlagarismDetector
	chmod +x PlagarismDetector
	./PlagarismDetector Examples

clean:
	rm -f $(TARGET) lex.yy.c
