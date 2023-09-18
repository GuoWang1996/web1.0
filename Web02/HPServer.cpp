#include "pch.h"
#include "HPServer.h"
#include "afxdialogex.h"
HPServer::HPServer() : m_Server(this)
{
}

//绑定监听地址前触发 由server组件中间件返回
EnHandleResult HPServer::OnPrepareListen(ITcpServer* pSender, SOCKET soListen)
{
	TCHAR szAddress[100];//本地监听ip地址
	int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);//地址缓冲区
	USHORT usPort;

	pSender->GetListenAddress(szAddress, iAddressLen, usPort);
	return HR_OK;
}
//客户端连接请求到达时触发 自动获取参数列表值
EnHandleResult HPServer::OnAccept(ITcpServer* pSender, CONNID dwConnID, UINT_PTR soClient)
{
		BOOL bPass = TRUE;
	TCHAR szAddress[100];//客户端ip地址缓冲区
	int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);//客户端ip地址缓冲区大小
	USHORT usPort;

	pSender->GetRemoteAddress(dwConnID, szAddress, iAddressLen, usPort);
	Call_输出调试信息("接收到客户端请求 ID:%d IP：%s,端口:%d,地址长度:%d \n", dwConnID, szAddress, usPort, iAddressLen);

	//绑定包信息
	if(bPass) pSender->SetConnectionExtra(dwConnID, new 包信息());
	return bPass ? HR_OK : HR_ERROR;
}
//数据发送成功触发
EnHandleResult HPServer::OnSend(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
	Call_输出调试信息("服务器send触发------------------\n");
	return HR_OK;
}
//接收到数据时触发
EnHandleResult HPServer::OnReceive(ITcpServer* pSender, CONNID dwConnID, int iLength)
{ 
	包信息* pInfo = 找到包信息通过CID(pSender, dwConnID);

	//父类强转为子类
	ITcpPullServer* pServer = ITcpPullServer::FromS(pSender);

	if (pInfo != nullptr)
	{
		int required = pInfo->packageLength;//初始包头长度 8
		int 接收包长 = iLength;//接收包长

		//如果接收包长小于包头长 忽略本次事件 当 接收包长 大于或等于当前期望的长度时，循环调用组件的
		//Fetch(dwConnID, pData, iDataLength) 方法把需要的数据拉取出来，直到剩余的数据长度小于当前期望的长度。
		while (接收包长 >= required)
		{
			接收包长 -= required;//接收包长-包头长=包身体长
			
			CBufferPtr buffer(required);

			EnFetchResult result = pServer->Fetch(dwConnID, buffer, (int)buffer.Size());
			if (result == FR_OK)
			{
				if (pInfo->isHead)
				{
					包头* pHeader = (包头*)buffer.Ptr();
					Call_输出调试信息("[Server] 包头 -> id: %d, 长度: %d\n", pHeader->packageId, pHeader->length);

					required = pHeader->length;
				}
				else
				{
					包身体* pBody = (包身体*)(BYTE*)buffer;
					Call_输出调试信息("[Server] 包内容 -> name: %s, age: %d, desc: %s\n", pBody->name, pBody->age, pBody->desc);

					required = sizeof(包头);
				}


				pInfo->isHead = !pInfo->isHead;
				pInfo->packageLength = required;


				/*if (!pSender->Send(dwConnID, buffer, (int)buffer.Size()))
					return HR_ERROR;*/
			}
		}
	}

	return HR_OK;
}
//客户端断开连接触发 服务器与客户端均可以向中间件发送DisConnect触发
EnHandleResult HPServer::OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
{
	删除包信息通过CID(pSender, dwConnID);
	Call_输出调试信息("客户端断开连接 ID:%d 错误码:%d \n", dwConnID, iErrorCode);
	return HR_OK;
}

EnHandleResult HPServer::OnShutdown(ITcpServer* pSender)
{
	return HR_OK;
}

包信息* HPServer::找到包信息通过CID(ITcpServer* pSender,CONNID dwConnID)
{
	PVOID pInfo = nullptr;

	pSender->GetConnectionExtra(dwConnID, &pInfo);

	return (包信息*)pInfo;
}

void  HPServer::删除包信息通过CID(ITcpServer* pSender,CONNID dwConnID)
{
	包信息* pInfo = 找到包信息通过CID(pSender, dwConnID);
	ASSERT(pInfo != nullptr);

	delete pInfo;
	pInfo = NULL;
}

BOOL HPServer::开启服务器(TCHAR *ip, USHORT port)
{
	if (m_Server->Start(ip, port))
	{
		Call_输出调试信息("开启服务器成功\n");
		return TRUE;
	}
	else
	{
		Call_输出调试信息("开启服务器失败 error:%d\n",GetLastError());
		return FALSE;
	}
}

BOOL HPServer::停止服务器()
{
	if (m_Server->Stop())
	{
		Call_输出调试信息("debug:停止服务器成功\n");
		return TRUE;
	}
	else
	{
		Call_输出调试信息("debug:停止服务器失败 error:%d\n", GetLastError());
		ASSERT(FALSE);
		return FALSE;
	}
}

void HPServer::关闭连接(ITcpServer* pSender,CONNID id)
{
	if (m_Server->Disconnect(id))
	{
		Call_输出调试信息("客户端断开连接成功\n");
	}
	else
	{
		Call_输出调试信息("客户端断开连接失败 error:%d\n", GetLastError());

	}
}

void HPServer::处理函数(CONNID dwConnID, DWORD id, CBufferPtr pBuffer)
{
	switch (id)
	{
	case WM_登录消息:
		//m_name = "";
		break;
	default:
		break;
	}
}

