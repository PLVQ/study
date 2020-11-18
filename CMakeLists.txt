
cmake_minimum_required(VERSION 3.9)
project(study)
#设定编译参数
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE "Debug")
 
#设定头文件路径
include_directories(
        "D:/boost/include"
        ${PROJECT_SOURCE_DIR}/include
        "D:/protobuf/include"
)
 
 
#设定链接库的路径（一般使用第三方非系统目录下的库）
link_directories(
        "D:/boost/lib"
        "D:/Windows Kits/10/Lib/10.0.18362.0/um/x64"
        "D:/protobuf/lib"
)
 
#同时执行，子目录中的CMakeLists.txt一般是编译成一个库，作为一个模块
#在父目录中可以直接引用子目录生成的库
add_subdirectory(esayClient)
add_subdirectory(esayServer)