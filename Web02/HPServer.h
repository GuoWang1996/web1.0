#pragma once
#include "pch.h"
#include "通用结构体.h"
class HPServer: public CTcpPullServerListener
{
public:
	HPServer();
	~HPServer() {};
private:
	//自定义函数
	static 包信息* 找到包信息通过CID(ITcpServer* pSender,CONNID dwConnID);
	static void  删除包信息通过CID(ITcpServer* pSender,CONNID dwConnID);
private:
	CTcpPullServerPtr m_Server;//server组件指针 需要初始化时传入监听器对象
public:
	virtual EnHandleResult OnPrepareListen(ITcpServer* pSender, SOCKET soListen);//绑定监听地址前触发
	virtual EnHandleResult OnAccept(ITcpServer* pSender, CONNID dwConnID, UINT_PTR soClient);//客户端连接请求到达时触发
	virtual EnHandleResult OnSend(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength);//数据发送成功触发
	virtual EnHandleResult OnReceive(ITcpServer* pSender, CONNID dwConnID, int iLength);//接收到数据时触发
	virtual EnHandleResult OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode);//连接正常或异常关闭触发
	virtual EnHandleResult OnShutdown(ITcpServer* pSender);//关闭通信组件触发
	BOOL 开启服务器(TCHAR *ip,USHORT port);
	BOOL 停止服务器();
	void 关闭连接(ITcpServer* pSender,CONNID dwConnID);
	void 处理函数(CONNID dwConnID,DWORD id,CBufferPtr pBuffer);
};


