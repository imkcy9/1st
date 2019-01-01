/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   order_sender.h
 * Author: kecy
 *
 * Created on 2018年12月31日, 下午4:11
 */

#ifndef ORDER_SENDER_H
#define ORDER_SENDER_H
#include "zmq_poller_reactor.h"
#include "proto_message_dispatcher.h"
class order_sender : public proto_message_dispatcher<order_sender>, public zmq_poller_reactor{
public:
    order_sender();
    virtual ~order_sender();
    
    virtual bool init();

    virtual void zmq_timer_event(int id_);
    
    void on_order_feed(zmq::socket_t* router,zmq::message_t& rid, google::protobuf::Message& body);
    void on_trade_feed(zmq::socket_t* router,zmq::message_t& rid, google::protobuf::Message& body);
    void on_create_order_response(zmq::socket_t* router,zmq::message_t& rid, google::protobuf::Message& body);
private:
    zmq::context_t ctx_;
    zmq::socket_t router_;
    
};

#endif /* ORDER_SENDER_H */

