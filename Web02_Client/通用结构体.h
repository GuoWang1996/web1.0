#pragma once
#include "pch.h"
typedef struct 包头
{
	DWORD packageId;
	int length;//记录下一个包长度
	包头() :packageId(0), length(0) {}
}包头;
typedef struct 包信息
{
	BOOL isHead;//是否包头
	int packageLength;//包长
	DWORD packageId;
	包信息():isHead(TRUE),packageLength(sizeof(包头)), packageId(0)
	{
	}
	void 重置()
	{
		isHead = TRUE;
		packageLength = sizeof(包头);
		packageId = 0;
	}
}包信息;
struct TPkgHeader
{
	DWORD seq;
	int body_len;
};

struct 包身体
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
