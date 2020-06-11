cmake_minimum_required(VERSION 3.15) # Проверка версии CMake.

# Переменные
set(INCLUDE_DIR "${ENGINE_SOURCE}/Engine.h")
set(SOURCE_DIR "${ENGINE_SOURCE}/Engine.cpp")

#
set(CMAKE_CXX_STANDARD 17)

# Расположение заголовочных файлов
include_directories(${GAME_SOURCE})


# Создание статической библиотеки
add_library(${ENGINE_NAME} STATIC ${INCLUDE_DIR} ${SOURCE_DIR})

