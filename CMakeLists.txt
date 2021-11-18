cmake_minimum_required(VERSION 3.5)

project(Client VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-g -O0 -Wextra -Wall -Wextra -Wpedantic -lgcov --coverage)
add_link_options(-O0 -lgcov --coverage)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread)
include_directories(${Boost_INCLUDE_DIRS})

set(PROJECT_SOURCES
        src/main.cpp
        src/MainWindow.cpp
        src/User.cpp
        src/Chat.cpp
        src/Message.cpp
        src/Client.cpp
        src/Commands.cpp
        src/MainWindow.h
        src/User.h
        src/Chat.h
        src/Message.h
        src/Client.h
        src/Commands.h
        src//MainWindow.ui)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Client MANUAL_FINALIZATION ${PROJECT_SOURCES})
else ()
    add_executable(Client ${PROJECT_SOURCES})
endif ()

target_link_libraries(Client PRIVATE ${Boost_LIBRARIES} Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(Client PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        )

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Client)
endif ()


