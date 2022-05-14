cmake_minimum_required(VERSION 3.18)

project(colutius LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_VERSION 6)
set(REQUIRED_LIBS Core Gui Widgets Network)
set(REQUIRED_LIBS_QUALIFIED Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Network)

add_library(${PROJECT_NAME} SHARED
        Server.cpp
        Server.h
        Channel.cpp
        Channel.h
        Message.cpp
        Message.h
        )
find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})

file(COPY ${PROJECT_SOURCE_DIR}/Server.h DESTINATION ${PROJECT_BINARY_DIR}/../include)
file(COPY ${PROJECT_SOURCE_DIR}/Channel.h DESTINATION ${PROJECT_BINARY_DIR}/../include)
file(COPY ${PROJECT_SOURCE_DIR}/Message.h DESTINATION ${PROJECT_BINARY_DIR}/../include)