/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   proto_message_dispatcher.h
 * Author: chengyeke
 *
 * Created on 2017年9月20日, 下午4:16
 */

#ifndef PROTO_MESSAGE_DISPATCHER_H
#define PROTO_MESSAGE_DISPATCHER_H
#include "zmq_poller_reactor.h"
#include "zmq_msg_dispatcher.h"
#include <google/protobuf/message.h>

template<class T>
class proto_message_dispatcher: public zmq_msg_dispatcher {
public:
    typedef void (T::*dealer_msg_func)(void* data, size_t size);
    typedef void (T::*router_msg_func)(zmq::message_t& router_id, google::protobuf::Message& body);

    proto_message_dispatcher(zmq_poller_reactor* reactor);
    virtual ~proto_message_dispatcher();
    void
    on_recv_router_message(zmq::socket_t* socket) override;
    void
    on_recv_dealer_message(zmq::socket_t* socket) override;
    void
    on_recv_xpub_message(zmq::socket_t* socket) override;
    void
    on_recv_sub_message(zmq::socket_t* socket) override;
    void
    on_recv_pull_message(zmq::socket_t* socket) override;
    void on_recv_other_message(zmq::socket_t* socket) override;

    void add_socket(zmq::socket_t* socket);
    void add_timer(int timer_id, size_t interval);
protected:

    template<class M>
    void add_dealer_msg_mapping(const char* message_tyep, dealer_msg_func func);
    template<class M>
    void add_router_msg_mapping(const char* message_tyep, router_msg_func func);
private:
    zmq_poller_reactor* reactor_;
    std::unordered_map<std::string, std::pair<google::protobuf::Message*, router_msg_func>> router_funcs_;
};

#include "proto_message_dispatcher_impl.h"
#endif /* PROTO_MESSAGE_DISPATCHER_H */

