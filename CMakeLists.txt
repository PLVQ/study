
cmake_minimum_required(VERSION 3.9)
project(LevealDBTry)
 
 
#设定编译参数
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE "Debug")
 
#设定源码列表.cpp
# set(SOURCE_FILES ./main.cpp)
#设定所有源码列表 ：aux_source_directory(<dir> <variable>)
#比如:aux_source_directory(${CMAKE_SOURCE_DIR} DIR)  将${CMAKE_SOURCE_DIR}目录下，也就是最顶级目录下所有的.cpp文件放入DIR变量中，后面的add_executable就可以很简化
#    add_executable(hello_world ${DIR})
 aux_source_directory(${PROJECT_SOURCE_DIR}/src SOURCE_DIR)
 
#设定头文件路径
include_directories(
        "D:/boost/include"
        ${PROJECT_SOURCE_DIR}/include
)
#include_directories("路径1"  “路径2”...)
 
 
#设定链接库的路径（一般使用第三方非系统目录下的库）
link_directories(
        "D:/boost/lib"
        "D:/Windows Kits/10/Lib/10.0.18362.0/um/x64"
)
#link_directories("路径1"  “路径2”...)
 
 
#添加子目录,作用相当于进入子目录里面，展开子目录的CMakeLists.txt
#同时执行，子目录中的CMakeLists.txt一般是编译成一个库，作为一个模块
#在父目录中可以直接引用子目录生成的库
#add_subdirectory(math)
 
 
#生成动/静态库
#add_library(动/静态链接库名称  SHARED/STATIC(可选，默认STATIC)  源码列表)
#可以单独生成多个模块
 
 
#生成可执行文件
add_executable(hello_world ${SOURCE_DIR})
#比如：add_executable(hello_world    ${SOURCE_FILES})
 
 
target_link_libraries(hello_world
        libboost_atomic-mgw8-mt-d-x64-1_74
        libboost_chrono-mgw8-mt-d-x64-1_74
        libboost_container-mgw8-mt-d-x64-1_74
        libboost_context-mgw8-mt-d-x64-1_74
        libboost_contract-mgw8-mt-d-x64-1_74
        libboost_coroutine-mgw8-mt-d-x64-1_74
        libboost_date_time-mgw8-mt-d-x64-1_74
        libboost_exception-mgw8-mt-d-x64-1_74
        libboost_fiber-mgw8-mt-d-x64-1_74
        libboost_filesystem-mgw8-mt-d-x64-1_74
        libboost_graph-mgw8-mt-d-x64-1_74
        libboost_iostreams-mgw8-mt-d-x64-1_74
        libboost_locale-mgw8-mt-d-x64-1_74
        libboost_log-mgw8-mt-d-x64-1_74
        libboost_log_setup-mgw8-mt-d-x64-1_74
        libboost_math_c99-mgw8-mt-d-x64-1_74
        libboost_math_c99f-mgw8-mt-d-x64-1_74
        libboost_math_c99l-mgw8-mt-d-x64-1_74
        libboost_math_tr1-mgw8-mt-d-x64-1_74
        libboost_math_tr1f-mgw8-mt-d-x64-1_74
        libboost_math_tr1l-mgw8-mt-d-x64-1_74
        libboost_nowide-mgw8-mt-d-x64-1_74
        libboost_prg_exec_monitor-mgw8-mt-d-x64-1_74
        libboost_program_options-mgw8-mt-d-x64-1_74
        libboost_random-mgw8-mt-d-x64-1_74
        libboost_regex-mgw8-mt-d-x64-1_74
        libboost_serialization-mgw8-mt-d-x64-1_74
        libboost_stacktrace_noop-mgw8-mt-d-x64-1_74
        libboost_stacktrace_windbg-mgw8-mt-d-x64-1_74
        libboost_stacktrace_windbg_cached-mgw8-mt-d-x64-1_74
        libboost_system-mgw8-mt-d-x64-1_74
        libboost_test_exec_monitor-mgw8-mt-d-x64-1_74
        libboost_thread-mgw8-mt-d-x64-1_74
        libboost_timer-mgw8-mt-d-x64-1_74
        libboost_type_erasure-mgw8-mt-d-x64-1_74
        libboost_unit_test_framework-mgw8-mt-d-x64-1_74
        libboost_wave-mgw8-mt-d-x64-1_74
        libboost_wserialization-mgw8-mt-d-x64-1_74
        WS2_32
        WSock32
)
 
# ADD_CUSTOM_COMMAND( #执行shell命令
#           TARGET myLevelDB 
#           POST_BUILD #在目标文件myLevelDBbuild之后，执行下面的拷贝命令，还可以选择PRE_BUILD命令将会在其他依赖项执行前执行  PRE_LINK命令将会在其他依赖项执行完后执行  POST_BUILD命令将会在目标构建完后执行。
#           COMMAND cp ./myLevelDB  ../
# )
# MESSAGE("sss:", ${PROJECT_SOURCE_DIR}/src)