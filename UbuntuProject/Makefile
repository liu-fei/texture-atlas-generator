############################################################
# Name         : Makefile
# Author       : Fei Liu (liu.s.fei@gmail.com)
# Description  : Makefile For atlastgenerator Application
############################################################

TARGET := atlas_generator
BUILD_DIR := build
SRC_DIR := ../src

SOURCES := $(shell find $(SRC_DIR) -type f -name *.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(SOURCES:.cpp=.o))

CXXFLAGS := -g # -Wall 
LIB := -lpng -lz
INC := -I../thirdparty_common/include -Isrc 

CC := g++ -std=c++11

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	$(CC) $^ -o $(TARGET) $(LIB)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo " Compiling..."
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Removing $(BUILD_DIR) folder and the executable file $(TARGET)."; 
	$(RM) -r $(BUILD_DIR) $(TARGET)

.PHONY: clean
