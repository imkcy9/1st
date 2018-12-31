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
class order_sender : public zmq_poller_reactor, public zmq_poll_events{
public:
    order_sender();
    virtual ~order_sender();
    
    virtual bool init();

    virtual void zmq_in_event(zmq::socket_t* socket);

    virtual void zmq_timer_event(int id_);
private:
    zmq::context_t ctx_;
    zmq::socket_t dealer_;
    
};

#endif /* ORDER_SENDER_H */

