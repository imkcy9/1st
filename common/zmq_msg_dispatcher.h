/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   zmq_msg_dispatcher.h
 * Author: chengyeke
 *
 * Created on 2017年11月10日, 下午5:32
 */

#ifndef ZMQ_MSG_DISPATCHER_H
#define ZMQ_MSG_DISPATCHER_H

#include "zmq_poll_events.h"
#include <unordered_map>
typedef unsigned short ushort;
//template<class T>
class zmq_msg_dispatcher: public zmq_poll_events {
protected:
    //typedef int (T:: *msg_func) (ushort cmd, zmq::message_t& msg, zmq::message_t& rid);
    //void add_message_mapping(ushort cmd, msg_func func) {
    //    m_cmdmapping[cmd] = func;
    //}
public:
    zmq_msg_dispatcher()
    {

    };

    virtual ~zmq_msg_dispatcher() { };

    virtual void on_recv_router_message(zmq::socket_t* socket) { }

    virtual void on_recv_dealer_message(zmq::socket_t* socket) { }

    virtual void on_recv_xpub_message(zmq::socket_t* socket) { }

    virtual void on_recv_sub_message(zmq::socket_t* socket) { }

    virtual void on_recv_pull_message(zmq::socket_t* socket) { }

    virtual void on_recv_other_message(zmq::socket_t* socket) { }
private:

    void zmq_in_event(zmq::socket_t* socket)
    {
        int socket_type = -1;
        size_t size = sizeof(4);
        socket->getsockopt(ZMQ_TYPE, (void*) &socket_type, &size);
        switch (socket_type) {
        case ZMQ_ROUTER:this->on_recv_router_message(socket);
            break;

        case ZMQ_DEALER:this->on_recv_dealer_message(socket);
            break;

        case ZMQ_SUB:this->on_recv_sub_message(socket);
            break;

        case ZMQ_XPUB:this->on_recv_xpub_message(socket);
            break;

        case ZMQ_PULL:this->on_recv_pull_message(socket);
            break;
        default:this->on_recv_other_message(socket);
            assert(false);
            break;
        }

        return;

//
//        ZMQ_ASSERT(socket);
//        zmq::message_t rid;
//        socket->recv(&rid);
//        ZMQ_ASSERT(rid.more());
//
//        zmq::message_t body;
//        socket->recv(&body);
//        ZMQ_ASSERT(!body.more());
//
//        int rc = 0;
//        size_t insize = body.size();
//        size_t processed = 0;
//
//        if(insize == 0) {
//            //todo
//        }
//        const unsigned char* inpos = (const unsigned char*)body.data();
//        while(insize > 0) {
//            rc = m_decoder->decode(inpos,insize,processed);
//            ZMQ_ASSERT(processed <= insize);
//            inpos += processed;
//            insize -= processed;
//            if(rc == 0 || rc == -1)
//                break;
//
//            // todo, process message
//            typename std::unordered_map<ushort,msg_func>::iterator it = m_cmdmapping.find(m_decoder->get_cmd());
//            if(it != m_cmdmapping.end() && it->second) {
//                //next = it->second;
//                (static_cast <T *> (this)->*it->second)(m_decoder->get_cmd(), *m_decoder->msg(), rid);
//            }
//
//        }
//
//
//        return;



/*
        ZMQ_ASSERT(socket);
        zmq::message_t routingid;
        socket->recv(&routingid);
        ZMQ_ASSERT(routingid.more());
        zmq::message_t cmd;
        socket->recv(&cmd);
        ZMQ_ASSERT(cmd.more());
        ushort ucmd = *(ushort*)cmd.data();
        zmq::message_t body;
        socket->recv(&body);
        ZMQ_ASSERT(!body.more());
        typename std::unordered_map<ushort,msg_func>::iterator it = m_cmdmapping.find(ucmd);
        if(it != m_cmdmapping.end() && it->second) {
            //next = it->second;
            (static_cast <T *> (this)->*it->second)(ucmd, body, routingid);
        }*/
    };
};

#endif /* ZMQ_MSG_DISPATCHER_H */

