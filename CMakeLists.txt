include(FetchContent)

cmake_minimum_required(VERSION 3.10...3.31)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
  aer
  LANGUAGES CXX
)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog
  GIT_TAG v1.16.0
  GIT_SHALLOW ON
  EXCLUDE_FROM_ALL
  SYSTEM
)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.5
  GIT_SHALLOW ON
  EXCLUDE_FROM_ALL
  SYSTEM
)

FetchContent_Declare(
  SPSCQueue
  GIT_REPOSITORY https://github.com/rigtorp/SPSCQueue.git
  GIT_TAG v1.1
  GIT_SHALLOW ON
  EXCLUDE_FROM_ALL
  SYSTEM
)

FetchContent_MakeAvailable(spdlog)
FetchContent_MakeAvailable(raylib)
FetchContent_MakeAvailable(SPSCQueue)

add_subdirectory(lib/core)
add_subdirectory(lib/input)
add_subdirectory(src)
