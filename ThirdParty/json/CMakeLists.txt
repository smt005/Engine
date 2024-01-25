cmake_minimum_required(VERSION 3.8)

# Переменные
set(CMAKE_CXX_STANDARD 20)

# Расположение файлов библиотеки
file(GLOB_RECURSE INCLUDES ${JSONCPP_INCLUDES}/*.h)
file(GLOB_RECURSE SOURCES ${JSONCPP_SOURCE}/*.cpp)
source_group(TREE ${JSONCPP_DIR} TREE PREFIX "" FILES ${INCLUDES} ${SOURCES})

# Создание статической библиотеки
add_library(${JSONCPP_NAME} STATIC ${INCLUDES} ${SOURCES})

