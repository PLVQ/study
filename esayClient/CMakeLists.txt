cmake_minimum_required(VERSION 2.8)
project(esayClient)
set(CMAKE_BUILD_TYPE "release") 
#设定源码列表.cpp
# set(SOURCE_FILES ./main.cpp)
#设定所有源码列表 ：aux_source_directory(<dir> <variable>)
#比如:aux_source_directory(${CMAKE_SOURCE_DIR} DIR)  将${CMAKE_SOURCE_DIR}目录下，也就是最顶级目录下所有的.cpp文件放入DIR变量中，后面的add_executable就可以很简化
#    add_executable(hello_world ${DIR})
#设定头文件路径
include_directories(
        ${PROJECT_SOURCE_DIR}/include
) 

aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC)

add_executable(client ${SRC})
 
if (WIN32)
        target_link_libraries(client 
                WS2_32
                WSock32
        )
else (UNIX)
endif ()
