##
## Makefile for gg - a command-line based version control system
##

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -I./include
LDFLAGS = -DNCURSES_DISABLED -lpthread

# Directories
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# Source files
SRCS = $(SRC_DIR)/add.cpp $(SRC_DIR)/branch.cpp $(SRC_DIR)/init.cpp $(SRC_DIR)/kermit.cpp \
       $(SRC_DIR)/learn.cpp $(SRC_DIR)/log.cpp $(SRC_DIR)/main.cpp $(SRC_DIR)/merge.cpp \
       $(SRC_DIR)/status.cpp $(SRC_DIR)/stubs.cpp
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Add stubs.cpp to dependencies
$(OBJ_DIR)/stubs.o: $(SRC_DIR)/stubs.cpp $(INCLUDE_DIR)/avl.h $(INCLUDE_DIR)/dag.h $(INCLUDE_DIR)/trie.h $(INCLUDE_DIR)/skiplist.h $(INCLUDE_DIR)/disjoint.h $(INCLUDE_DIR)/bloom.h

# Binary target
TARGET = $(BIN_DIR)/gg

# Default target
all: directories force-compile $(TARGET)

# Simplified build for systems without ncurses
force-compile:
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/main.cpp -o $(OBJ_DIR)/main.o
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/stubs.cpp -o $(OBJ_DIR)/stubs.o

# Create necessary directories
directories:
	mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Link objects into executable
$(TARGET): $(OBJ_DIR)/main.o $(OBJ_DIR)/stubs.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | directories
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Install rule (copy to /usr/local/bin or similar)
install: all
	cp $(TARGET) /usr/local/bin/

# Phony targets
.PHONY: all clean install directories

# Dependencies
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp $(INCLUDE_DIR)/gg.h
$(OBJ_DIR)/init.o: $(SRC_DIR)/init.cpp $(INCLUDE_DIR)/gg.h $(INCLUDE_DIR)/trie.h $(INCLUDE_DIR)/dag.h $(INCLUDE_DIR)/avl.h $(INCLUDE_DIR)/skiplist.h $(INCLUDE_DIR)/disjoint.h $(INCLUDE_DIR)/bloom.h
$(OBJ_DIR)/add.o: $(SRC_DIR)/add.cpp $(INCLUDE_DIR)/gg.h $(INCLUDE_DIR)/trie.h $(INCLUDE_DIR)/bloom.h
$(OBJ_DIR)/kermit.o: $(SRC_DIR)/kermit.cpp $(INCLUDE_DIR)/gg.h $(INCLUDE_DIR)/trie.h $(INCLUDE_DIR)/dag.h $(INCLUDE_DIR)/skiplist.h $(INCLUDE_DIR)/avl.h
$(OBJ_DIR)/status.o: $(SRC_DIR)/status.cpp $(INCLUDE_DIR)/gg.h $(INCLUDE_DIR)/trie.h $(INCLUDE_DIR)/bloom.h $(INCLUDE_DIR)/avl.h $(INCLUDE_DIR)/dag.h
$(OBJ_DIR)/log.o: $(SRC_DIR)/log.cpp $(INCLUDE_DIR)/gg.h $(INCLUDE_DIR)/dag.h $(INCLUDE_DIR)/skiplist.h $(INCLUDE_DIR)/avl.h
$(OBJ_DIR)/branch.o: $(SRC_DIR)/branch.cpp $(INCLUDE_DIR)/gg.h $(INCLUDE_DIR)/avl.h $(INCLUDE_DIR)/dag.h
$(OBJ_DIR)/merge.o: $(SRC_DIR)/merge.cpp $(INCLUDE_DIR)/gg.h $(INCLUDE_DIR)/dag.h $(INCLUDE_DIR)/disjoint.h $(INCLUDE_DIR)/avl.h $(INCLUDE_DIR)/trie.h $(INCLUDE_DIR)/skiplist.h
$(OBJ_DIR)/learn.o: $(SRC_DIR)/learn.cpp $(INCLUDE_DIR)/gg.h $(INCLUDE_DIR)/trie.h $(INCLUDE_DIR)/dag.h $(INCLUDE_DIR)/avl.h $(INCLUDE_DIR)/skiplist.h $(INCLUDE_DIR)/disjoint.h $(INCLUDE_DIR)/bloom.h
