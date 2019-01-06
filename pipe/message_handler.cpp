//
// Created by chengye.ke on 2018-12-23.
//

#include "base_definitions.pb.h"
#include "first_definitions.pb.h"
#include "message_handler.h"
#include "google/protobuf/util/json_util.h"

#include "order_sender.h"

message_handler::message_handler(zmq::context_t* ctx, zmq_poller_reactor* reactor)
: proto_message_dispatcher(reactor)
, router_(*ctx, ZMQ_ROUTER)
, push_(*ctx, ZMQ_PUSH)
, pull_(*ctx, ZMQ_PULL)
, reactor_(reactor)
, spi_(NULL)
, trader_(NULL)
, md_(NULL) {
    add_pull_msg_mapping<FIRST::InstrumentField>(&message_handler::on_resp_instrument);
    add_pull_msg_mapping<FIRST::OrderFeed>(&message_handler::on_order_feed);
    add_pull_msg_mapping<FIRST::TradeFeed>(&message_handler::on_trade_feed);
    add_router_msg_mapping<FIRST::CreateOrderRequest>(&message_handler::on_create_order_request);
    add_router_msg_mapping<FIRST::CancelOrderRequest>(&message_handler::cancel_order_request);
}

message_handler::~message_handler() {
    router_.close();
    push_.close();
    pull_.close();
    trader_->Disconnect();
    md_->Disconnect();
}

void message_handler::zmq_timer_event(int id_) {
    LOG_DEBUG("on timer {}", id_);
    if (id_ == 1) {
        ReqQueryField query_field;
        memset(&query_field, 0, sizeof (ReqQueryField));
        trader_->ReqQuery(QueryType_ReqQryInstrument, &query_field);
        this->cancell_timer(id_);
    }

    if (id_ == 2) {
        OrderIDType Out = {1};
        OrderField* pOrder = new OrderField();
        memset(pOrder, 0, sizeof (OrderField));
        strcpy(pOrder->InstrumentID, "SR901");
        pOrder->OpenClose = OpenCloseType::OpenCloseType_Open;
        pOrder->HedgeFlag = HedgeFlagType::HedgeFlagType_Speculation;
        pOrder->Price = 4800;
        pOrder->Qty = 1;
        pOrder->Type = OrderType::OrderType_Limit;
        pOrder->Side = OrderSide::OrderSide_Buy;
        pOrder->ClientOrderID = 11;

        trader_->SendOrder(pOrder, 1, Out);

        sleep(2);
        OrderIDType cancelid = {1};
        trader_->CancelOrder(&Out, 1, cancelid);
        this->cancell_timer(id_);
    }
}

bool message_handler::init() {
    add_router_msg_mapping<FIRST::LogField>(&message_handler::on_req_login);
    //router_.setsockopt(ZMQ_IDENTITY, "SUB", 3);
    push_.bind("inproc://xapi");
    spi_ = new xapi_impl(&push_);

    pull_.connect("inproc://xapi");
    router_.bind("tcp://*:10003");
    //router_.bind("ipc:///home/kcy/1st/bin/ipc");
    this->add_socket(&pull_);
    this->add_socket(&router_);

    this->add_timer(1, 2000);
    //this->add_timer(2, 5000);


    char DLLPath1[1024] = "libCTP_Trade.so";
    char DLLPath2[1024] = "libCTP_Quote.so";
    ServerInfoField m_ServerInfo1 = {0};
    ServerInfoField m_ServerInfo2 = {0};
    UserInfoField m_UserInfo = {0};

    strcpy(m_ServerInfo1.BrokerID, "9999");
    strcpy(m_ServerInfo1.Address, "tcp://180.168.146.187:10030");

    strcpy(m_ServerInfo2.BrokerID, "9999");
    strcpy(m_ServerInfo2.Address, "tcp://180.168.146.187:10031");

    strcpy(m_UserInfo.UserID, "005890");
    strcpy(m_UserInfo.Password, "123456");

    trader_ = CXApi::CreateApi(DLLPath1);
    md_ = CXApi::CreateApi(DLLPath2);


    if (!trader_->Init()) {
        printf("%s\r\n", trader_->GetLastError());
        trader_->Disconnect();
        return false;
    }

    if (!md_->Init()) {
        printf("%s\r\n", md_->GetLastError());
        md_->Disconnect();
        return false;
    }

    trader_->RegisterSpi(spi_);
    trader_->Connect("./", &m_ServerInfo1, &m_UserInfo, 1);
    printf("trader_ Connect\n");

    md_->RegisterSpi(spi_);
    md_->Connect("./", &m_ServerInfo2, &m_UserInfo, 1);
    printf("md_ Connect\n");


    //test
    order_sender* test_sender = new order_sender();
    test_sender->init();
    test_sender->start();
    


    return true;
}

