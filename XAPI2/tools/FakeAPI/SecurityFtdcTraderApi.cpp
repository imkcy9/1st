#include "stdafx.h"

#define ISLIB
#define LIB_TRADER_API_EXPORT

#include <stdio.h>
#include "../../include/LTS_v2/SecurityFtdcTraderApi.h"


class MyClassLTS :public CSecurityFtdcTraderApi
{

public:
	///����TraderApi
	///@param pszFlowPath ����������Ϣ�ļ���Ŀ¼��Ĭ��Ϊ��ǰĿ¼
	///@return ��������UserApi
	static CSecurityFtdcTraderApi *CreateFtdcTraderApi(const char *pszFlowPath = "");

	///ɾ���ӿڶ�����
	///@remark ����ʹ�ñ��ӿڶ���ʱ,���øú���ɾ���ӿڶ���
	virtual void Release(){}

	///��ʼ��
	///@remark ��ʼ�����л���,ֻ�е��ú�,�ӿڲſ�ʼ����
	virtual void Init()
	{
		m_pSpi->OnFrontConnected();
	}

	///�ȴ��ӿ��߳̽�������
	///@return �߳��˳�����
	virtual int Join(){ return 0; }

	///��ȡ��ǰ������
	///@retrun ��ȡ���Ľ�����
	///@remark ֻ�е�¼�ɹ���,���ܵõ���ȷ�Ľ�����
	virtual const char *GetTradingDay(){ return nullptr; }

	///ע��ǰ�û������ַ
	///@param pszFrontAddress��ǰ�û������ַ��
	///@remark �����ַ�ĸ�ʽΪ����protocol://ipaddress:port�����磺��tcp://127.0.0.1:17001���� 
	///@remark ��tcp��������Э�飬��127.0.0.1�������������ַ����17001������������˿ںš�
	virtual void RegisterFront(char *pszFrontAddress)
	{

	}

	///ע��ص��ӿ�
	///@param pSpi �����Իص��ӿ����ʵ��
	virtual void RegisterSpi(CSecurityFtdcTraderSpi *pSpi)
	{
		m_pSpi = pSpi;
	}

	///����˽������
	///@param nResumeType ˽�����ش���ʽ  
	///        SECURITY_TERT_RESTART:�ӱ������տ�ʼ�ش�
	///        SECURITY_TERT_RESUME:���ϴ��յ�������
	///        SECURITY_TERT_QUICK:ֻ���͵�¼��˽����������
	///@remark �÷���Ҫ��Init����ǰ���á����������򲻻��յ�˽���������ݡ�
	virtual void SubscribePrivateTopic(SECURITY_TE_RESUME_TYPE nResumeType){}

	///���Ĺ�������
	///@param nResumeType �������ش���ʽ  
	///        SECURITY_TERT_RESTART:�ӱ������տ�ʼ�ش�
	///        SECURITY_TERT_RESUME:���ϴ��յ�������
	///        SECURITY_TERT_QUICK:ֻ���͵�¼�󹫹���������
	///@remark �÷���Ҫ��Init����ǰ���á����������򲻻��յ������������ݡ�
	virtual void SubscribePublicTopic(SECURITY_TE_RESUME_TYPE nResumeType){}

	///�û���¼����
	virtual int ReqUserLogin(CSecurityFtdcReqUserLoginField *pReqUserLoginField, int nRequestID)
	{
		char buf[512] = { 0 };
		sprintf(buf,"UserID:%s\nPassword:%s\nUserProductInfo:%s\nAuthCode:%s\nRandCode:%s\n�Ѿ����Ƶ�������",
			pReqUserLoginField->UserID,
			pReqUserLoginField->Password,
			pReqUserLoginField->UserProductInfo,
			pReqUserLoginField->AuthCode,
			pReqUserLoginField->RandCode);

		if (OpenClipboard(NULL))
		{
			int len = strlen(buf) + 1;

			HGLOBAL hmem = GlobalAlloc(GHND, len);
			char *pmem = (char*)GlobalLock(hmem);

			EmptyClipboard();
			strcpy(pmem, buf);
			SetClipboardData(CF_TEXT, hmem);
			CloseClipboard();
			GlobalFree(hmem);
		}

		MessageBoxA(nullptr, buf, "", MB_OK);

		return 0;
	}


	///�ǳ�����
	virtual int ReqUserLogout(CSecurityFtdcUserLogoutField *pUserLogout, int nRequestID){ return 0; }

	///��ȡ��֤���������
	virtual int ReqFetchAuthRandCode(CSecurityFtdcAuthRandCodeField *pAuthRandCode, int nRequestID)
	{
		CSecurityFtdcAuthRandCodeField body = { 0 };

		strcpy(body.RandCode, "ABCDEF123456");

		m_pSpi->OnRspFetchAuthRandCode(&body, nullptr, nRequestID, true);

		return 0;
	}

	///����¼������
	virtual int ReqOrderInsert(CSecurityFtdcInputOrderField *pInputOrder, int nRequestID){ return 0; }

	///������������
	virtual int ReqOrderAction(CSecurityFtdcInputOrderActionField *pInputOrderAction, int nRequestID){ return 0; }

	///�û������������
	virtual int ReqUserPasswordUpdate(CSecurityFtdcUserPasswordUpdateField *pUserPasswordUpdate, int nRequestID) { return 0; }

	///�ʽ��˻������������
	virtual int ReqTradingAccountPasswordUpdate(CSecurityFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, int nRequestID) { return 0; }

	///Liber�����������
	virtual int ReqFundOutByLiber(CSecurityFtdcInputFundTransferField *pInputFundTransfer, int nRequestID) { return 0; }

	///�ʽ���ת����
	virtual int ReqFundInterTransfer(CSecurityFtdcFundInterTransferField *pFundInterTransfer, int nRequestID) { return 0; }

private:
	CSecurityFtdcTraderSpi *m_pSpi;
};

CSecurityFtdcTraderApi *CSecurityFtdcTraderApi::CreateFtdcTraderApi(const char *pszFlowPath)
{
	return new MyClassLTS();
}

