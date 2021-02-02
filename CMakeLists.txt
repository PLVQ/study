cmake_minimum_required(VERSION 2.8)
project(main_project)
#设定编译参数
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE "Debug")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

if (WIN32)
        set(INCLUDE_PATH
                "D:/boost/include"
                "D:/protobuf/include"         
        )
        set(LIB_PATH
                "D:/boost/lib"
                "D:/Windows Kits/10/Lib/10.0.18362.0/um/x64"
                "D:/protobuf/lib"
        )
        add_compile_options(-std=c++11)
elseif (UNIX)
        set(INCLUDE_PATH
                "/home/admin/boost/include"
        )
        set(LIB_PATH
                "/home/admin/boost/lib"
        )
        SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")
 endif()

#设定头文件路径
include_directories(${INCLUDE_PATH})
 
 
#设定链接库的路径（一般使用第三方非系统目录下的库）
link_directories(${LIB_PATH})
 
#同时执行，子目录中的CMakeLists.txt一般是编译成一个库，作为一个模块
#在父目录中可以直接引用子目录生成的库
add_subdirectory(esayClient)
add_subdirectory(esayServer)
add_subdirectory(memoryPool)
add_subdirectory(beastServer)
add_subdirectory(test)