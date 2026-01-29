include(sources.cmake)

# game
set(NAME game)
add_library(${NAME} ${SOURCES})
target_compile_features(${NAME} PUBLIC cxx_std_20)
target_include_directories(
  ${NAME}
  PRIVATE ${CMAKE_SOURCE_DIR}/lib
)
target_link_libraries(
  ${NAME}
  PUBLIC
    core
  PUBLIC
    raylib
)
