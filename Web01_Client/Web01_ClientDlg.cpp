
// Web01_ClientDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Web01_Client.h"
#include "Web01_ClientDlg.h"
#include "afxdialogex.h"
#define  PORT 5698
#define  IP "112.22.170.88" 
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
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

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CWeb01ClientDlg 对话框



CWeb01ClientDlg::CWeb01ClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WEB01_CLIENT_DIALOG, pParent)
	,socketServer(INVALID_SOCKET)
	,isInit(FALSE)
	, m_editStatus(_T(""))
	, m_editContext(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWeb01ClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC1, m_editStatus);
	DDX_Text(pDX, IDC_EDIT1, m_editContext);
}

BEGIN_MESSAGE_MAP(CWeb01ClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CWeb01ClientDlg::OnBnClickedButton1)
//	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON2, &CWeb01ClientDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CWeb01ClientDlg 消息处理程序

BOOL CWeb01ClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWeb01ClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWeb01ClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CWeb01ClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


unsigned __stdcall MyBeginthreadex(void* lpParam) {
	
	CWeb01ClientDlg *client = (CWeb01ClientDlg*)lpParam;
	if (client == NULL) return -1;
	if (client->socketServer== INVALID_SOCKET) return -1;
	TCHAR recvbuf[1024] = { 0 };
	while (1)
	{
		int ret = recv(client->socketServer, recvbuf, 1024 - sizeof(TCHAR), 0);
		switch (ret)
		{
		case 0:
			Call_输出调试信息("客户端断开链接 error = %d\n", WSAGetLastError());
			break;
			//return -2; //不用return 因为还需要继续等待链接
		case SOCKET_ERROR:
			Call_输出调试信息("客户端接收数据失败 error = %d\n",WSAGetLastError());
			break;
			//return -3;
		default:
			Call_输出调试信息("客户端接收数据成功:  %s\n", recvbuf);
		}

	
	}
	if (client)
	{
		delete client;
		client = NULL;
	}
	return -5;
};
//连接服务器
void CWeb01ClientDlg::OnBnClickedButton1()
{
	if (socketServer!= INVALID_SOCKET)
	{
		m_editStatus = "请勿重复连接!";
		return;
	}
	else
	{
		if (ConnectServer()==TRUE)
		{
			_beginthreadex(NULL, NULL, MyBeginthreadex, this, 0, NULL);
			m_editStatus = "连接服务器成功!";
		}
		else
		{
			m_editStatus = "连接服务器失败!";
		}
		
	}
	UpdateData(TRUE);

}

BOOL CWeb01ClientDlg::ConnectServer()
{
	//1.初始化网络环境
	WORD version = MAKEWORD(2, 2);
	WSADATA data = { 0 };
	int isTrue = WSAStartup(version, &data);//通过进程启动 Winsock DLL 的使用。
	if (isTrue != 0)
	{
		Call_输出调试信息("WSAStartup启动失败 error = %d\n", WSAGetLastError());
		return FALSE;
	}
	Call_输出调试信息("WSAStartup启动成功\n");
	//2.创建socket
	socketServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socketServer == INVALID_SOCKET)
	{
		Call_输出调试信息("socket 创建失败 error = %d\n", WSAGetLastError());
		return FALSE;
	}
	Call_输出调试信息("创建socket成功\n");
	//3.连接IP 端口
	SOCKADDR_IN sinServer;
	sinServer.sin_family = AF_INET;
	sinServer.sin_addr.s_addr = inet_addr(IP);
	sinServer.sin_port = htons(PORT); //大端
	if (connect(socketServer, (SOCKADDR*)&sinServer, sizeof(SOCKADDR_IN))== SOCKET_ERROR)
	{
		Call_输出调试信息("socket 连接失败 error = %d\n", WSAGetLastError());
		return FALSE;

	}
	Call_输出调试信息("绑定服务器成功\n");
	return TRUE;
}

void CWeb01ClientDlg::OnDestroy()
{
	if (socketServer)
	{
		shutdown(socketServer, 2);
		closesocket(socketServer);
		WSACleanup();
		Call_输出调试信息("客户端退出清理结束---------\n");
	}
	
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}


void CWeb01ClientDlg::OnBnClickedButton2()
{
	UpdateData(TRUE);
	int recvbuflenSend = 512;
	int retSend = send(socketServer, m_editContext.GetBuffer(), m_editContext.GetLength()+1, 0);
	if (retSend== SOCKET_ERROR)
	{
		Call_输出调试信息("客户端发送消息失败---------\n");
	}
	else
	{
		Call_输出调试信息("客户端发送消息成功---------\n");
	}
	
	UpdateData(FALSE);
}
