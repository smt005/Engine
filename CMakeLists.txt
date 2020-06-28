cmake_minimum_required(VERSION 3.15)

# Переменные
set(CMAKE_CXX_STANDARD 17)

# Расположение файлов библиотеки
FILE(GLOB_RECURSE INCLUDES ${ENGINE_SOURCE}/*.h)
FILE(GLOB_RECURSE SOURCES ${ENGINE_SOURCE}/*.cpp)

# Расположение заголовочных файлов
include_directories(${GAME_SOURCE})

# ThirdPathy
include_directories(${GLM_INCLUDES})
include_directories(${JSONCPP_INCLUDES})
include_directories(${GLFW_INCLUDES})
include_directories(${GLEW_INCLUDES})

# Создание статической библиотеки
add_library(${ENGINE_NAME} STATIC ${INCLUDES} ${SOURCES})

