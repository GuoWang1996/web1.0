#pragma once
#include "pch.h"
typedef struct ��ͷ
{
	DWORD packageId;
	int length;//��¼��һ��������
	��ͷ() :packageId(0), length(0) {}
}��ͷ;
typedef struct ����Ϣ
{
	BOOL isHead;//�Ƿ��ͷ
	int packageLength;//����
	DWORD packageId;
	����Ϣ():isHead(TRUE),packageLength(sizeof(��ͷ)), packageId(0)
	{
	}
	void ����()
	{
		isHead = TRUE;
		packageLength = sizeof(��ͷ);
		packageId = 0;
	}
}����Ϣ;
struct TPkgHeader
{
	DWORD seq;
	int body_len;
};

struct ������
{
	char name[30];
	short age;
	char desc[10];
};

struct TPkgInfo
{
	bool is_header;
	int length;

	TPkgInfo(bool header = true, int len = sizeof(TPkgHeader)) : is_header(header), length(len) {}
	void Reset() { is_header = true, length = sizeof(TPkgHeader); }
	~TPkgInfo() {}
};
