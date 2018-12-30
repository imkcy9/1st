//
// Created by chengye.ke on 2018-12-22.
//

#include <iostream>
#include <zmq.hpp>

int main() {
  zmq::context_t ctx;
  zmq::socket_t socket(ctx, ZMQ_SUB);
  socket.setsockopt(ZMQ_SUBSCRIBE, "test", 4);
  socket.connect("tcp://localhost:10003");

  zmq_sleep(2);
  while(true) {
    zmq::message_t msg;
    socket.recv(&msg);
    std::cout << (char*)msg.data() << std::endl;
    socket.setsockopt(ZMQ_UNSUBSCRIBE, "test", 4);
    //socket.send(sst.str().c_str(), sst.str().size() + 1);
    //zmq_sleep(2);
  }

  return 0;
}