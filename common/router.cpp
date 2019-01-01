/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   router.cpp
 * Author: kecy
 * 
 * Created on 2019年1月1日, 上午9:32
 */

#include "router.h"

router::router(zmq::context_t &context_) : socket_base(context_, ZMQ_ROUTER){
}

router::~router() {
}

bool router::send_message(std::string topic, google::protobuf::Message& message, void* routerid, size_t idsize) {
    try {
        if(!routerid) {
            throw "routerid is null.";
            return false;
        }
        this->send(routerid, idsize, ZMQ_SNDMORE);
        this->send(topic.c_str(), topic.size(), ZMQ_SNDMORE);
        zmq::message_t msg(message.ByteSizeLong());
        message.SerializePartialToArray(msg.data(), msg.size());
        this->send(msg);
    } catch (zmq::error_t& e) {
        return false;
    }
    return true;
}


