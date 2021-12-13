project(aoc)

file(GLOB src ${CMAKE_CURRENT_SOURCE_DIR}/*c)

add_executable(aoc ${src})