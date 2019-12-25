# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

#set(CMAKE_C_COMPILER gcc CACHE STRING "C compiler" FORCE)
#set(CMAKE_CXX_COMPILER g++ CACHE STRING "C++ compiler" FORCE)

#项目名称, 参数值是 Demo1, 该命令表示项目的名称是 Demo1
project(TestClient)

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

set(BUILD_FLAGS "${BUILD_FLAGS} -Wno-error")
###  指定c++文件时编译选项
set(CMAKE_CXX_FLAGS ${BUILD_FLAGS})
add_definitions(-DNDEBUG)
add_definitions(-D_USE_PREC)
add_definitions(-DX64)
add_definitions(-D_REENTRANT)
add_definitions(-DLUA_USE_LINUX)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(libzmq/include)
include_directories(google)

link_directories(${PROJECT_SOURCE_DIR}/libzmq)
link_directories(${PROJECT_SOURCE_DIR}/protobuffer/lib)

set(src_lib_zeromq
  libzmq/include/zmq.h
  libzmq/include/zmq_utils.h
)

### 添加源代码
set(src_root
  main.h
  main.cpp
)


# protobuf源码文件
set(src_protobuf
  protobuffer/player.pb.cc
  protobuffer/player.pb.h
)

set(SOURCE_FILES
  ${src_lib_zeromq}
  ${src_protobuf}
  ${src_root}
)

### 指定生成的可执行文件目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build) 
# 指定生成目标
add_executable(testclient ${SOURCE_FILES})

target_link_libraries(testclient libzmq libprotobuf)