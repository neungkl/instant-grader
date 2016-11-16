CXX ?= g++
CFLAGS := -g -Wno-write-strings -std=c++11
TARGET := ./build
SOURCE := ./src
TEST := ./test
OUTPUT_NAME := grader-dev
BUILD_NAME := grader

SRCS := $(wildcard $(SOURCE)/*.cpp)
OBJS := $(patsubst $(SOURCE)/%.cpp,$(TARGET)/%.o,$(SRCS))

build: prepareBuild all

prepareBuild:
	$(eval OUTPUT_NAME = $(BUILD_NAME))

all: mkdir bundle
	@echo "Build Finish.\nRun the program by typing.";
	@echo "\033[1m./$(OUTPUT_NAME) add.cpp"

mkdir:
	@mkdir -p $(TARGET)/

bundle: $(OBJS)
	@$(CC) $(CFLAGS) $^ -o $(OUTPUT_NAME)

$(TARGET)/%.o: $(SOURCE)/%.cpp
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(TARGET) *.o

test: $(filter-out $(TARGET)/main.o, $(OBJS))
	@$(CC) $(CFLAGS) -c $(TEST)/test.cpp -o $(TEST)/test.o
	@$(CC) $(CFLAGS) $^ $(TEST)/test.o -o $(TEST)/test
	@$(TEST)/test
	@rm $(TEST)/test.o

.PHONY: clean test build
