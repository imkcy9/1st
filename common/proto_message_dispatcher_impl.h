/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   proto_message_dispatcher.cpp
 * Author: chengyeke
 * 
 * Created on 2017年9月20日, 下午4:16
 */

#include <zmq.h>

#include "proto_message_dispatcher.h"
#include "log.h"
#include <iostream>

template <class T>
proto_message_dispatcher<T>::proto_message_dispatcher(zmq_poller_reactor* reactor)
: reactor_(reactor) {
    //add_message_mapping(1005, &proto_message_dispatcher::on_recv_tick);
    //add_message_mapping(1004, &proto_message_dispatcher::on_recv_hb);
}

template <class T>
proto_message_dispatcher<T>::~proto_message_dispatcher() {

}

template <class T>
void
proto_message_dispatcher<T>::on_recv_router_message(zmq::socket_t* socket) {
    zmq::message_t rid;
    socket->recv(&rid);
    ZMQ_ASSERT(rid.more());

    zmq::message_t msg_type;
    socket->recv(&msg_type);
    std::string type((char*) msg_type.data(), msg_type.size());

    zmq::message_t msg;
    socket->recv(&msg);

    auto it = router_funcs_.find(type);
    if (it != router_funcs_.end()) {
        std::pair<google::protobuf::Message*, router_msg_func>& p = it->second;
        p.first->ParseFromArray(msg.data(), msg.size());
        (static_cast<T *> (this)->*p.second)(socket, rid, *p.first);
    } else {
        LOG_ERROR("Unknown router prototype message: {}", type);
    }

    while (msg.more()) {
        socket->recv(&msg);
    }
    ZMQ_ASSERT(!msg.more());
}

template <class T>
void
proto_message_dispatcher<T>::on_recv_dealer_message(zmq::socket_t* socket) {

}

template <class T>
void
proto_message_dispatcher<T>::on_recv_xpub_message(zmq::socket_t* socket) {

}

template <class T>
void
proto_message_dispatcher<T>::on_recv_sub_message(zmq::socket_t* socket) {

}

template <class T>
void
proto_message_dispatcher<T>::on_recv_pull_message(zmq::socket_t* socket) {
    zmq::message_t msg_type;
    socket->recv(&msg_type);
    std::string type((char*) msg_type.data(),msg_type.size());

    zmq::message_t msg;
    socket->recv(&msg);

    auto it = pull_funcs_.find(type);
    if (it != pull_funcs_.end()) {
        std::pair<google::protobuf::Message*, pull_msg_func>& p = it->second;
        p.first->ParseFromArray(msg.data(), msg.size());
        (static_cast<T *> (this)->*p.second)(*p.first);
    } else {
        LOG_ERROR("Unknown pull prototype message: {}", type);
    }

    while (msg.more()) {
        socket->recv(&msg);
    }
    ZMQ_ASSERT(!msg.more());
}

template<class T>
void proto_message_dispatcher<T>::on_recv_other_message(zmq::socket_t* socket) {

}

template <class T>
template <class PROTOBUF_MESSAGE_TYPE>
void proto_message_dispatcher<T>::add_dealer_msg_mapping(dealer_msg_func func) {

}

template <class T>
template <class PROTOBUF_MESSAGE_TYPE>
void proto_message_dispatcher<T>::add_router_msg_mapping(router_msg_func func) {
    router_funcs_[PROTOBUF_MESSAGE_TYPE::default_instance().GetTypeName()] = std::make_pair(new PROTOBUF_MESSAGE_TYPE(), func);
}

template <class T>
template <class PROTOBUF_MESSAGE_TYPE>
void proto_message_dispatcher<T>::add_pull_msg_mapping(pull_msg_func func) {
    pull_funcs_[PROTOBUF_MESSAGE_TYPE::default_instance().GetTypeName()] = std::make_pair(new PROTOBUF_MESSAGE_TYPE(), func);
}

template <class T>
void proto_message_dispatcher<T>::add_socket(zmq::socket_t* socket) {
    this->reactor_->add_socket(socket, this);
}

template <class T>
void proto_message_dispatcher<T>::add_timer(int timer_id, size_t interval) {
    this->reactor_->timers_add(timer_id, interval, this);
}

template <class T>
void proto_message_dispatcher<T>::cancell_timer(int timer_id) {
    this->reactor_->timers_cancel(timer_id, this);
}