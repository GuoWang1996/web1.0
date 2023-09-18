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
	Call_���������Ϣ("�ͻ���send����------------------\n");
	return HR_OK;
}

EnHandleResult HPClient::OnReceive(ITcpClient* pSender, CONNID dwConnID, int iLength)
{
	
	ITcpPullClient* pClient = ITcpPullClient::FromS(pSender);

		int required = m_����Ϣ.packageLength;//��ʼ��ͷ���� 8
		int ���հ��� = iLength;//���հ���

		//������հ���С�ڰ�ͷ�� ���Ա����¼� �� ���հ��� ���ڻ���ڵ�ǰ�����ĳ���ʱ��ѭ�����������
		//Fetch(dwConnID, pData, iDataLength) ��������Ҫ��������ȡ������ֱ��ʣ������ݳ���С�ڵ�ǰ�����ĳ��ȡ�
		while (���հ��� >= required)
		{
			���հ��� -= required;//���հ���-��ͷ��=�����峤

			CBufferPtr buffer(required);

			EnFetchResult result = pClient->Fetch(buffer, (int)buffer.Size());
			if (result == FR_OK)
			{
				if (m_����Ϣ.isHead)
				{
					��ͷ* pHeader = (��ͷ*)buffer.Ptr();
					Call_���������Ϣ("[Client] ��ͷ -> seq: %d, body_len: %d\n", pHeader->packageId, pHeader->length);

					required = pHeader->length;
				}
				else
				{
					������* pBody = (������*)(BYTE*)buffer;
					Call_���������Ϣ("[Client] ������ -> name: %s, age: %d, desc: %s\n", pBody->name, pBody->age, pBody->desc);

					required = sizeof(��ͷ);
				}

				m_����Ϣ.isHead = !m_����Ϣ.isHead;
				m_����Ϣ.packageLength = required;


				if (!pSender->Send( buffer, (int)buffer.Size()))
					return HR_ERROR;
			}
		}
	

	return HR_OK;
}

EnHandleResult HPClient::OnClose(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
{
	Call_���������Ϣ("�ͻ��˶Ͽ����� ID:%d ������:%d \n", dwConnID, iErrorCode);
	return HR_OK;
}


BOOL HPClient::���ӷ�����(TCHAR *ip, USHORT port)
{
	m_����Ϣ.����();
	if (m_Client->GetState()== SS_STOPPED)
	{
		if (m_Client->Start(IP, PORT))
		{
			Call_���������Ϣ("�ͻ������ӳɹ� \n");
		}
		else
		{
			Call_���������Ϣ("�ͻ�������ʧ�� ������:%d \n", GetLastError());
			return FALSE;

		}

		
	}
	return TRUE;
}

BOOL HPClient::�Ͽ�����()
{
	if (m_Client->Stop())
	{
		Call_���������Ϣ("�Ͽ�����CONNID:%d \n", m_Client->GetConnectionID());
	}
	else 
	{
		ASSERT("�Ͽ�����ʧ��");
		return FALSE;


	}
	return TRUE;
}

void HPClient::������(DWORD id, CBufferPtr pBuffer)
{
	switch (id)
	{
	case WM_��¼��Ϣ:
		break;
	default:
		break;
	}
}

BOOL HPClient::������Ϣ()
{
	static DWORD SEQ = 0;

	USES_CONVERSION;

	CString strContent="test";
	//m_Content.GetWindowText(strContent);

	LPCSTR name = "heart";
	LPCSTR desc = (LPCTSTR)strContent;
	int desc_len = (int)strlen(desc) + 1;
	int body_len = sizeof(������)+ 1;

	������* pBody = (������*)malloc(body_len);
	//�� pBody ָ����ڴ���ǰ body_len �ֽڶ�����Ϊ�㡣
	memset(pBody, 0, body_len);
	ZeroMemory(pBody, body_len+1);

	pBody->age = 23;
	strcpy(pBody->name, name);
	strcpy(pBody->desc, desc);

	��ͷ header;
	header.packageId = ++SEQ;
	header.length = body_len;

	WSABUF bufs[2];
	bufs[0].len = sizeof(��ͷ);
	bufs[0].buf = (char*)&header;
	bufs[1].len = body_len;
	bufs[1].buf = (char*)pBody;

	if (m_Client->SendPackets(bufs, 2))
	{
		Call_���������Ϣ("���ͳɹ� \n");
		return TRUE;
	}
		//::LogSend(m_Client->GetConnectionID(), strContent);
	else
	{
		Call_���������Ϣ("����ʧ�� error %d\n" ,GetLastError());
		return FALSE;

	}
		//::LogSendFail(m_Client->GetConnectionID(), ::SYS_GetLastError(), ::HP_GetSocketErrorDesc(SE_DATA_SEND));
	return TRUE;
}

����Ϣ* HPClient::�ҵ�����Ϣͨ��CID(CONNID dwConnID)
{
	return NULL;
}

void HPClient::ɾ������Ϣͨ��CID(CONNID dwConnID)
{

}

