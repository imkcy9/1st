/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   router.h
 * Author: kecy
 *
 * Created on 2019年1月1日, 上午9:32
 */

#ifndef ROUTER_H
#define ROUTER_H

#include "socket_base.h"
class router : public socket_base{
public:
    router(zmq::context_t &context_);
    virtual ~router();

    virtual bool send_message(std::string topic, google::protobuf::Message& message, void* routerid, size_t idsize);

private:

};

#endif /* ROUTER_H */

