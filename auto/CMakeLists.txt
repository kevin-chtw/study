# 设置 CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(auto)

# 启用 C++17 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加可执行文件
# 使用 glob 匹配当前路径下的所有 .cpp 文件
file(GLOB SOURCES "*.cpp")

# 添加目标可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

