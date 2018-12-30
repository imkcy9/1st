//
// Created by chengye.ke on 2018-12-22.
//

#include <iostream>
#include <zmq.hpp>

int main() {
  zmq::context_t ctx;
  zmq::socket_t socket(ctx, ZMQ_PUB);
  //socket.setsockopt(ZMQ_IDENTITY, "test_router", 12);
  socket.bind("tcp://*:10003");
  int count = 0;
  std::string str("hello");
  while(true) {
    ++count;
    std::stringstream sst;
    sst << str << count;
    std::cout << sst.str() << std::endl;
    socket.send(sst.str().c_str(), sst.str().size() + 1);
    zmq_sleep(2);
  }

  return 0;
}