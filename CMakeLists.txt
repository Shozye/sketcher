# CMakeLists.txt for Sketcher project

# Minimum CMake version
cmake_minimum_required(VERSION 3.14)

# Project name and version
project(Sketcher VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3")

# Profiling flags
set(PROFILING_FLAGS "-g")

# Source and include directories
set(SRC_DIR src)
file(GLOB_RECURSE SRCS "${SRC_DIR}/*.cpp")

# Output directories
set(BIN_DIR "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

# Main executable without profiling
add_executable(sketcher ${SRCS})
target_compile_options(sketcher PRIVATE -O3)

# Profiling version of the executable
add_executable(sketcher_prof ${SRCS})
target_compile_options(sketcher_prof PRIVATE ${PROFILING_FLAGS})

# Run target to execute the application
add_custom_target(run
    COMMAND ${BIN_DIR}/sketcher
    DEPENDS sketcher
    COMMENT "Running sketcher..."
)

# Run profiling version with Valgrind
find_program(VALGRIND_EXECUTABLE valgrind)
if (VALGRIND_EXECUTABLE)
    add_custom_target(run_prof
        COMMAND ${VALGRIND_EXECUTABLE} --tool=callgrind ${BIN_DIR}/sketcher_prof
        DEPENDS sketcher_prof
        COMMENT "Running sketcher_prof with Valgrind..."
    )
else()
    message(WARNING "Valgrind not found. Profiling target run_prof will not be available.")
endif()


# Adding Catch2 and setting up tests
find_package(Catch2 3 REQUIRED)
set(TEST_DIR test)
file(GLOB_RECURSE TESTS "${TEST_DIR}/*.cpp")

# Define TEST_SRCS for tests, excluding main.cpp
set(TEST_SRCS ${SRCS})
list(REMOVE_ITEM TEST_SRCS "${CMAKE_SOURCE_DIR}/${SRC_DIR}/main.cpp")

message("${TEST_SRCS}") 
 

# Create the tests executable with TEST_SRCS (excluding main.cpp)
add_executable(tests ${TESTS} ${TEST_SRCS})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

