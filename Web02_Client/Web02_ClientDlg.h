
// Web02_ClientDlg.h: 头文件
//

#pragma once
#include "HPClient.h"

// CWeb02ClientDlg 对话框
class CWeb02ClientDlg : public CDialogEx
{
// 构造
public:
	CWeb02ClientDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WEB02_CLIENT_DIALOG };
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
	CString m_status;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	HPClient hClient;
	void 发送登录消息();
	afx_msg void OnBnClickedButton3();
};
