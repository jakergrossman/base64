TARGET=base64
BUILD_DIR=build
SRC_DIR=src
TEST_DIR=test

CC=g++
CXXFLAGS=-std=c++14 -Wall -Wextra -Wshadow
INCLUDE_FLAGS=-I$(SRC_DIR)

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(SRC_DIR)/$(TARGET).cpp
	$(CC) -o $@ $(CXXFLAGS) $(INCLUDE_FLAGS) $^
