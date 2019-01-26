# Compiler
CC = g++

# Name
NAME = main

# Flags
IFLAGS = -I $(IDIR)
CFLAGS = -Wall -std=c++11
LFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Directories
IDIR = ./include/
SDIR = ./src/
ODIR = $(SDIR)obj/

# Header files
_DEPS = GameOfLive.h
DEPS = $(patsubst %, $(IDIR)%, $(_DEPS))

# Source files
_OBJS = main.cpp GameOfLive.cpp
OBJS = $(patsubst %.cpp, $(ODIR)%.o, $(_OBJS))

# Linking
$(NAME): $(OBJS) $(DEPS)
	$(CC) -o $@ $(OBJS) $(IFLAGS) $(CFLAGS) $(LFLAGS)

# Compiling
$(ODIR)%.o: $(SDIR)%.cpp $(DEPS)
	$(CC) -o $@ -c $< $(IFLAGS) $(CFLAGS)

# $@ - Name of the target
# $< - Name of the first dependency

.PHONY: clean
clean:
	rm $(ODIR)*.o

