# Makefile for Sketcher program

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++17 -O3  # Enable optimization for the normal binary

# Profiling flags for Valgrind
PROFILING_FLAGS = -g  # Enable debugging symbols and disable optimization for better profiling

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Target executable names
TARGET = sketcher
TARGET_PROF = sketcher_prof

# Source and object files
SRCS = $(shell find $(SRC_DIR) -name "*.cpp")
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Default target
all: $(BIN_DIR)/$(TARGET) $(BIN_DIR)/$(TARGET_PROF)

# Create executable without profiling
$(BIN_DIR)/$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Create executable with profiling flags
$(BIN_DIR)/$(TARGET_PROF): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(PROFILING_FLAGS) -o $@ $^

# Compile each .cpp file to .o, maintaining subdirectory structure
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build and bin directories
clean:
	rm -rf $(BUILD_DIR)/*.o $(BIN_DIR)/$(TARGET) $(BIN_DIR)/$(TARGET_PROF)

# Run the normal version of the program
run: all
	$(BIN_DIR)/$(TARGET)

# Run the profiling version of the program with Valgrind
run_prof: all
	valgrind --tool=callgrind $(BIN_DIR)/$(TARGET_PROF)

# Clean profiling output files
clean_profiling:
	rm -rf callgrind.out.*

.PHONY: all clean run run_prof clean_profiling
