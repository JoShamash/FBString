cmake_minimum_required(VERSION 3.27)
project(FBString)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)

# 设置 vcpkg 的工具链文件
set(CMAKE_TOOLCHAIN_FILE "D:/c++lib/vcpkg/scripts/buildsystems/vcpkg.cmake")

# 手动添加 jemalloc 的包含目录
include_directories("D:/c++lib/vcpkg/installed/x64-windows/include/jemalloc")

# 定义 USE_JEMALLOC
add_definitions(-DUSE_JEMALLOC)

# 添加可执行文件
add_executable(FBString
        FBStringCore.cpp
        FBString.cpp
        main.cpp
        Test_Proformance.cpp
)

# 链接 jemalloc 库
target_link_libraries(FBString PRIVATE "D:/c++lib/vcpkg/installed/x64-windows/lib/jemalloc.lib")
