
// Web02Dlg.h: 头文件
//

#pragma once
#include "HPServer.h"

// CWeb02Dlg 对话框
class CWeb02Dlg : public CDialogEx
{
// 构造
public:
	CWeb02Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WEB02_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	HPServer hpServer;
	afx_msg void OnBnClickedButton2();
	CString m_status;
	CString m_name;
};
