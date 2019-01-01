/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   socket_base.h
 * Author: kecy
 *
 * Created on 2019年1月1日, 上午9:25
 */

#ifndef SOCKET_BASE_H
#define SOCKET_BASE_H

#include <google/protobuf/message.h>
#include "zmq.hpp"

class socket_base : public zmq::socket_t {
public:
    socket_base(zmq::context_t &context_, int type_);
    virtual ~socket_base();

    virtual bool send_message(std::string topic, google::protobuf::Message& message, void* routerid = NULL, size_t idsize = 0) = 0;
    
    
private:

};

#endif /* SOCKET_BASE_H */

