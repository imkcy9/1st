cmake_minimum_required(VERSION 2.6)
project(1st)

set(CMAKE_CXX_STANDARD 11)
aux_source_directory(./ SRC_ROOT)
aux_source_directory(./proto SRC_PROTO)
aux_source_directory(./pipe SRC_PIPE)
aux_source_directory(./marketdata SRC_MARKETDATA)
aux_source_directory(./common SRC_PUBLIC)
aux_source_directory(./common/timer SRC_TIMER)
aux_source_directory(./common/tinyxml2 SRC_TINYXML2)

add_definitions(-std=c++11 -g -fpermissive -DUSE_SPDLOG -DZMQ_BUILD_DRAFT_API)
SET(EXECUTABLE_OUTPUT_PATH ./bin)
#target_include_directories()
include_directories(./proto
        ./gateway
        ./common
        ./common/timer
        ./include
        ./XAPI2/include)

link_directories(/usr/local/lib)
link_directories(./lib/centos)
link_directories(./XAPI2/lib)
link_directories(./XAPI2/bin)


add_executable(pipe ${SRC_PIPE} ${SRC_PROTO} ${SRC_PUBLIC} ${SRC_TIMER} ${SRC_TINYXML2})
target_link_libraries(pipe pthread protobuf zmq tinyxml2 -lXAPI_CPP)

add_executable(marketdata ${SRC_MARKETDATA} ${SRC_PROTO} ${SRC_PUBLIC} ${SRC_TIMER} ${SRC_TINYXML2})
target_link_libraries(marketdata pthread protobuf zmq tinyxml2 -lXAPI_CPP)

add_executable(test test/main.cpp ${SRC_PROTO})
target_link_libraries(test zmq protobuf)

add_executable(pub test/pub.cpp)
target_link_libraries(pub zmq)

add_executable(sub test/sub.cpp ${SRC_PROTO} ${SRC_PUBLIC} ${SRC_TIMER} ${SRC_TINYXML2})
target_link_libraries(sub pthread protobuf zmq tinyxml2)