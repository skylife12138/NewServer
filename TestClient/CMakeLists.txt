# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

#set(CMAKE_C_COMPILER gcc CACHE STRING "C compiler" FORCE)
#set(CMAKE_CXX_COMPILER g++ CACHE STRING "C++ compiler" FORCE)

#项目名称, 参数值是 Demo1, 该命令表示项目的名称是 Demo1
project(TestClient)

include_directories(libzmq/include)

link_directories(${PROJECT_SOURCE_DIR}/libzmq)

set(src_lib_zeromq
  libzmq/include/zmq.h
  libzmq/include/zmq_utils.h
)

### 添加源代码
set(src_root
  main.h
  main.cpp
)

set(SOURCE_FILES
  ${src_lib_zeromq}
  ${src_root}
)

### 指定生成的可执行文件目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}) 
# 指定生成目标
add_executable(testclient ${SOURCE_FILES})

target_link_libraries(testclient libzmq)