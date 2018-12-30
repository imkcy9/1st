//
// Created by chengye.ke on 2018-12-23.
//

#ifndef GATEWAY_MESSAGE_HANDLER_H
#define GATEWAY_MESSAGE_HANDLER_H

#include "proto_message_dispatcher.h"
#include "xapi_impl.h"
class message_handler : public proto_message_dispatcher<message_handler>{
public:
    message_handler(zmq::context_t* ctx, zmq_poller_reactor* reactor);
    virtual ~message_handler();

    void zmq_timer_event(int id_) override;
    bool init();

    void on_req_login(zmq::message_t& rid, google::protobuf::Message& body);

private:
    zmq::socket_t router_;
    zmq::socket_t pull_;
    zmq_poller_reactor* reactor_;
    xapi_impl* spi_;
    CXApi* trader_;
    CXApi* md_;
};

#endif //GATEWAY_MESSAGE_HANDLER_H
