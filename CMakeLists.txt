cmake_minimum_required(VERSION 3.15)
project(kaleidoscope LANGUAGES CXX)

# 1) Enforce C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 2) Export a compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 3) Find llvm-config
find_program(LLVM_CONFIG_EXEC NAMES llvm-config REQUIRED)

# 4) Grab LLVM compile‑time flags (-I, -std, -stdlib, etc.)
execute_process(
  COMMAND ${LLVM_CONFIG_EXEC} --cxxflags
  OUTPUT_VARIABLE LLVM_CXXFLAGS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
separate_arguments(LLVM_CXXFLAGS_LIST UNIX_COMMAND "${LLVM_CXXFLAGS}")
add_compile_options(${LLVM_CXXFLAGS_LIST})

# 5) Grab LLVM link‑time flags (-L, -lLLVM*, system libs)
execute_process(
  COMMAND ${LLVM_CONFIG_EXEC} --ldflags --system-libs --libs all
  OUTPUT_VARIABLE LLVM_LDFLAGS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
separate_arguments(LLVM_LDFLAGS_LIST UNIX_COMMAND "${LLVM_LDFLAGS}")

# 6) Tell CMake where YOUR headers live
include_directories(${CMAKE_SOURCE_DIR}/include)

# 7) Collect sources & build
file(GLOB_RECURSE SRC
     ${CMAKE_SOURCE_DIR}/src/*.cpp
     ${CMAKE_SOURCE_DIR}/main.cpp)

add_executable(kaleidoscope ${SRC})
target_link_libraries(kaleidoscope PRIVATE ${LLVM_LDFLAGS_LIST})

