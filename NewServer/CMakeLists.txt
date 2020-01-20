# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

#项目名称, 参数值是 Demo1, 该命令表示项目的名称是 Demo1
project(NewServer)

set(CMAKE_BUILD_TYPE Release CACHE STRING "set build type to Release")

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
set(BUILD_FLAGS "${BUILD_FLAGS} -gdwarf-2")
set(BUILD_FLAGS "${BUILD_FLAGS} -Wno-unknown-pragmas")


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
#include_directories(${PROJECT_SOURCE_DIR}/gflags/gflags)
#include_directories(${PROJECT_SOURCE_DIR}/gflags/gflags_win/gflags)
include_directories(protobuffer)#protobuffer库头文件路径
include_directories(libevent/include)#libevent库头文件路径
include_directories(tcmalloc/inlcude)#tcmalloc库头文件路径
include_directories(json/inlcude)#jsoncpp库头文件路径
include_directories(python3.7/inlcude)#python3.7库头文件路径


### 动态链接库或静态链接库的搜索路径
link_directories(${PROJECT_SOURCE_DIR}/gflags)
link_directories(${PROJECT_SOURCE_DIR}/libzmq)
link_directories(${PROJECT_SOURCE_DIR}/protobuffer/lib)
link_directories(${PROJECT_SOURCE_DIR}/libevent)
link_directories(${PROJECT_SOURCE_DIR}/tcmalloc)
link_directories(${PROJECT_SOURCE_DIR}/json)
link_directories(${PROJECT_SOURCE_DIR}/python3.7)

if(WIN32)
    set(LIBS ${LIBS} ws2_32)#wsock32)
    set(LIBS ${LIBS} winmm)
endif()

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
  Common/MemoryPool.h
  Common/MemoryPool.cpp
  Common/ObjectFactory.h
  Common/ObjectFactory.cpp
  Common/Generate.h
  Common/GTimer.h
  Common/GTimer.cpp
  Common/List.h
  Common/Lock.h
  Common/Portable.h
  Common/Portable.cpp
  Common/Thread.h
  Common/Thread.cpp
  Common/Log.h 
  Common/Log.cpp 
  Common/GPython.h 
  Common/GPython.cpp 
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
  Logic/CPlayerRecv.cpp
  Logic/CPlayer.h
  Logic/CPlayer.cpp
  Logic/CJsonLog.h 
  Logic/CJsonLog.cpp 
)

set(src_net
  Net/IoThread.h
  Net/IoThread.cpp
  Net/MsgFilter.h 
  Net/MsgFilter.cpp 
  Net/NetPack.h 
  Net/NetPack.cpp 
  Net/NetWorkManger.h 
  Net/NetWorkManger.cpp 
  Net/CClient.h
  Net/CClient.cpp 
)

# protobuf源码文件
set(src_protobuf
  protobuffer/player.pb.cc
  protobuffer/player.pb.h
)



set(SOURCE_FILES
  ${src_lib_zeromq}
  ${src_lib_gflags}
  ${src_common}
  ${src_net}
  ${src_Logic}
  ${src_root}
  ${src_protobuf}
)

### 指定生成的可执行文件目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build) 
# 指定生成目标
add_executable(newserver ${SOURCE_FILES})
#add_gcc_precompiled_header(newserver Prec.h)
#use_gcc_precompiled_header("${SOURCE_FILES}")

target_link_libraries(newserver ${LIBS} libzmq libprotobuf libevent libjsoncpp python37 libtcmalloc_minimal)