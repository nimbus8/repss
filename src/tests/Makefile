
#-------------------------------------
# Generic Makefile for C++11 Projects
#-------------------------------------

# Executable name
TARGET	= test_repss_string

# Compiler
CC		= clang++

# Compiler Flags
CFLAGS	= -g -std=c++11 -O0 -Wall -pedantic 

# Linker Flags
LFLAGS	= 

#-------------------------------------
# Nothing to change below here
#-------------------------------------
SRC=$(wildcard *.cpp)
#HEADERS=$(wildcard ../includes/*.hpp)
OBJS=$(SRC:.cpp=.o) 

all: $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) -o ../../bin/tests/$(TARGET) $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -f bin/$(TARGET) $(OBJS)
