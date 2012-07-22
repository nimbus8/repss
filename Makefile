
#-------------------------------------
# Generic Makefile for C++11 Projects
#-------------------------------------

# Executable name
TARGET	= repss

# Compiler
CC		= clang++

# Compiler Flags
CFLAGS	= -g -std=c++11 -stdlib=libc++ -O0 -Wall -pedantic 

# Linker Flags
LFLAGS	= 

#-------------------------------------
# Nothing to change below here
#-------------------------------------
SRC=$(wildcard src/*.cpp)
OBJS=$(SRC:.cpp=.o)

all: $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) -o bin/$(TARGET) $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -f bin/$(TARGET) $(OBJS)
