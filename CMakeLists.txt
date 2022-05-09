cmake_minimum_required(VERSION 3.18)
project(libcolutius LANGUAGES CXX)
#设置可执行文件与库文件输出位置
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
#添加子目录项目
add_subdirectory(src/Message)
add_subdirectory(src/Demo)