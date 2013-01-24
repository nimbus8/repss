
#-------------------------------------
# Generic Makefile for C++11 Projects
#-------------------------------------

# Executable name
TARGET	= scratch

# Compiler
CC		= clang++

# Compiler Flags
CFLAGS	= -g -std=c++11 -stdlib=libc++ -O0 -Wall -pedantic 

# Linker Flags
LFLAGS	= 

#-------------------------------------
# Nothing to change below here
#-------------------------------------
SRC=$(wildcard *.cpp)
OBJS=$(SRC:.cpp=.o)

all: $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) -o $(TARGET) $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)
