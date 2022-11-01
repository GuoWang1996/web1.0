
// Web01_ClientDlg.h: 头文件
//

#pragma once


// CWeb01ClientDlg 对话框
class CWeb01ClientDlg : public CDialogEx
{
// 构造
public:
	CWeb01ClientDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WEB01_CLIENT_DIALOG };
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
	BOOL ConnectServer();//连接服务器
	SOCKET socketServer;
	BOOL isInit;//服务器连接状态
	afx_msg void OnDestroy();
	CString m_editStatus;
	CString m_editContext;
	afx_msg void OnBnClickedButton2();
};
