cmake_minimum_required(VERSION 3.18)
project(colutius-demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT_VERSION 6)
set(REQUIRED_LIBS Core Gui Widgets Network)
set(REQUIRED_LIBS_QUALIFIED
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
        #我的动态库
        colutius
        )

#设置头文件路径
include_directories(
        ${PROJECT_SOURCE_DIR}/../src
)

add_executable(${PROJECT_NAME}
        main.cpp
        )

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})