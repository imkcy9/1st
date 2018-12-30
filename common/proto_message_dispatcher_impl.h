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
:reactor_(reactor)
{
    //add_message_mapping(1005, &proto_message_dispatcher::on_recv_tick);
    //add_message_mapping(1004, &proto_message_dispatcher::on_recv_hb);
}

template <class T>
proto_message_dispatcher<T>::~proto_message_dispatcher()
{

}

template <class T>
void
proto_message_dispatcher<T>::on_recv_router_message(zmq::socket_t* socket)
{

    zmq::message_t rid;
    socket->recv(&rid);
    ZMQ_ASSERT(rid.more());


    zmq::message_t msg_type;
    socket->recv(&msg_type);
    std::string type((char*)msg_type.data());

    zmq::message_t msg;
    socket->recv(&msg);
    LOG_INFO("{}", (char*) rid.data());

    auto it = router_funcs_.find(type);
    LOG_INFO("{}", (char*) rid.data());
    if(it != router_funcs_.end()) {
        std::pair<google::protobuf::Message*, router_msg_func>& p = it->second;
        p.first->ParseFromArray(msg.data(), msg.size());
        (static_cast <T *> (this)->*p.second)(rid, *p.first);
    }

    while(msg.more()) {
        socket->recv(&msg);
    }

    LOG_INFO("{} size:{}", (char*) msg.data(), msg.size());



    //socket->send("test111",8);
    //socket->send("test123",8);
    //socket->send("nono123",8);
    //int c = ((char*)msg.data())[0];
    //LOG_INFO("{}", c);
    //LOG_INFO("{}", (char*)msg.data() + 1);
    ZMQ_ASSERT(!msg.more());
}

template <class T>
void
proto_message_dispatcher<T>::on_recv_dealer_message(zmq::socket_t* socket)
{

}

template <class T>
void
proto_message_dispatcher<T>::on_recv_xpub_message(zmq::socket_t* socket)
{

}

template <class T>
void
proto_message_dispatcher<T>::on_recv_sub_message(zmq::socket_t* socket)
{

}

template <class T>
void
proto_message_dispatcher<T>::on_recv_pull_message(zmq::socket_t* socket)
{

}

template<class T>
void proto_message_dispatcher<T>::on_recv_other_message(zmq::socket_t* socket)
{

}

template <class T>
template <class M>
void proto_message_dispatcher<T>::add_dealer_msg_mapping(const char* message_tyep, dealer_msg_func func) {

}

template <class T>
template <class M>
void proto_message_dispatcher<T>::add_router_msg_mapping(const char* message_tyep, router_msg_func func) {
    std::string type = message_tyep;
    router_funcs_[type] = std::make_pair(new M(), func);
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