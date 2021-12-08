cmake_minimum_required(VERSION 3.8)

project(Client VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-g -O0 -Wextra -Wall -Wextra -Wpedantic -lgcov --coverage)
add_link_options(-O0 -lgcov --coverage)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread)
include_directories(${Boost_INCLUDE_DIRS})

set(PROJECT_MAIN src/main.cpp)
set(PROJECT_SOURCES
        src/MainWindow.cpp
        src/Client.cpp
        src/Commands.cpp
        src/MainWindow.h
        src/Structs.h
        src/Client.h
        src/Commands.h
        src/handlers.h
        src/handlers.cpp
        src/MainWindow.ui)

qt5_add_resources(PROJECT_SOURCES src/res.qrc)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Client MANUAL_FINALIZATION ${PROJECT_MAIN} ${PROJECT_SOURCES})
else ()
    add_executable(Client ${PROJECT_MAIN} ${PROJECT_SOURCES})
endif ()

target_link_libraries(Client PRIVATE ${Boost_LIBRARIES} Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(Client PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR})

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Client)
endif ()

#enable_testing()
#find_package(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIRS})

#add_executable(tests test/mainTest.cpp ${PROJECT_SOURCES})
#target_link_libraries(tests ${GTEST_LIBRARIES} ${Boost_LIBRARIES} Qt${QT_VERSION_MAJOR}::Widgets)
#set_target_properties(tests PROPERTIES
 #       MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
  #      MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
   #     MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR})

