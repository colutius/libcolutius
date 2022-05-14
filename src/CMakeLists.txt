cmake_minimum_required(VERSION 3.18)

project(colutius LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Gui Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Network)

add_library(${PROJECT_NAME} SHARED
        Server.cpp
        Server.h
        Channel.cpp
        Channel.h
        Message.cpp
        Message.h
        )

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core)
file(COPY ${PROJECT_SOURCE_DIR}/Server.h DESTINATION ${PROJECT_BINARY_DIR}/../include)
file(COPY ${PROJECT_SOURCE_DIR}/Channel.h DESTINATION ${PROJECT_BINARY_DIR}/../include)
file(COPY ${PROJECT_SOURCE_DIR}/Message.h DESTINATION ${PROJECT_BINARY_DIR}/../include)