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
#include "base_definitions.pb.h"
#include "zmq.hpp"
#include "first_definitions.pb.h"

xapi_impl::xapi_impl(zmq::socket_t* push)
: push_(push) {
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
    FIRST::InstrumentField instrumentField;
    //instrumentField.set_instrument_name(pInstrument->InstrumentName);
    instrumentField.set_symbol(pInstrument->Symbol);
    instrumentField.set_instrument_id(pInstrument->InstrumentID);
    instrumentField.set_exchange_id(pInstrument->ExchangeID);
    instrumentField.set_client_id(pInstrument->ClientID);
    instrumentField.set_account_id(pInstrument->AccountID);
    instrumentField.set_exchange_instid(pInstrument->ExchangeInstID);
    instrumentField.set_type((FIRST::InstrumentType)pInstrument->Type);
    instrumentField.set_volume_multiple(pInstrument->VolumeMultiple);
    instrumentField.set_price_tick(pInstrument->PriceTick);
    instrumentField.set_expire_date(pInstrument->ExpireDate);
    instrumentField.set_strike_price(pInstrument->StrikePrice);
    instrumentField.set_options_type((FIRST::PutCall)pInstrument->OptionsType);
    instrumentField.set_product_id(pInstrument->ProductID);
    instrumentField.set_underlying_instrid(pInstrument->UnderlyingInstrID);
    instrumentField.set_instlife_phase((FIRST::InstLifePhaseType)pInstrument->InstLifePhase);
    instrumentField.set_is_last(bIsLast);
    zmq::message_t msg(instrumentField.ByteSizeLong());
    instrumentField.SerializePartialToArray(msg.data(), msg.size());
    try {
        push_->send(instrumentField.GetTypeName().c_str(), instrumentField.GetTypeName().size(), ZMQ_SNDMORE);
        push_->send(msg);
    } catch (zmq::error_t& e) {
        LOG_ERROR("{}", e.what());
    }
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
    FIRST::OrderFeed orderFeed;
    FIRST::OrderField& orderField = *orderFeed.mutable_order();
    //orderField.set_instrument_name()
    orderField.set_symbol(pOrder->Symbol);
    orderField.set_instrument_id(pOrder->InstrumentID);
    orderField.set_exchange_id(pOrder->ExchangeID);
    orderField.set_client_id(pOrder->ClientID);
    orderField.set_account_id(pOrder->AccountID);
    orderField.set_size((FIRST::OrderSide)pOrder->Side);
    orderField.set_qty(pOrder->Qty);
    orderField.set_price(pOrder->Price);
    orderField.set_open_close((FIRST::OpenCloseType)pOrder->OpenClose);
    orderField.set_hedge_flag((FIRST::HedgeFlagType)pOrder->HedgeFlag);
    orderField.set_date(pOrder->Date);
    orderField.set_time(pOrder->Time);
    orderField.set_id(pOrder->ID);
    orderField.set_order_id(pOrder->OrderID);
    orderField.set_local_id(pOrder->LocalID);
    orderField.set_type((FIRST::OrderType)pOrder->Type);
    orderField.set_stop_px(pOrder->StopPx);
    orderField.set_time_in_force((FIRST::TimeInForce)pOrder->TimeInForce);
    orderField.set_status((FIRST::OrderStatus)pOrder->Status);
    orderField.set_exec_type((FIRST::ExecType)pOrder->ExecType);
    orderField.set_leaves_qty(pOrder->LeavesQty);
    orderField.set_cum_qty(pOrder->CumQty);
    orderField.set_avg_px(pOrder->AvgPx);
    orderField.set_xerror_id(pOrder->XErrorID);
    orderField.set_raw_error_id(pOrder->RawErrorID);
    //orderField.set_text(pOrder->Text);
    orderField.set_client_order_id(pOrder->ClientOrderID);
    orderField.set_reserve_char64(pOrder->ReserveChar64);
    orderField.set_portfolio_id1(pOrder->PortfolioID1);
    orderField.set_portfolio_id2(pOrder->PortfolioID2);
    orderField.set_portfolio_id3(pOrder->PortfolioID3);
    orderField.set_business((FIRST::BusinessType)pOrder->Business);
    try {
        zmq::message_t msg(orderFeed.ByteSizeLong());
        orderFeed.SerializePartialToArray(msg.data(), msg.size());
        push_->send(orderFeed.GetTypeName().c_str(), orderFeed.GetTypeName().size(), ZMQ_SNDMORE);
        push_->send(msg);
    } catch (zmq::error_t& e) {
        LOG_ERROR("{}", e.what());
    }
    printf("%d,%s\r\n", pOrder->RawErrorID, pOrder->Text);
}

void xapi_impl::OnRtnQuote(CXApi* pApi, QuoteField* pQuote) {

}

void xapi_impl::OnRtnQuoteRequest(CXApi* pApi, QuoteRequestField* pQuoteRequest) {

}

void xapi_impl::OnRtnTrade(CXApi* pApi, TradeField* pTrade) {
    FIRST::TradeFeed tradeFeed;
    FIRST::TradeField* tradeField = tradeFeed.mutable_trade();
    tradeField->set_symbol(pTrade->Symbol);
    tradeField->set_instrument_id(pTrade->InstrumentID);
    tradeField->set_exchange_id(pTrade->ExchangeID);
    tradeField->set_client_id(pTrade->ClientID);
    tradeField->set_account_id(pTrade->AccountID);
    tradeField->set_side((FIRST::OrderSide)pTrade->Side);
    tradeField->set_qty(pTrade->Qty);
    tradeField->set_price(pTrade->Price);
    tradeField->set_open_close((::FIRST::OpenCloseType)pTrade->OpenClose);
    tradeField->set_hedge_flag((FIRST::HedgeFlagType)pTrade->HedgeFlag);
    tradeField->set_date(pTrade->Date);
    tradeField->set_time(pTrade->Time);
    tradeField->set_id(pTrade->ID);
    tradeField->set_tradeid(pTrade->TradeID);
    tradeField->set_commission(pTrade->Commission);
    tradeField->set_client_order_id(pTrade->ClientOrderID);
    tradeField->set_reservechar64(pTrade->ReserveChar64);
    tradeField->set_portfolioid1(pTrade->PortfolioID1);
    tradeField->set_portfolioid2(pTrade->PortfolioID2);
    tradeField->set_portfolioid3(pTrade->PortfolioID3);
    tradeField->set_business((FIRST::BusinessType)pTrade->Business);
    
    zmq::message_t msg(tradeFeed.ByteSizeLong());
    tradeFeed.SerializePartialToArray(msg.data(), msg.size());
    push_->send(tradeFeed.GetTypeName().c_str(), tradeFeed.GetTypeName().size(), ZMQ_SNDMORE);
    push_->send(msg);
}





