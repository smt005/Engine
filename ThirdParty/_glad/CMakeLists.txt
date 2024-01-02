cmake_minimum_required(VERSION 3.15)

# Переменные
set(CMAKE_CXX_STANDARD 17)

# Расположение файлов библиотеки
FILE(GLOB_RECURSE INCLUDES ${GLAD_INCLUDES}/*.h)
FILE(GLOB_RECURSE SOURCES ${GLAD_SOURCE}/*.c)
source_group(TREE ${GLAD_DIR} TREE PREFIX "" FILES ${INCLUDES} ${SOURCES})

# Расположение заголовочных файлов
include_directories(${INCLUDES})

# Создание статической библиотеки
add_library(${GLAD_NAME} STATIC ${INCLUDES} ${SOURCES})
set_target_properties(${GLAD_NAME} PROPERTIES LINKER_LANGUAGE CXX)
