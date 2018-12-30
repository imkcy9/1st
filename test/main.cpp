//
// Created by chengye.ke on 2018-12-22.
//

#include <iostream>
#include <zmq.hpp>
#include "trader.pb.h"

int main() {
  zmq::context_t ctx;
  zmq::socket_t socket(ctx, ZMQ_DEALER);
  socket.setsockopt(ZMQ_IDENTITY, "test_router", 12);
  int probe = 1;
  socket.setsockopt(ZMQ_PROBE_ROUTER, &probe, sizeof(int));
  socket.connect("tcp://localhost:10003");
  int count = 0;
  std::string str("hello");
  LT::LogField logField;
  while(true) {
    ++count;
    std::stringstream sst;
    sst << str << count;
    std::cout << sst.str() << std::endl;
    //socket.send("", 0);
    socket.send("1001", 5, ZMQ_SNDMORE);

    //logField.set_level(LT::LogLevel::INFO);
    logField.set_message(sst.str());
    zmq::message_t msg(logField.ByteSizeLong());
    logField.SerializeToArray(msg.data(), msg.size());

    socket.send(msg);
    zmq_sleep(5);
  }

  return 0;
}