/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   marketdata_handler.h
 * Author: kecy
 *
 * Created on 2019年1月1日, 下午7:23
 */

#ifndef MARKETDATA_HANDLER_H
#define MARKETDATA_HANDLER_H

#include "proto_message_dispatcher.h"
#include "xapi_md_impl.h"
#include <google/protobuf/message.h>
class marketdata_handler : public proto_message_dispatcher<marketdata_handler>{
public:
    marketdata_handler(zmq::context_t* ctx, zmq_poller_reactor* reactor);

    virtual ~marketdata_handler();
    
    bool init();

    virtual void on_recv_xpub_message(zmq::socket_t* socket);

    virtual void on_recv_sub_message(zmq::socket_t* socket);

private:
    xapi_md_impl md_impl_;
    zmq::socket_t sub_;
    zmq::socket_t xpub_;
    CXApi* md_;
    google::protobuf::Arena arena_;
    std::unordered_map<std::string, google::protobuf::Message*> ticks_cache_;
    std::unordered_map<std::string, int> subscription_client_count_;
};

#endif /* MARKETDATA_HANDLER_H */

