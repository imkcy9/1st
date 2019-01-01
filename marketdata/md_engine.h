/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   md_engine.h
 * Author: kecy
 *
 * Created on 2019年1月1日, 下午7:12
 */

#ifndef MD_ENGINE_H
#define MD_ENGINE_H
#include "zmq_poller_reactor.h"
#include "marketdata_handler.h"
class md_engine : public zmq_poller_reactor{
public:
    md_engine(zmq::context_t* ctx);
    virtual ~md_engine();

    virtual bool init();

private:
    marketdata_handler md_handler_;
};

#endif /* MD_ENGINE_H */

