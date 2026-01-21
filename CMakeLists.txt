include(FetchContent)

cmake_minimum_required(VERSION 3.10...3.31)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
  aer
  LANGUAGES CXX
)

# FetchContents -------------------------------------------------------------- #

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

# OS-dependent libs ---------------------------------------------------------- #

if(LINUX)
  message("Detected platform: LINUX")
  find_package(X11)

  if(X11_FOUND)
    find_library(XI_LIBRARY Xi)

    if (XI_LIBRARY)
      message("X11 + XI2 available")
      set(AER_HAS_LIB_X11 TRUE)
      add_compile_definitions(AER_HAS_LIB_X11)
    else()
      message("XI2 not found")
    endif()

  else()
    message("X11 not found")
  endif()
endif()

add_subdirectory(lib/core)
add_subdirectory(lib/input)
add_subdirectory(src)
