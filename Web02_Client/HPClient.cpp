#include "pch.h"
#include "HPClient.h"
HPClient::HPClient():m_Client(this)
{

}


 EnHandleResult HPClient::OnConnect(ITcpClient* pSender, CONNID dwConnID)
{
	 TCHAR szAddress[100];
	 int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
	 USHORT usPort;

	 pSender->GetLocalAddress(szAddress, iAddressLen, usPort);
	 return HR_OK;
}

EnHandleResult HPClient::OnSend(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
	Call_输出调试信息("客户端send触发------------------\n");
	return HR_OK;
}

EnHandleResult HPClient::OnReceive(ITcpClient* pSender, CONNID dwConnID, int iLength)
{
	
	ITcpPullClient* pClient = ITcpPullClient::FromS(pSender);

		int required = m_包信息.packageLength;//初始包头长度 8
		int 接收包长 = iLength;//接收包长

		//如果接收包长小于包头长 忽略本次事件 当 接收包长 大于或等于当前期望的长度时，循环调用组件的
		//Fetch(dwConnID, pData, iDataLength) 方法把需要的数据拉取出来，直到剩余的数据长度小于当前期望的长度。
		while (接收包长 >= required)
		{
			接收包长 -= required;//接收包长-包头长=包身体长

			CBufferPtr buffer(required);

			EnFetchResult result = pClient->Fetch(buffer, (int)buffer.Size());
			if (result == FR_OK)
			{
				if (m_包信息.isHead)
				{
					包头* pHeader = (包头*)buffer.Ptr();
					Call_输出调试信息("[Client] 包头 -> seq: %d, body_len: %d\n", pHeader->packageId, pHeader->length);

					required = pHeader->length;
				}
				else
				{
					包身体* pBody = (包身体*)(BYTE*)buffer;
					Call_输出调试信息("[Client] 包内容 -> name: %s, age: %d, desc: %s\n", pBody->name, pBody->age, pBody->desc);

					required = sizeof(包头);
				}

				m_包信息.isHead = !m_包信息.isHead;
				m_包信息.packageLength = required;


				if (!pSender->Send( buffer, (int)buffer.Size()))
					return HR_ERROR;
			}
		}
	

	return HR_OK;
}

EnHandleResult HPClient::OnClose(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
{
	Call_输出调试信息("客户端断开连接 ID:%d 错误码:%d \n", dwConnID, iErrorCode);
	return HR_OK;
}


BOOL HPClient::连接服务器(TCHAR *ip, USHORT port)
{
	m_包信息.重置();
	if (m_Client->GetState()== SS_STOPPED)
	{
		if (m_Client->Start(IP, PORT))
		{
			Call_输出调试信息("客户端连接成功 \n");
		}
		else
		{
			Call_输出调试信息("客户端连接失败 错误码:%d \n", GetLastError());
			return FALSE;

		}

		
	}
	return TRUE;
}

BOOL HPClient::断开连接()
{
	if (m_Client->Stop())
	{
		Call_输出调试信息("断开连接CONNID:%d \n", m_Client->GetConnectionID());
	}
	else 
	{
		ASSERT("断开连接失败");
		return FALSE;


	}
	return TRUE;
}

void HPClient::处理函数(DWORD id, CBufferPtr pBuffer)
{
	switch (id)
	{
	case WM_登录消息:
		break;
	default:
		break;
	}
}

BOOL HPClient::发送信息()
{
	static DWORD SEQ = 0;

	USES_CONVERSION;

	CString strContent="test";
	//m_Content.GetWindowText(strContent);

	LPCSTR name = "heart";
	LPCSTR desc = (LPCTSTR)strContent;
	int desc_len = (int)strlen(desc) + 1;
	int body_len = sizeof(包身体)+ 1;

	包身体* pBody = (包身体*)malloc(body_len);
	//将 pBody 指向的内存块的前 body_len 字节都设置为零。
	memset(pBody, 0, body_len);
	ZeroMemory(pBody, body_len+1);

	pBody->age = 23;
	strcpy(pBody->name, name);
	strcpy(pBody->desc, desc);

	包头 header;
	header.packageId = ++SEQ;
	header.length = body_len;

	WSABUF bufs[2];
	bufs[0].len = sizeof(包头);
	bufs[0].buf = (char*)&header;
	bufs[1].len = body_len;
	bufs[1].buf = (char*)pBody;

	if (m_Client->SendPackets(bufs, 2))
	{
		Call_输出调试信息("发送成功 \n");
		return TRUE;
	}
		//::LogSend(m_Client->GetConnectionID(), strContent);
	else
	{
		Call_输出调试信息("发送失败 error %d\n" ,GetLastError());
		return FALSE;

	}
		//::LogSendFail(m_Client->GetConnectionID(), ::SYS_GetLastError(), ::HP_GetSocketErrorDesc(SE_DATA_SEND));
	return TRUE;
}

包信息* HPClient::找到包信息通过CID(CONNID dwConnID)
{
	return NULL;
}

void HPClient::删除包信息通过CID(CONNID dwConnID)
{

}

