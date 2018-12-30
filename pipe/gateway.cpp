//
// Created by chengye.ke on 2018-12-23.
//

#include "gateway.h"
gateway::gateway(zmq::context_t *ctx)
:ctx_(ctx)
{

}
gateway::~gateway()
{
    if(message_handler_) {
        delete message_handler_;
    }
}
bool gateway::init()
{
    message_handler_ = new message_handler(this->ctx_, this);
    message_handler_->init();
    return true;
}
