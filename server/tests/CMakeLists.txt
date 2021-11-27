cmake_minimum_required(VERSION 3.16)
project(tests)

# стандарт
set(CMAKE_CXX_STANDARD 14)

# подключаем директорию
include_directories("../src")

# Включаем тестирование
enable_testing()

# Загружаем библиотеки
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

# Создаем исполняемый файл
add_executable(test_server test.cpp)

# Линкуем (соединяем) программу с библиотекой
target_link_libraries(test_server ${GTEST_LIBRARIES} server_lib pthread)