#include "pch.h"
#include "helper.h"
void Call_���������Ϣ(char*pszFormat, ...) {
#ifdef _DEBUG
	char szbufFormat[0x1000];
	char szbufFormat_Game[0x1000] = "debug:";
	va_list argList;
	va_start(argList, pszFormat);//�����б��ʼ��
	vsprintf_s(szbufFormat, pszFormat, argList);
	strcat_s(szbufFormat_Game, szbufFormat);
	OutputDebugString(szbufFormat_Game);
	va_end(argList);
#endif
}