void message_handler::on_req_login(zmq::socket_t* router, zmq::message_t& rid, google::protobuf::Message& body) {
    LOG_DEBUG("on_req_lon {} {}", (char*) rid.data(), body.DebugString());
}

void message_handler::on_create_order_request(zmq::socket_t* router, zmq::message_t& rid, google::protobuf::Message& body) {
    FIRST::CreateOrderRequest& createOrderRequest = (FIRST::CreateOrderRequest&)body;
    //todo: order manage

    FIRST::OrderField* orderField = createOrderRequest.mutable_order();

    OrderIDType Out = {0};
    OrderField xpiOrderField = {0};
    strncpy(xpiOrderField.InstrumentID, orderField->instrument_id().c_str(), orderField->instrument_id().size() + 1);
    strncpy(xpiOrderField.Symbol, orderField->symbol().c_str(), orderField->symbol().size() + 1);
    strncpy(xpiOrderField.ExchangeID, orderField->exchange_id().c_str(), orderField->exchange_id().size());
    strncpy(xpiOrderField.ClientID, orderField->client_id().c_str(), orderField->client_id().size());
    strncpy(xpiOrderField.AccountID, orderField->account_id().c_str(), orderField->account_id().size());
    xpiOrderField.Side = (OrderSide) orderField->size(); //OrderSide::OrderSide_Buy;
    xpiOrderField.Qty = orderField->qty();
    xpiOrderField.Price = orderField->price();
    xpiOrderField.OpenClose = (OpenCloseType) orderField->open_close(); // OpenCloseType::OpenCloseType_Open;
    xpiOrderField.HedgeFlag = (HedgeFlagType) orderField->hedge_flag(); //HedgeFlagType::HedgeFlagType_Speculation;
    xpiOrderField.Price = orderField->price();
    xpiOrderField.Qty = orderField->qty();
    xpiOrderField.Type = (OrderType) orderField->type(); //OrderType::OrderType_Limit;
    xpiOrderField.StopPx = orderField->stop_px();
    xpiOrderField.TimeInForce = (TimeInForce)orderField->time_in_force();
    xpiOrderField.Status = (OrderStatus)orderField->status();
    xpiOrderField.ExecType = (ExecType)orderField->exec_type();
    //xpiOrderField.LeavesQty = orderField->leaves_qty();
    
    //strncpy(xpiOrderField.OrderID, "9", sizeof (xpiOrderField.OrderID));
    //strncpy(xpiOrderField.OrderID, "222", sizeof (xpiOrderField.OrderID));
    //strncpy(xpiOrderField.LocalID, "10", sizeof (xpiOrderField.LocalID));
    xpiOrderField.ClientOrderID = orderField->client_order_id();
    strncpy(xpiOrderField.ReserveChar64, orderField->reserve_char64().c_str(), orderField->reserve_char64().size());
    strncpy(xpiOrderField.PortfolioID1, orderField->portfolio_id1().c_str(), orderField->portfolio_id1().size());
    strncpy(xpiOrderField.PortfolioID2, orderField->portfolio_id2().c_str(), orderField->portfolio_id2().size());
    strncpy(xpiOrderField.PortfolioID3, orderField->portfolio_id3().c_str(), orderField->portfolio_id3().size());
    xpiOrderField.Business = (BusinessType)orderField->business();
    trader_->SendOrder(&xpiOrderField, 1, Out);

    //记录ID与router ID
    router_mapping_.insert(std::make_pair(Out, std::string((const char*) rid.data(), rid.size())));

    FIRST::CreateOrderResponse createOrderResponse;
    createOrderResponse.mutable_order()->CopyFrom(*orderField);
    createOrderResponse.set_request_id(createOrderRequest.request_id());
    createOrderResponse.mutable_order()->set_status(FIRST::OrderStatus::PENDING_NEW);
    createOrderResponse.mutable_order()->set_exec_type(FIRST::ExecType::EXECTYPE_PENDING_NEW);

    
    zmq::message_t resp(createOrderResponse.ByteSizeLong());
    createOrderResponse.SerializePartialToArray(resp.data(), resp.size());
    router->send(rid, ZMQ_SNDMORE);
    router->send(createOrderResponse.GetTypeName().c_str(), createOrderResponse.GetTypeName().size(), ZMQ_SNDMORE);
    router->send(resp);
}

