/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   xapi_md_impl.cpp
 * Author: kecy
 * 
 * Created on 2019年1月1日, 下午7:13
 */

#include "xapi_md_impl.h"
#include "base_definitions.pb.h"

xapi_md_impl::xapi_md_impl(zmq::context_t* ctx)
:pub_(*ctx,ZMQ_PUB){
}

xapi_md_impl::~xapi_md_impl() {
    pub_.close();
}

bool xapi_md_impl::init() {
    pub_.bind("inproc://md");
}

void xapi_md_impl::OnConnectionStatus(CXApi* pApi, ConnectionStatus status, RspUserLoginField* pUserLogin, int size1) {
    printf("%d\r\n", status);
    if (pUserLogin) {
        printf("%s\r\n", pUserLogin->Text);
    }
    if (status == ConnectionStatus::ConnectionStatus_Done) {
        if ((pApi->GetApiTypes() & ApiType::ApiType_MarketData) == ApiType::ApiType_MarketData) {
            time_t rawtime;
            struct tm * timeinfo;
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            int mon = timeinfo->tm_mon;
            char buf[64] = {0};
            for (int i = 0; i < 12; ++i) {
                int x = mon + i;
                int a = x / 12;
                int b = x % 12;
                //sprintf(buf, "IF%d%02d", (1900 + timeinfo->tm_year + a) % 100, b + 1);
                //pApi->Subscribe(buf, "");
                //sprintf(buf, "TF%d%02d", (1900 + timeinfo->tm_year + a) % 100, b + 1);
                //pApi->Subscribe("SR903", "");
            }
        }
    }
}

void xapi_md_impl::OnLog(CXApi* pApi, LogField* pLog) {

}

void xapi_md_impl::OnRtnDepthMarketDataN(CXApi* pApi, DepthMarketDataNField* pMarketData) {
    FIRST::Ticker t;
    FIRST::Ticker* ticker = &t;
   
    
    ticker->set_trading_day(pMarketData->TradingDay);
    ticker->set_update_time(pMarketData->UpdateTime);
    ticker->set_update_millisec(pMarketData->UpdateMillisec);
    ticker->set_symbol(pMarketData->Symbol);
    ticker->set_instrument_id(pMarketData->InstrumentID);
    ticker->set_exchange_id(pMarketData->ExchangeID);
    ticker->set_exchange((::FIRST::ExchangeType)pMarketData->Exchange);
    ticker->set_last_price(pMarketData->LastPrice);
    ticker->set_volume(pMarketData->Volume);
    ticker->set_turnover(pMarketData->Turnover);
    ticker->set_openinterest(pMarketData->OpenInterest);
    ticker->set_average_price(pMarketData->AveragePrice);
    ticker->set_open_price(pMarketData->OpenPrice);
    ticker->set_highest_price(pMarketData->HighestPrice);
    ticker->set_lowest_price(pMarketData->LowestPrice);
    ticker->set_close_price(pMarketData->ClosePrice);
    ticker->set_settlement_price(pMarketData->SettlementPrice);
    ticker->set_upperlimit_price(pMarketData->UpperLimitPrice);
    ticker->set_lowerlimit_price(pMarketData->LowerLimitPrice);
    ticker->set_preclose_price(pMarketData->PreClosePrice);
    ticker->set_presettlement_price(pMarketData->PreSettlementPrice);
    ticker->set_preopeninterest(pMarketData->PreOpenInterest);
    ticker->set_trading_phase((FIRST::TradingPhaseType)pMarketData->TradingPhase);
    
    int size = sizeof (DepthField);
    char* pBid = ((char*) pMarketData) + sizeof (DepthMarketDataNField);
    int AskCount = (pMarketData->Size - sizeof (DepthMarketDataNField)) / size - pMarketData->BidCount;
    char* pAsk = ((char*) pMarketData) + sizeof (DepthMarketDataNField) + pMarketData->BidCount*size;
    for (int i = 0; i < pMarketData->BidCount; ++i) {
        DepthField* pDF = (DepthField*) (pBid + i * size);
        ticker->add_bid_depth();
        ticker->mutable_bid_depth(i)->set_count(pDF->Count);
        ticker->mutable_bid_depth(i)->set_price(pDF->Price);
        ticker->mutable_bid_depth(i)->set_size(pDF->Size);
        //printf("%d,%f,%d,%d\r\n", i, pDF->Price, pDF->Size, pDF->Count);
    }
    for (int i = 0; i < AskCount; ++i) {
        DepthField* pDF = (DepthField*) (pAsk + i * size);
        ticker->add_offer_depth();
        ticker->mutable_offer_depth(i)->set_count(pDF->Count);
        ticker->mutable_offer_depth(i)->set_price(pDF->Price);
        ticker->mutable_offer_depth(i)->set_size(pDF->Size);
        //printf("%d,%f,%d,%d\r\n", i, pDF->Price, pDF->Size, pDF->Count);
    }
    
    zmq::message_t tickmsg(ticker->ByteSizeLong());
    ticker->SerializePartialToArray(tickmsg.data(), tickmsg.size());
    pub_.send(ticker->instrument_id().c_str(), ticker->instrument_id().size(), ZMQ_SNDMORE);
    pub_.send(tickmsg);
}

void xapi_md_impl::OnRtnQuoteRequest(CXApi* pApi, QuoteRequestField* pQuoteRequest) {

}

bool xapi_md_impl::OnFilterSubscribe(CXApi* pApi, ExchangeType exchange, int instrument_part1, int instrument_part2, int instrument_part3, char* pInstrument) {
    if (instrument_part1 == 0)
        // ֻҪ�Ϻ������ڣ�����������
        return exchange != ExchangeType::ExchangeType_NEEQ;

    //type = ExchangeType::SZSE;
    //double1 = 399300;

    int prefix1 = instrument_part1 / 100000;
    int prefix3 = instrument_part1 / 1000;
    switch (exchange) {
        case ExchangeType::ExchangeType_SSE:
            return (prefix1 == 6);
        case ExchangeType::ExchangeType_SZSE:
            return (prefix1 == 0) || (prefix3 == 300);
        default:
            break;
    }

    return true;
}
