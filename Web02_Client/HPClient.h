#pragma once
#include "pch.h"
#include "通用结构体.h"
class HPClient :public CTcpPullClientListener
{
public:
	包信息 m_包信息;
public:
	HPClient();
	~HPClient() {};
	virtual EnHandleResult OnConnect(ITcpClient* pSender, CONNID dwConnID);//成功建立与服务器的连接后触发
	virtual EnHandleResult OnSend(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength);//数据发送成功触发
	virtual EnHandleResult OnReceive(ITcpClient* pSender, CONNID dwConnID, int iLength);//接收到数据时触发
	virtual EnHandleResult OnClose(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode);//连接正常或异常关闭触发
	BOOL 连接服务器(TCHAR *ip, USHORT port);
	BOOL 断开连接();
	void 处理函数(DWORD id, CBufferPtr pBuffer);
	BOOL 发送信息();
private:
	CTcpPullClientPtr m_Client;
private:
	//自定义函数
	static 包信息* 找到包信息通过CID( CONNID dwConnID);
	static void  删除包信息通过CID(CONNID dwConnID);
};
