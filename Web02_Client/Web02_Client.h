
// Web02_Client.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// 主符号


// CWeb02ClientApp:
// 有关此类的实现，请参阅 Web02_Client.cpp
//

class CWeb02ClientApp : public CWinApp
{
public:
	CWeb02ClientApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CWeb02ClientApp theApp;
