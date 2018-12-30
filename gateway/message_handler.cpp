//
// Created by chengye.ke on 2018-12-23.
//

#include <trader.pb.h>
#include "message_handler.h"
message_handler::message_handler(zmq::context_t* ctx, zmq_poller_reactor* reactor)
        :proto_message_dispatcher(reactor)
        ,router_(*ctx, ZMQ_ROUTER)
        ,reactor_(reactor){

}

message_handler::~message_handler()
{
    router_.close();
}
void message_handler::zmq_timer_event(int id_)
{
    LOG_DEBUG("on timer {}", id_);
}
bool message_handler::init()
{
    add_router_msg_mapping<LT::LogField>("1001", &message_handler::on_req_login);
    //router_.setsockopt(ZMQ_IDENTITY, "SUB", 3);
    router_.bind("tcp://*:10003");
    this->add_socket(&router_);

    this->add_timer(1,2000);
    return true;
}
void message_handler::on_req_login(zmq::message_t& rid, google::protobuf::Message& body)
{
    LOG_DEBUG("on_req_lon {} {}", (char*)rid.data(), body.DebugString());
}
