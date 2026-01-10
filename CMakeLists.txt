include(FetchContent)

cmake_minimum_required(VERSION 3.10...3.31)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
  name
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
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.5
  GIT_SHALLOW ON
  EXCLUDE_FROM_ALL
  SYSTEM
)

FetchContent_MakeAvailable(spdlog)
FetchContent_MakeAvailable(raylib)

add_subdirectory(src)
