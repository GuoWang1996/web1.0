#include "pch.h"
#include "HPServer.h"
#include "afxdialogex.h"
HPServer::HPServer() : m_Server(this)
{
}

//�󶨼�����ַǰ���� ��server����м������
EnHandleResult HPServer::OnPrepareListen(ITcpServer* pSender, SOCKET soListen)
{
	TCHAR szAddress[100];//���ؼ���ip��ַ
	int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);//��ַ������
	USHORT usPort;

	pSender->GetListenAddress(szAddress, iAddressLen, usPort);
	return HR_OK;
}
//�ͻ����������󵽴�ʱ���� �Զ���ȡ�����б�ֵ
EnHandleResult HPServer::OnAccept(ITcpServer* pSender, CONNID dwConnID, UINT_PTR soClient)
{
		BOOL bPass = TRUE;
	TCHAR szAddress[100];//�ͻ���ip��ַ������
	int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);//�ͻ���ip��ַ��������С
	USHORT usPort;

	pSender->GetRemoteAddress(dwConnID, szAddress, iAddressLen, usPort);
	Call_���������Ϣ("���յ��ͻ������� ID:%d IP��%s,�˿�:%d,��ַ����:%d \n", dwConnID, szAddress, usPort, iAddressLen);

	//�󶨰���Ϣ
	if(bPass) pSender->SetConnectionExtra(dwConnID, new ����Ϣ());
	return bPass ? HR_OK : HR_ERROR;
}
//���ݷ��ͳɹ�����
EnHandleResult HPServer::OnSend(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
	Call_���������Ϣ("������send����------------------\n");
	return HR_OK;
}
//���յ�����ʱ����
EnHandleResult HPServer::OnReceive(ITcpServer* pSender, CONNID dwConnID, int iLength)
{ 
	����Ϣ* pInfo = �ҵ�����Ϣͨ��CID(pSender, dwConnID);

	//����ǿתΪ����
	ITcpPullServer* pServer = ITcpPullServer::FromS(pSender);

	if (pInfo != nullptr)
	{
		int required = pInfo->packageLength;//��ʼ��ͷ���� 8
		int ���հ��� = iLength;//���հ���

		//������հ���С�ڰ�ͷ�� ���Ա����¼� �� ���հ��� ���ڻ���ڵ�ǰ�����ĳ���ʱ��ѭ�����������
		//Fetch(dwConnID, pData, iDataLength) ��������Ҫ��������ȡ������ֱ��ʣ������ݳ���С�ڵ�ǰ�����ĳ��ȡ�
		while (���հ��� >= required)
		{
			���հ��� -= required;//���հ���-��ͷ��=�����峤
			
			CBufferPtr buffer(required);

			EnFetchResult result = pServer->Fetch(dwConnID, buffer, (int)buffer.Size());
			if (result == FR_OK)
			{
				if (pInfo->isHead)
				{
					��ͷ* pHeader = (��ͷ*)buffer.Ptr();
					Call_���������Ϣ("[Server] ��ͷ -> id: %d, ����: %d\n", pHeader->packageId, pHeader->length);

					required = pHeader->length;
				}
				else
				{
					������* pBody = (������*)(BYTE*)buffer;
					Call_���������Ϣ("[Server] ������ -> name: %s, age: %d, desc: %s\n", pBody->name, pBody->age, pBody->desc);

					required = sizeof(��ͷ);
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
//�ͻ��˶Ͽ����Ӵ��� ��������ͻ��˾��������м������DisConnect����
EnHandleResult HPServer::OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
{
	ɾ������Ϣͨ��CID(pSender, dwConnID);
	Call_���������Ϣ("�ͻ��˶Ͽ����� ID:%d ������:%d \n", dwConnID, iErrorCode);
	return HR_OK;
}

EnHandleResult HPServer::OnShutdown(ITcpServer* pSender)
{
	return HR_OK;
}

����Ϣ* HPServer::�ҵ�����Ϣͨ��CID(ITcpServer* pSender,CONNID dwConnID)
{
	PVOID pInfo = nullptr;

	pSender->GetConnectionExtra(dwConnID, &pInfo);

	return (����Ϣ*)pInfo;
}

void  HPServer::ɾ������Ϣͨ��CID(ITcpServer* pSender,CONNID dwConnID)
{
	����Ϣ* pInfo = �ҵ�����Ϣͨ��CID(pSender, dwConnID);
	ASSERT(pInfo != nullptr);

	delete pInfo;
	pInfo = NULL;
}

BOOL HPServer::����������(TCHAR *ip, USHORT port)
{
	if (m_Server->Start(ip, port))
	{
		Call_���������Ϣ("�����������ɹ�\n");
		return TRUE;
	}
	else
	{
		Call_���������Ϣ("����������ʧ�� error:%d\n",GetLastError());
		return FALSE;
	}
}

BOOL HPServer::ֹͣ������()
{
	if (m_Server->Stop())
	{
		Call_���������Ϣ("debug:ֹͣ�������ɹ�\n");
		return TRUE;
	}
	else
	{
		Call_���������Ϣ("debug:ֹͣ������ʧ�� error:%d\n", GetLastError());
		ASSERT(FALSE);
		return FALSE;
	}
}

void HPServer::�ر�����(ITcpServer* pSender,CONNID id)
{
	if (m_Server->Disconnect(id))
	{
		Call_���������Ϣ("�ͻ��˶Ͽ����ӳɹ�\n");
	}
	else
	{
		Call_���������Ϣ("�ͻ��˶Ͽ�����ʧ�� error:%d\n", GetLastError());

	}
}

void HPServer::������(CONNID dwConnID, DWORD id, CBufferPtr pBuffer)
{
	switch (id)
	{
	case WM_��¼��Ϣ:
		//m_name = "";
		break;
	default:
		break;
	}
}

