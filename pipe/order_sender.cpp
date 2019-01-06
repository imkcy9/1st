/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   order_sender.cpp
 * Author: kecy
 * 
 * Created on 2018年12月31日, 下午4:11
 */

#include "order_sender.h"
#include "first_definitions.pb.h"
#include "log.h"
#include "google/protobuf/util/json_util.h"

order_sender::order_sender()
: proto_message_dispatcher(this)
, router_(ctx_, ZMQ_ROUTER)
{
    add_router_msg_mapping<FIRST::OrderFeed>(&order_sender::on_order_feed);
    add_router_msg_mapping<FIRST::TradeFeed>(&order_sender::on_trade_feed);
    add_router_msg_mapping<FIRST::CreateOrderResponse>(&order_sender::on_create_order_response);
    add_router_msg_mapping<FIRST::CancelOrderResponse>(&order_sender::on_cancel_order_response);
}

order_sender::~order_sender() {
}

bool order_sender::init() {
    int mandatory = 1;
    router_.setsockopt(ZMQ_ROUTER_MANDATORY,&mandatory,sizeof(mandatory));
    router_.setsockopt(ZMQ_CONNECT_RID,"PIPE",4);
    router_.connect("tcp://localhost:10003");
    //router_.connect("ipc:///home/kcy/1st/bin/ipc");
    proto_message_dispatcher::add_socket(&router_);
    timers_add(1, 5000, this);
    return true;
}

//void order_sender::zmq_in_event(zmq::socket_t* socket) {
//    zmq::message_t rid;
//    router_.recv(&rid);
//    zmq::message_t type;
//    router_.recv(&type);
//    zmq::message_t msg;
//    router_.recv(&msg);
//    ZMQ_ASSERT(!msg.more());
//    
//    FIRST::OrderFeed orderFeed;
//    orderFeed.ParsePartialFromArray(msg.data(), msg.size());
//    google::protobuf::util::JsonOptions op;
//    op.always_print_primitive_fields = true;
//    op.always_print_enums_as_ints = false;
//    op.preserve_proto_field_names = false;
//    std::string output;
//    google::protobuf::util::MessageToJsonString(orderFeed, &output, op);
//    LOG_DEBUG("{} : {}", output);
//}

void order_sender::zmq_timer_event(int id_) {
//    OrderIDType Out = {1};
//        OrderField* pOrder = new OrderField();
//        memset(pOrder, 0, sizeof (OrderField));
//        strcpy(pOrder->InstrumentID, "SR901");
//        pOrder->OpenClose = OpenCloseType::OpenCloseType_Open;
//        pOrder->HedgeFlag = HedgeFlagType::HedgeFlagType_Speculation;
//        pOrder->Price = 4800;
//        pOrder->Qty = 1;
//        pOrder->Type = OrderType::OrderType_Limit;
//        pOrder->Side = OrderSide::OrderSide_Buy;
//        pOrder->ReserveInt32 = 11;
    FIRST::CreateOrderRequest orderRequest;
    orderRequest.set_request_id(1);
    orderRequest.mutable_order()->set_instrument_id("SR901");
    orderRequest.mutable_order()->set_open_close(FIRST::OpenCloseType::OPEN);
    orderRequest.mutable_order()->set_client_id("kcy");
    orderRequest.mutable_order()->set_account_id("chengye.ke");
    orderRequest.mutable_order()->set_hedge_flag(FIRST::HedgeFlagType::SPECULATION);
    orderRequest.mutable_order()->set_price(5000);
    orderRequest.mutable_order()->set_qty(1);
    orderRequest.mutable_order()->set_type(FIRST::OrderType::LIMIT);
    orderRequest.mutable_order()->set_size(FIRST::OrderSide::BUY);
    orderRequest.mutable_order()->set_client_order_id(1);
    
    zmq::message_t req(orderRequest.ByteSizeLong());
    orderRequest.SerializePartialToArray(req.data(), req.size());
    try {
    router_.send("PIPE",4,ZMQ_SNDMORE);
    router_.send(orderRequest.GetTypeName().c_str(), orderRequest.GetTypeName().size(), ZMQ_SNDMORE);
    router_.send(req);
    } catch (zmq::error_t& e) {
        LOG_ERROR("{}", e.what());
    }
    timers_cancel(id_, this);
}

void order_sender::on_order_feed(zmq::socket_t* router, zmq::message_t& rid, google::protobuf::Message& body) {
    LOG_INFO("on_order_feed {}", body.ShortDebugString());
    
//    FIRST::OrderFeed& feed = (FIRST::OrderFeed&)body;
//    
//    if(feed.order().exec_type() == FIRST::ExecType::EXECTYPE_CANCELL_REJECT)
//        return;
//    int clOrdid = feed.order().client_order_id();
//    FIRST::CancelOrderRequest req;
//    req.set_request_id(1);
//    req.set_client_order_id(clOrdid);
//    req.mutable_order()->set_id(feed.order().id());
//    
//    zmq::message_t msg(req.ByteSizeLong());
//    req.SerializePartialToArray(msg.data(), msg.size());
//    
//    router->send(rid, ZMQ_SNDMORE);
//    router->send(req.GetTypeName().c_str(), req.GetTypeName().size(), ZMQ_SNDMORE);
//    router->send(msg);
}

void order_sender::on_trade_feed(zmq::socket_t* router, zmq::message_t& rid, google::protobuf::Message& body) {
    LOG_INFO("on_trade_feed {}", body.ShortDebugString());
}

void order_sender::on_create_order_response(zmq::socket_t* router, zmq::message_t& rid, google::protobuf::Message& body) {
    LOG_INFO("on_create_order_response {}", body.ShortDebugString());
}

void order_sender::on_cancel_order_response(zmq::socket_t* router, zmq::message_t& rid, google::protobuf::Message& body) {
    LOG_INFO("on_cancel_order_response {}", body.ShortDebugString());
    
}

