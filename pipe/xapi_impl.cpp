/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   xapi_impl.cpp
 * Author: kecy
 * 
 * Created on 2018年12月30日, 下午7:08
 */

#include "xapi_impl.h"
#include <stdio.h>
#include <time.h>
#include "log.h"
#include "trader.pb.h"
#include "zmq.hpp"

xapi_impl::xapi_impl(zmq::socket_t* push)
:push_(push)
{
}

xapi_impl::~xapi_impl() {
}

void xapi_impl::OnConnectionStatus(CXApi* pApi, ConnectionStatus status, RspUserLoginField* pUserLogin, int size1) {
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
                sprintf(buf, "IF%d%02d", (1900 + timeinfo->tm_year + a) % 100, b + 1);
                //pApi->Subscribe(buf, "");
                sprintf(buf, "TF%d%02d", (1900 + timeinfo->tm_year + a) % 100, b + 1);
                //pApi->Subscribe(buf, "");
            }
        }
    }
}

bool xapi_impl::OnFilterSubscribe(CXApi* pApi, ExchangeType exchange, int instrument_part1, int instrument_part2, int instrument_part3, char* pInstrument) {
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

void xapi_impl::OnLog(CXApi* pApi, LogField* pLog) {

}

void xapi_impl::OnRspQryHistoricalBars(CXApi* pApi, BarField* pBars, int size1, HistoricalDataRequestField* pRequest, int size2, bool bIsLast) {

}

void xapi_impl::OnRspQryHistoricalTicks(CXApi* pApi, TickField* pTicks, int size1, HistoricalDataRequestField* pRequest, int size2, bool bIsLast) {

}

void xapi_impl::OnRspQryInstrument(CXApi* pApi, InstrumentField* pInstrument, int size1, bool bIsLast) {
    int i = 0;
    LT::InstrumentField instrumentField;
    //instrumentField.set_instrument_name(pInstrument->InstrumentName);
    instrumentField.set_symbol(pInstrument->Symbol);
    instrumentField.set_instrument_id(pInstrument->InstrumentID);
    instrumentField.set_exchange_id(pInstrument->ExchangeID);
    instrumentField.set_client_id(pInstrument->ClientID);
    instrumentField.set_account_id(pInstrument->AccountID);
    instrumentField.set_exchange_instid(pInstrument->ExchangeInstID);
    instrumentField.set_type((int)pInstrument->Type);
    instrumentField.set_volume_multiple(pInstrument->VolumeMultiple);
    instrumentField.set_price_tick(pInstrument->PriceTick);
    instrumentField.set_expire_date(pInstrument->ExpireDate);
    instrumentField.set_strike_price(pInstrument->StrikePrice);
    instrumentField.set_options_type((int)pInstrument->OptionsType);
    instrumentField.set_product_id(pInstrument->ProductID);
    instrumentField.set_underlying_instrid(pInstrument->UnderlyingInstrID);
    instrumentField.set_instlife_phase((int)pInstrument->InstLifePhase);
    zmq::message_t msg(instrumentField.ByteSizeLong());
    instrumentField.SerializePartialToArray(msg.data(), msg.size());
    push_->send(instrumentField.GetTypeName().c_str(), instrumentField.GetTypeName().size(), ZMQ_SNDMORE);
    push_->send(msg);
}

void xapi_impl::OnRspQryInvestor(CXApi* pApi, InvestorField* pInvestor, int size1, bool bIsLast) {

}

void xapi_impl::OnRspQryInvestorPosition(CXApi* pApi, PositionField* pPosition, int size1, bool bIsLast) {
    printf("%s,%s\r\n", pPosition->InstrumentName, pPosition->InstrumentID);
}

void xapi_impl::OnRspQryOrder(CXApi* pApi, OrderField* pOrder, int size1, bool bIsLast) {

}

void xapi_impl::OnRspQryQuote(CXApi* pApi, QuoteField* pQuote, int size1, bool bIsLast) {

}

void xapi_impl::OnRspQrySettlementInfo(CXApi* pApi, SettlementInfoField* pSettlementInfo, int size1, bool bIsLast) {

}

void xapi_impl::OnRspQryTrade(CXApi* pApi, TradeField* pTrade, int size1, bool bIsLast) {

}

void xapi_impl::OnRspQryTradingAccount(CXApi* pApi, AccountField* pAccount, int size1, bool bIsLast) {

}

void xapi_impl::OnRtnDepthMarketDataN(CXApi* pApi, DepthMarketDataNField* pMarketData) {
    printf("%s,%f\r\n", pMarketData->Symbol, pMarketData->LastPrice);

    int size = sizeof (DepthField);
    char* pBid = ((char*) pMarketData) + sizeof (DepthMarketDataNField);
    int AskCount = (pMarketData->Size - sizeof (DepthMarketDataNField)) / size - pMarketData->BidCount;
    char* pAsk = ((char*) pMarketData) + sizeof (DepthMarketDataNField) + pMarketData->BidCount*size;
    for (int i = 0; i < pMarketData->BidCount; ++i) {
        DepthField* pDF = (DepthField*) (pBid + i * size);
        printf("%d,%f,%d,%d\r\n", i, pDF->Price, pDF->Size, pDF->Count);
    }
    for (int i = 0; i < AskCount; ++i) {
        DepthField* pDF = (DepthField*) (pAsk + i * size);
        printf("%d,%f,%d,%d\r\n", i, pDF->Price, pDF->Size, pDF->Count);
    }
}

void xapi_impl::OnRtnError(CXApi* pApi, ErrorField* pError) {

}

void xapi_impl::OnRtnInstrumentStatus(CXApi* pApi, InstrumentStatusField* pInstrumentStatus) {

}

void xapi_impl::OnRtnOrder(CXApi* pApi, OrderField* pOrder) {
    printf("%d,%s\r\n", pOrder->RawErrorID, pOrder->Text);
}

void xapi_impl::OnRtnQuote(CXApi* pApi, QuoteField* pQuote) {

}

void xapi_impl::OnRtnQuoteRequest(CXApi* pApi, QuoteRequestField* pQuoteRequest) {

}

void xapi_impl::OnRtnTrade(CXApi* pApi, TradeField* pTrade) {

}

