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
: dealer_(ctx_, ZMQ_DEALER)
{
}

order_sender::~order_sender() {
}

bool order_sender::init() {
    dealer_.connect("tcp://localhost:10003");
    this->add_socket(&dealer_, this);
    timers_add(1, 5000, this);
    return true;
}

void order_sender::zmq_in_event(zmq::socket_t* socket) {
    zmq::message_t type;
    dealer_.recv(&type);
    zmq::message_t msg;
    dealer_.recv(&msg);
    ZMQ_ASSERT(!msg.more());
    
    FIRST::OrderFeed orderFeed;
    orderFeed.ParsePartialFromArray(msg.data(), msg.size());
    google::protobuf::util::JsonOptions op;
    op.always_print_primitive_fields = true;
    op.always_print_enums_as_ints = false;
    op.preserve_proto_field_names = false;
    std::string output;
    google::protobuf::util::MessageToJsonString(orderFeed, &output, op);
    LOG_DEBUG("{}", output);
}

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
    orderRequest.mutable_order()->set_reserve_int32(1);
    
    zmq::message_t req(orderRequest.ByteSizeLong());
    orderRequest.SerializePartialToArray(req.data(), req.size());
    dealer_.send(orderRequest.GetTypeName().c_str(), orderRequest.GetTypeName().size(), ZMQ_SNDMORE);
    dealer_.send(req);
    timers_cancel(id_, this);
}
