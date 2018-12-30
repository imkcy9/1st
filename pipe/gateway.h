//
// Created by chengye.ke on 2018-12-23.
//

#ifndef GATEWAY_GATEWAY_H
#define GATEWAY_GATEWAY_H

#include "zmq_poller_reactor.h"
#include "proto_message_dispatcher.h"
#include "message_handler.h"
class gateway: public zmq_poller_reactor {
public:
    gateway(zmq::context_t *ctx);
    virtual ~gateway();

    bool init() override;

private:
    zmq::context_t* ctx_;
    message_handler* message_handler_;
};

#endif //GATEWAY_GATEWAY_H
