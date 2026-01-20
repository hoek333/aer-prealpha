include(sources.cmake)

set(NAME input)
add_library(${NAME} ${SOURCES})
target_compile_features(${NAME} PUBLIC cxx_std_20)
target_include_directories(
  ${NAME}
  PRIVATE ${CMAKE_SOURCE_DIR}/lib
)
target_link_libraries(
  ${NAME}
  PUBLIC
    spdlog
    raylib
    SPSCQueue
)

# look for OS-specific deps
find_package(X11)
if(X11_FOUND)
  message("X11 available")
  target_link_libraries(${NAME} PUBLIC X11::X11 X11::Xi)
  target_include_directories(${NAME} PUBLIC ${X11_INCLUDE_DIR})
  target_compile_definitions(${NAME} PUBLIC AER_HAS_X11)
endif()
