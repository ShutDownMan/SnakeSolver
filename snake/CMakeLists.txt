project(Snake)

cmake_minimum_required(VERSION 3.15)

set(CMAKE_C_STANDARD 11)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(Snake src/snake_path.c src/snake.c src/screen.c src/input.c src/main.c src/board.c src/init.c)
target_link_libraries(Snake ${CURSES_LIBRARIES} m)
