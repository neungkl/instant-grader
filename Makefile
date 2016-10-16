CC := g++
CFLAGS := -g -Wno-write-strings -std=c++11
TARGET := ./build
SOURCE := ./src
OUTPUT_NAME := grader-build

SRCS := $(wildcard $(SOURCE)/*.cpp)
OBJS := $(patsubst $(SOURCE)/%.cpp,$(TARGET)/%.o,$(SRCS))

all: mkdir build
	@echo "Build Finish.\nRun the program by typing.";
	@echo "\033[1m./$(OUTPUT_NAME) add.cpp"

mkdir:
	@mkdir -p $(TARGET)/

build: $(OBJS)
	@$(CC) $(CFLAGS) $^ -o $(OUTPUT_NAME)

$(TARGET)/%.o: $(SOURCE)/%.cpp
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(TARGET) *.o

.PHONY: all clean
