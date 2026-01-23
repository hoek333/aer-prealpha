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
    core
  PUBLIC
    spdlog
    raylib
    SPSCQueue
)

if(LINUX)
if(AER_HAS_LIB_X11)
  target_include_directories(${NAME} PUBLIC ${X11_INCLUDE_DIR})
  target_link_libraries(${NAME} PUBLIC X11::X11 X11::Xi)
endif()
endif()

if(WIN32)
  target_link_libraries(${NAME} PUBLIC comctl32)
endif()
