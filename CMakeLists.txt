cmake_minimum_required(VERSION 3.14)
project(SudokuSolver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Fetch SFML
set(SFML_BUILD_AUDIO FALSE CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK FALSE CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(
  sfml
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 2.6.1
)
FetchContent_MakeAvailable(sfml)

# Source files
set(SOURCES
    src/main.cpp
    src/SudokuBoard.cpp
    src/Solver.cpp
    src/Generator.cpp
    src/Visualizer.cpp
)

# Executable
add_executable(SudokuSolver ${SOURCES})
target_link_libraries(SudokuSolver PRIVATE sfml-graphics sfml-window sfml-system)

# Test Executable (Headless)
add_executable(SolverTest src/test_solver.cpp src/SudokuBoard.cpp src/Solver.cpp)
# target_compile_definitions(SudokuSolver PRIVATE SFML_STATIC) # Using shared libs
