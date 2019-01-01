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

    //------------------request from client----------------------------
    void on_req_login(zmq::socket_t* router,zmq::message_t& rid, google::protobuf::Message& body);
    void on_create_order_request(zmq::socket_t* router, zmq::message_t& rid, google::protobuf::Message& body);
    void cancel_order_request(zmq::socket_t* router, zmq::message_t& rid, google::protobuf::Message& body);
    //------------------response from xapi----------------------------
    void on_resp_instrument(google::protobuf::Message& body);
    void on_order_feed(google::protobuf::Message& body);
    void on_trade_feed(google::protobuf::Message& body);
private:
    zmq::socket_t router_;
    zmq::socket_t push_;
    zmq::socket_t pull_;
    zmq_poller_reactor* reactor_;
    xapi_impl* spi_;
    CXApi* trader_;
    CXApi* md_;
    
    google::protobuf::Arena arena_;
    std::unordered_map<std::string, std::string> router_mapping_;
    
    //Cause' a message queue thread, no need to user atomic operation.
};

#endif //GATEWAY_MESSAGE_HANDLER_H
