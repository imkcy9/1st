//
// Created by chengye.ke on 2018-12-22.
//

#include <iostream>
#include <zmq.hpp>
#include "base_definitions.pb.h"
int main() {
  zmq::context_t ctx;
  zmq::socket_t socket(ctx, ZMQ_SUB);
  socket.setsockopt(ZMQ_SUBSCRIBE, "IF1901", 6);
  socket.connect("tcp://139.196.94.59:1234");


  while(true) {
    zmq::message_t msg;
    socket.recv(&msg);
    std::cout << (char*)msg.data() << std::endl;
    socket.recv(&msg);
    FIRST::Ticker tick;
    tick.ParseFromArray(msg.data(), msg.size());
    std::cout << tick.ShortDebugString() << std::endl;
    //socket.setsockopt(ZMQ_UNSUBSCRIBE, "test", 4);
    //socket.send(sst.str().c_str(), sst.str().size() + 1);
    //zmq_sleep(2);
  }

  return 0;
}