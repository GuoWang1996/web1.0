#pragma once
#include "pch.h"
#include "ͨ�ýṹ��.h"
class HPClient :public CTcpPullClientListener
{
public:
	����Ϣ m_����Ϣ;
public:
	HPClient();
	~HPClient() {};
	virtual EnHandleResult OnConnect(ITcpClient* pSender, CONNID dwConnID);//�ɹ�����������������Ӻ󴥷�
	virtual EnHandleResult OnSend(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength);//���ݷ��ͳɹ�����
	virtual EnHandleResult OnReceive(ITcpClient* pSender, CONNID dwConnID, int iLength);//���յ�����ʱ����
	virtual EnHandleResult OnClose(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode);//�����������쳣�رմ���
	BOOL ���ӷ�����(TCHAR *ip, USHORT port);
	BOOL �Ͽ�����();
	void ������(DWORD id, CBufferPtr pBuffer);
	BOOL ������Ϣ();
private:
	CTcpPullClientPtr m_Client;
private:
	//�Զ��庯��
	static ����Ϣ* �ҵ�����Ϣͨ��CID( CONNID dwConnID);
	static void  ɾ������Ϣͨ��CID(CONNID dwConnID);
};
