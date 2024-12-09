cmake_minimum_required(VERSION 3.10)
project(ChessGame)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Находим пакет FLTK
set(FLTK_SKIP_FLUID True)
set(FLTK_SKIP_FORMS True)

find_package(FLTK 1.3.8 EXACT REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(SYSTEM ${FLTK_INCLUDE_DIR})
link_directories(${FLTK_INCLUDE_DIR}/../lib)

set(SOURCES
        frontend.cpp
        backend.cpp
        bot.cpp
        main_chess.cpp
)
# Добавляем исполняемый файл
add_executable(chess ${SOURCES})

target_link_libraries(chess ${FLTK_LIBRARIES})

set(TESTER_SOURCES
        frontend.cpp
        tester.cpp
        backend.cpp
        bot.cpp
)
# Включаем заголовочные файлы FLTK
include_directories(${FLTK_INCLUDE_DIR})

add_executable(ChessTester ${TESTER_SOURCES})

target_link_libraries(ChessTester ${FLTK_LIBRARIES})