void message_handler::cancel_order_request(zmq::socket_t* router, zmq::message_t& rid, google::protobuf::Message& body) {
    FIRST::CancelOrderRequest& req = (FIRST::CancelOrderRequest&)body;
    OrderIDType in = {0};
    memcpy(in,req.order().id().c_str(), req.order().id().size());//req.order().id().c_str();
    OrderIDType out = {0};
    trader_->CancelOrder(&in, 1, out);
}


//------------------response from xapi----------------------------

void message_handler::on_resp_instrument(google::protobuf::Message& body) {
    google::protobuf::util::JsonOptions op;
    //op.always_print_primitive_fields = true;
    op.always_print_enums_as_ints = false;
    op.preserve_proto_field_names = false;

    std::string output;
    google::protobuf::util::MessageToJsonString(body, &output, op);
    LOG_DEBUG("{}", output);
}

void message_handler::on_order_feed(google::protobuf::Message& body) {
    
    google::protobuf::util::JsonOptions op;
    op.always_print_primitive_fields = true;
    op.always_print_enums_as_ints = false;
    op.preserve_proto_field_names = false;

    std::string output;
    google::protobuf::util::MessageToJsonString(body, &output, op);
    
    
    FIRST::OrderFeed& orderFeed = (FIRST::OrderFeed&)body;
    auto it = router_mapping_.find(orderFeed.order().id());
    if (it == router_mapping_.end()) {
        LOG_WARN("not a order send from FIRST {}", output);
        return;
    }
    LOG_DEBUG("{}", output);
    
    zmq::message_t feed(orderFeed.ByteSizeLong());
    orderFeed.SerializePartialToArray(feed.data(), feed.size());
    router_.send(it->second.c_str(), it->second.size(), ZMQ_SNDMORE);
    router_.send(orderFeed.GetTypeName().c_str(), orderFeed.GetTypeName().size(), ZMQ_SNDMORE);
    router_.send(feed);
    //auto it = router_mapping
}

void message_handler::on_trade_feed(google::protobuf::Message& body) {
    FIRST::TradeFeed& tradeFeed = (FIRST::TradeFeed&)body;
    zmq::message_t feed(tradeFeed.ByteSizeLong());
    tradeFeed.SerializePartialToArray(feed.data(), feed.size());
    auto it = router_mapping_.find(tradeFeed.trade().id());
    if (it == router_mapping_.end()) {
        LOG_WARN("not a trade send from FIRST {}", body.DebugString());
        return;
    }
    router_.send(it->second.c_str(), it->second.size(), ZMQ_SNDMORE);
    router_.send(tradeFeed.GetTypeName().c_str(), tradeFeed.GetTypeName().size(), ZMQ_SNDMORE);
    router_.send(feed);
}
