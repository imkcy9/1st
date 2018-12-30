cmake_minimum_required(VERSION 3.6)
project(gateway)

set(CMAKE_CXX_STANDARD 11)
aux_source_directory(./ SRC_ROOT)
aux_source_directory(./proto SRC_PROTO)
aux_source_directory(./gateway SRC_GATEWAY)
aux_source_directory(./common SRC_PUBLIC)
aux_source_directory(./common/timer SRC_TIMER)

add_definitions(-DUSE_SPDLOG)

#target_include_directories()
include_directories(./proto
        ./gateway
        ./common
        ./common/timer)

link_directories(/usr/local/lib)
link_directories(./include/mac)


add_executable(gateway ./gateway/main.cpp ${SRC_GATEWAY} ${SRC_PROTO} ${SRC_PUBLIC} ${SRC_TIMER} gateway/gateway.cpp gateway/gateway.h gateway/message_handler.cpp gateway/message_handler.h)
target_link_libraries(gateway protobuf zmq tinyxml2)

add_executable(test test/main.cpp ${SRC_PROTO})
target_link_libraries(test zmq protobuf)

add_executable(pub test/pub.cpp)
target_link_libraries(pub zmq)

add_executable(sub test/sub.cpp)
target_link_libraries(sub zmq)