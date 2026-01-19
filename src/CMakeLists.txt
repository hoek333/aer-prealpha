include(sources.cmake)

add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_include_directories(
  ${PROJECT_NAME}
  PRIVATE
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE
    core
    input
  PRIVATE
    spdlog
    raylib
)
