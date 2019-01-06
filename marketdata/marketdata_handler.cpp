/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   marketdata_handler.cpp
 * Author: kecy
 * 
 * Created on 2019年1月1日, 下午7:23
 */

#include "marketdata_handler.h"
#include "base_definitions.pb.h"

marketdata_handler::marketdata_handler(zmq::context_t* ctx, zmq_poller_reactor* reactor)
: proto_message_dispatcher(reactor)
, md_impl_(ctx)
, sub_(*ctx, ZMQ_SUB)
, xpub_(*ctx, ZMQ_XPUB) {
}

marketdata_handler::~marketdata_handler() {
    md_->Disconnect();
    sub_.close();
    xpub_.close();
}

bool marketdata_handler::init() {

    sub_.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    sub_.connect("inproc://md");
    md_impl_.init();
    this->add_socket(&sub_);

    int manual = 1;
    xpub_.setsockopt(ZMQ_XPUB_MANUAL, &manual, sizeof (manual));
    xpub_.bind("tcp://*:1234");
    this->add_socket(&xpub_);

    char DLLPath2[1024] = "libCTP_Quote.so";
    ServerInfoField m_ServerInfo2 = {0};
    UserInfoField m_UserInfo = {0};

    strcpy(m_ServerInfo2.BrokerID, "9999");
    strcpy(m_ServerInfo2.Address, "tcp://180.168.146.187:10031");

    strcpy(m_UserInfo.UserID, "203328");
    strcpy(m_UserInfo.Password, "yhdr123456");

    md_ = CXApi::CreateApi(DLLPath2);
    if (!md_->Init()) {
        printf("%s\r\n", md_->GetLastError());
        md_->Disconnect();
        return false;
    }

    md_->RegisterSpi(&md_impl_);
    md_->Connect("./", &m_ServerInfo2, &m_UserInfo, 1);
    printf("md_ Connect\n");
    return true;
}

void marketdata_handler::on_recv_xpub_message(zmq::socket_t* socket) {
    zmq::message_t msg;
    socket->recv(&msg);
    char* p = (char*) msg.data();
    bool subscribe = *p == 1 ? true : false;
    std::string symbol = std::string((p + 1), msg.size() - 1);

    auto it = subscription_client_count_.find(symbol);
    if (it == subscription_client_count_.end()) {
        it = subscription_client_count_.insert(std::make_pair(symbol, 0)).first;
    }

    if (subscribe) {
        if (0 == it->second)
            md_->Subscribe(symbol.c_str(), "");
        xpub_.setsockopt(ZMQ_SUBSCRIBE, symbol.c_str(), symbol.size());
        it->second++;
        auto it = ticks_cache_.find(symbol);
        if (it != ticks_cache_.end()) {
            xpub_.send(symbol.c_str(), symbol.size(), ZMQ_SNDMORE);
            zmq::message_t seral_msg(it->second->ByteSizeLong());
            it->second->SerializeToArray(seral_msg.data(), seral_msg.size());
            xpub_.send(seral_msg);
        }
    } else {
        it->second--;
        if (0 == it->second)
            md_->Unsubscribe(symbol.c_str(), "");

        xpub_.setsockopt(ZMQ_UNSUBSCRIBE, symbol.c_str(), symbol.size());
        ZMQ_ASSERT(it->second >= 0);
    }
    ZMQ_ASSERT(!msg.more());
}

void marketdata_handler::on_recv_sub_message(zmq::socket_t* socket) {
    char topic[256] = {0};
    int size = socket->recv(topic, 256);
    zmq::message_t msg;
    socket->recv(&msg);

    FIRST::Ticker* ticker = NULL;
    auto it = ticks_cache_.find(topic);
    if (it != ticks_cache_.end()) {
        ticker = static_cast<FIRST::Ticker*>(it->second);
    } else {
        ticker = google::protobuf::Arena::Create<FIRST::Ticker>(&arena_);
        ticks_cache_.insert(std::make_pair(topic, ticker));
    }
    ticker->ParsePartialFromArray(msg.data(), msg.size());
    xpub_.send(topic, size, ZMQ_SNDMORE);
    xpub_.send(msg);
    ZMQ_ASSERT(!msg.more());

    LOG_DEBUG("TICK {}", ticker->ShortDebugString());
}
