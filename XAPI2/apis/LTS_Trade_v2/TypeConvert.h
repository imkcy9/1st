#pragma once

#include "../../include/LTS_v2/SecurityFtdcUserApiDataType.h"
#include "../../include/LTS_v2/SecurityFtdcUserApiStruct.h"
#include "../../include/ApiStruct.h"


HedgeFlagType TSecurityFtdcHedgeFlagType_2_HedgeFlagType(TSecurityFtdcHedgeFlagType In);
TSecurityFtdcHedgeFlagType HedgeFlagType_2_TSecurityFtdcHedgeFlagType(HedgeFlagType In);

TSecurityFtdcOffsetFlagType OpenCloseType_2_TSecurityFtdcOffsetFlagType(OpenCloseType In);
OpenCloseType TSecurityFtdcOffsetFlagType_2_OpenCloseType(TSecurityFtdcOffsetFlagType In);

TSecurityFtdcDirectionType OrderSide_2_TSecurityFtdcDirectionType(OrderSide In);
OrderSide TSecurityFtdcDirectionType_2_OrderSide(TSecurityFtdcDirectionType In);
PositionSide TSecurityFtdcPosiDirectionType_2_PositionSide(TSecurityFtdcPosiDirectionType In);
PositionSide TradeField_2_PositionSide(TradeField* pIn);

TSecurityFtdcOrderPriceTypeType OrderType_2_TSecurityFtdcOrderPriceTypeType(OrderType In);

OrderStatus CSecurityFtdcOrderField_2_OrderStatus(CSecurityFtdcOrderField* pIn);
OrderType CSecurityFtdcOrderField_2_OrderType(CSecurityFtdcOrderField* pIn);
TimeInForce CSecurityFtdcOrderField_2_TimeInForce(CSecurityFtdcOrderField* pIn);
ExecType CSecurityFtdcOrderField_2_ExecType(CSecurityFtdcOrderField* pIn);

InstrumentType CSecurityFtdcInstrumentField_2_InstrumentType(CSecurityFtdcInstrumentField* pIn);
PutCall CSecurityFtdcInstrumentField_2_PutCall(CSecurityFtdcInstrumentField* pIn);
PriceType CSecurityFtdcInstrumentField_2_PriceTick(CSecurityFtdcInstrumentField* pIn);

IdCardType TSecurityFtdcIdCardTypeType_2_IdCardType(TSecurityFtdcIdCardTypeType In);

ExchangeType TSecurityFtdcExchangeIDType_2_ExchangeType(TSecurityFtdcExchangeIDType In);

void CSecurityFtdcOrderField_2_OrderField_0(OrderIDType OrderID, CSecurityFtdcOrderField* pIn, OrderField* pOut);