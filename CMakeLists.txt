cmake_minimum_required(VERSION 3.20)
project(MemoryTest VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON) 
set(CMAKE_AUTOUIC ON)


set(Qt5_DIR D:/tools/Qt/5.14.2/mingw73_64/lib/cmake/Qt5)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

include_directories(include
)

add_executable(MemoryTest
    src/main.cpp
    src/Memory.cpp
    include/TestWindow.h
    include/MainWindow.h
    include/InfoWindow.h
    include/AddFaultDialog.h
    src/Tests.cpp
    src/MainWindow.cpp
    src/TestWindow.cpp
    src/InfoWindow.cpp
    src/AddFaultDialog.cpp
)

target_link_libraries(MemoryTest PRIVATE
    Qt5::Widgets
)