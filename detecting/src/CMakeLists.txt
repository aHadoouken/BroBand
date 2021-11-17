# минимальная версия для запуска
cmake_minimum_required(VERSION 3.16)

# имя проекта
project(detecting_service)

add_compile_options(-g -O0 -Wextra -Wall -Wextra -Wpedantic -lgcov --coverage)
add_link_options( -O0 -lgcov --coverage)

# добавляем поиск файлов в директории include
include_directories("../include")

add_compile_options(-pthread)

# библиотека с общими функциями
set(DETECT_LIB_SOURCE detecting.cpp)

# создание статической библиотеки
add_library(detect STATIC ${DETECT_LIB_SOURCE})

# создание бинарных файлов
add_executable(main_detect main.cpp ${LIB_SOURCE_HEADER})

# соединение библиотеки и основного бинарного файла
target_link_libraries(main_detect detect m pthread)

