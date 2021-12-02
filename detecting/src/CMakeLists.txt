# минимальная версия для запуска
cmake_minimum_required(VERSION 3.16)

# имя проекта
project(detecting_service)

add_compile_options(-pthread)

set(CMAKE_PREFIX_PATH "/home/d3vyatk4/Рабочий стол/park.mail/C++/BroBand/detecting/libtorch")

# добавляем поиск файлов в директории include
include_directories("../include")

include_directories("../libtorch/include")
find_package(Torch REQUIRED)

find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

include_directories("../libopencv/include")

#add_compile_options(-g -O0 -Wextra -Wall -Wextra -Wpedantic -lgcov --coverage)
#add_link_options( -O0 -lgcov --coverage)

# библиотека с общими функциями
set(DETECT_LIB_SOURCE detecting.cpp)

# создание статической библиотеки
add_library(detect STATIC ${DETECT_LIB_SOURCE})

# создание бинарных файлов
add_executable(main_detect main.cpp ${LIB_SOURCE_HEADER})

# соединение библиотеки и основного бинарного файла
target_link_libraries(main_detect detect m pthread "${TORCH_LIBRARIES}" "${OpenCV_LIBS}")

set_property(TARGET main_detect PROPERTY CXX_STANDARD 14)

if (MSVC)
    file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
    add_custom_command(TARGET main_detect
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${TORCH_DLLS}
            $<TARGET_FILE_DIR:emain_detect>)
endif (MSVC)

