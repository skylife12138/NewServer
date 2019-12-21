# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

#项目名称, 参数值是 Demo1, 该命令表示项目的名称是 Demo1
project(NewServer)

# 为方便调试，禁用优化，有性能瓶颈时可考虑开启
set(BUILD_FLAGS "-m64 -pipe -O0 -ggdb3 -std=c++11 -ffloat-store")
# VC中已经使用/fp:fast启用了优化
set(BUILD_FLAGS "${BUILD_FLAGS} -ffast-math")
# 
set(BUILD_FLAGS "${BUILD_FLAGS} -Wno-invalid-offsetof")
# 
set(BUILD_FLAGS "${BUILD_FLAGS} -Wno-write-strings")
# 允许使用NULL宏而非nullptr
set(BUILD_FLAGS "${BUILD_FLAGS} -Wno-conversion-null")
# 运行不同枚举混用
set(BUILD_FLAGS "${BUILD_FLAGS} -Wno-enum-compare")
# 启用预编译头警告
set(BUILD_FLAGS "${BUILD_FLAGS} -Winvalid-pch")

###  指定c++文件时编译选项
set(CMAKE_CXX_FLAGS ${BUILD_FLAGS})
add_definitions(-DNDEBUG)
add_definitions(-D_USE_PREC)
add_definitions(-DX64)
add_definitions(-D_REENTRANT)
add_definitions(-DLUA_USE_LINUX)

### 指定库头文件的搜索路径
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(libzmq/include)
include_directories(gflags/gflags)
include_directories(gflags/gflags_win/gflags)



### 动态链接库或静态链接库的搜索路径
link_directories(${PROJECT_SOURCE_DIR}/gflags)
link_directories(${PROJECT_SOURCE_DIR}/libzmq)

### 添加库头文件
set(src_lib_zeromq
  libzmq/include/zmq.h
  libzmq/include/zmq_utils.h
)

if(WIN32)
set(src_lib_gflags
  
    gflags/gflags_win/gflags/defines.h
    gflags/gflags_win/gflags/gflags.h
    gflags/gflags_win/gflags/gflags_completions.h
    gflags/gflags_win/gflags/gflags_declare.h
    gflags/gflags_win/gflags/gflags_gflags.h
)
else()
set(src_lib_gflags
    gflags/gflags/defines.h
    gflags/gflags/gflags.h
    gflags/gflags/gflags_completions.h
    gflags/gflags/gflags_declare.h
    gflags/gflags/gflags_gflags.h

)
endif()

### 添加源代码
set(src_root
  main.h
  main.cpp
  netapi.h
  netapi.cpp
  Prec.h
)

set(src_common
  Common/Common.h
  Common/Common.cpp
  Common/DynamicPool.h
  Common/DynamicPool.cpp
  Common/Generate.h
  Common/GTimer.h
  Common/GTimer.cpp
  Common/List.h
  Common/Lock.h
  Common/Portable.h
  Common/Thread.h
  Common/Thread.cpp
)

set(src_Logic
  Logic/Define.h
  Logic/Person.h
  Logic/Person.cpp
  Logic/ProjectManger.h
  Logic/ProjectManger.cpp
  Logic/Reflector.h
  Logic/Reflector.cpp
  Logic/SingleTon.h
)

set(src_net
  Net/IoThread.h
  Net/IoThread.cpp
)

set(SOURCE_FILES
  ${src_lib_zeromq}
  ${src_lib_gflags}
  ${src_common}
  ${src_net}
  ${src_Logic}
  ${src_root}
)

### 指定生成的可执行文件目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}) 
# 指定生成目标
add_executable(newserver ${SOURCE_FILES})
#add_gcc_precompiled_header(newserver Prec.h)
#use_gcc_precompiled_header("${SOURCE_FILES}")

target_link_libraries(newserver libzmq)
