/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   xapi_md_impl.h
 * Author: kecy
 *
 * Created on 2019年1月1日, 下午7:13
 */

#ifndef XAPI_MD_IMPL_H
#define XAPI_MD_IMPL_H
#include "XApiCpp.h"
#include "zmq.hpp"

class xapi_md_impl : public CXSpi {
public:
    xapi_md_impl(zmq::context_t* ctx);
    virtual ~xapi_md_impl();

    bool init();

    virtual void OnConnectionStatus(CXApi* pApi, ConnectionStatus status, RspUserLoginField* pUserLogin, int size1);

    virtual bool OnFilterSubscribe(CXApi* pApi, ExchangeType exchange, int instrument_part1, int instrument_part2, int instrument_part3, char* pInstrument);

    virtual void OnLog(CXApi* pApi, LogField* pLog);

    virtual void OnRtnDepthMarketDataN(CXApi* pApi, DepthMarketDataNField* pMarketData);

    virtual void OnRtnQuoteRequest(CXApi* pApi, QuoteRequestField* pQuoteRequest);
private:
    zmq::socket_t pub_;
};

#endif /* XAPI_MD_IMPL_H */

