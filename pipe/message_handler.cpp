//
// Created by chengye.ke on 2018-12-23.
//

#include "base_definitions.pb.h"
#include "message_handler.h"
#include "google/protobuf/util/json_util.h"

message_handler::message_handler(zmq::context_t* ctx, zmq_poller_reactor* reactor)
: proto_message_dispatcher(reactor)
, router_(*ctx, ZMQ_ROUTER)
, push_(*ctx, ZMQ_PUSH)
, pull_(*ctx, ZMQ_PULL)
, reactor_(reactor) 
, spi_(NULL)
, trader_(NULL)
, md_(NULL){
    add_pull_msg_mapping<FIRST::InstrumentField>(FIRST::InstrumentField::default_instance().GetTypeName(),&message_handler::on_resp_instrument);
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
    ReqQueryField query_field;
    memset(&query_field, 0,sizeof(ReqQueryField));
    trader_->ReqQuery(QueryType_ReqQryInstrument, &query_field);
    this->cancell_timer(id_);
}

bool message_handler::init() {
    add_router_msg_mapping<FIRST::LogField>("1001", &message_handler::on_req_login);
    //router_.setsockopt(ZMQ_IDENTITY, "SUB", 3);
    push_.bind("inproc://xapi");
    spi_ = new xapi_impl(&push_);
    
    pull_.connect("inproc://xapi");
    router_.bind("tcp://*:10003");
    this->add_socket(&pull_);
    this->add_socket(&router_);

    this->add_timer(1, 5000);


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
    
    return true;
}

void message_handler::on_req_login(zmq::message_t& rid, google::protobuf::Message& body) {
    LOG_DEBUG("on_req_lon {} {}", (char*) rid.data(), body.DebugString());
}

void message_handler::on_resp_instrument(google::protobuf::Message& body) {
    google::protobuf::util::JsonOptions op;
    //op.always_print_primitive_fields = true;
    op.always_print_enums_as_ints = false;
    op.preserve_proto_field_names = false;
    
    std::string output;
    google::protobuf::util::MessageToJsonString(body,&output,op);
    LOG_DEBUG("{}",output);
}