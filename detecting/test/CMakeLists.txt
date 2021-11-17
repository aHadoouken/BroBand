cmake_minimum_required(VERSION 3.16)
project(tests)

add_compile_options(-g -O0 -Wextra -Wall -Wextra -Wpedantic -lgcov --coverage)
add_link_options( -O0 -lgcov --coverage)

include_directories("../include")
include_directories("../src")

# Включаем тестирование
enable_testing()

# Загружаем библиотеки ОБЯЗАТЕЛЬНО
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

# Исполняемый файл для динамической библиотеки и многопоточной реализации
add_executable(test_detect test.cpp)

# Линкуем (соединяем) программу со статической библиотекой
target_link_libraries(test_detect ${GTEST_LIBRARIES} detect pthread)
