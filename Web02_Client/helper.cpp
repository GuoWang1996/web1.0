#include "pch.h"
#include "helper.h"
void Call_输出调试信息(char*pszFormat, ...) {
#ifdef _DEBUG
	char szbufFormat[0x1000];
	char szbufFormat_Game[0x1000] = "debug:";
	va_list argList;
	va_start(argList, pszFormat);//参数列表初始化
	vsprintf_s(szbufFormat, pszFormat, argList);
	strcat_s(szbufFormat_Game, szbufFormat);
	OutputDebugString(szbufFormat_Game);
	va_end(argList);
#endif
}