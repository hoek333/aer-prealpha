include(sources.cmake)

add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC
    spdlog
    raylib
)
