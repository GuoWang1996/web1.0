#pragma once
#include "pch.h"
#include "ͨ�ýṹ��.h"
class HPServer: public CTcpPullServerListener
{
public:
	HPServer();
	~HPServer() {};
private:
	//�Զ��庯��
	static ����Ϣ* �ҵ�����Ϣͨ��CID(ITcpServer* pSender,CONNID dwConnID);
	static void  ɾ������Ϣͨ��CID(ITcpServer* pSender,CONNID dwConnID);
private:
	CTcpPullServerPtr m_Server;//server���ָ�� ��Ҫ��ʼ��ʱ�������������
public:
	virtual EnHandleResult OnPrepareListen(ITcpServer* pSender, SOCKET soListen);//�󶨼�����ַǰ����
	virtual EnHandleResult OnAccept(ITcpServer* pSender, CONNID dwConnID, UINT_PTR soClient);//�ͻ����������󵽴�ʱ����
	virtual EnHandleResult OnSend(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength);//���ݷ��ͳɹ�����
	virtual EnHandleResult OnReceive(ITcpServer* pSender, CONNID dwConnID, int iLength);//���յ�����ʱ����
	virtual EnHandleResult OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode);//�����������쳣�رմ���
	virtual EnHandleResult OnShutdown(ITcpServer* pSender);//�ر�ͨ���������
	BOOL ����������(TCHAR *ip,USHORT port);
	BOOL ֹͣ������();
	void �ر�����(ITcpServer* pSender,CONNID dwConnID);
	void ������(CONNID dwConnID,DWORD id,CBufferPtr pBuffer);
};


