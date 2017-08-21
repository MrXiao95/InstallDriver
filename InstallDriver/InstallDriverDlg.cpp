
// InstallDriverDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "InstallDriver.h"
#include "InstallDriverDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CInstallDriverDlg 对话框




CInstallDriverDlg::CInstallDriverDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInstallDriverDlg::IDD, pParent)
	, m_strDriverPath(_T(""))
	, m_hServiceMgr(NULL)
	, m_hServiceDDK(NULL)
	, m_strStatus(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInstallDriverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_STICK, m_checkStick);
	DDX_Text(pDX, IDC_EDIT_PATH, m_strDriverPath);
	DDX_Control(pDX, IDC_EDIT_PATH, m_editDriverPath);
	DDX_Text(pDX, IDC_STATIC_STAUS, m_strStatus);
}

BEGIN_MESSAGE_MAP(CInstallDriverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_INSTALL, &CInstallDriverDlg::OnBnClickedBtnInstall)
	ON_BN_CLICKED(IDC_BTN_START, &CInstallDriverDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CInstallDriverDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_UNINSTALL, &CInstallDriverDlg::OnBnClickedBtnUninstall)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CInstallDriverDlg::OnBnClickedBtnClose)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BTN_OPENFILE, &CInstallDriverDlg::OnBnClickedBtnOpenfile)
	ON_BN_CLICKED(IDC_CHECK_STICK, &CInstallDriverDlg::OnBnClickedCheckStick)
END_MESSAGE_MAP()


// CInstallDriverDlg 消息处理程序

BOOL CInstallDriverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	m_tooltip.Create(this);
	m_tooltip.Activate(TRUE);
	SetStatusInfo(CString(L"支持拖拽获取文件路径"));
	
	//默认总在最前
	m_checkStick.PostMessage(BM_CLICK,1);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CInstallDriverDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CInstallDriverDlg::OnPaint()
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
HCURSOR CInstallDriverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//安装驱动
void CInstallDriverDlg::OnBnClickedBtnInstall()
{
	TCHAR szDriverImagePath[MAX_PATH] = {0};
	//得到完整的驱动路径
	GetFullPathName(m_strDriverPath, MAX_PATH, szDriverImagePath, NULL);

	BOOL bRet = FALSE;
	CString strInfo;
	//打开服务控制管理器
	m_hServiceMgr = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );

	if( m_hServiceMgr == NULL )  
	{
		SetErrorInfo(GetLastError());
		bRet = FALSE;
		if(m_hServiceDDK)
		{
			CloseServiceHandle(m_hServiceDDK);
		}
		if(m_hServiceMgr)
		{
			CloseServiceHandle(m_hServiceMgr);
		}
		return ;
	}
	else
	{
		////OpenSCManager成功
		strInfo.Format(L"打开驱动服务成功");
		SetStatusInfo(strInfo);
	}
	strSymName = m_strDriverPath;
	int nPos = strSymName.ReverseFind('\\');
	strSymName = strSymName.Right(strSymName.GetLength() - nPos - 1);
	
	//创建驱动所对应的服务
	m_hServiceDDK = CreateService( m_hServiceMgr,
		strSymName, //驱动程序的在注册表中的名字  
		strSymName, // 注册表驱动程序的 DisplayName 值  
		SERVICE_ALL_ACCESS, // 加载驱动程序的访问权限  
		SERVICE_KERNEL_DRIVER,// 表示加载的服务是驱动程序  
		SERVICE_DEMAND_START, // 注册表驱动程序的 Start 值  
		SERVICE_ERROR_IGNORE, // 注册表驱动程序的 ErrorControl 值  
		szDriverImagePath, // 注册表驱动程序的 ImagePath 值  
		NULL,  
		NULL,  
		NULL,  
		NULL,  
		NULL);  

	DWORD dwRtn;
	//判断服务是否失败
	if( m_hServiceDDK == NULL )  
	{  
		dwRtn = GetLastError();
		if( dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_EXISTS )  
		{  
			SetErrorInfo(GetLastError());

			bRet = FALSE;
			if(m_hServiceDDK)
			{
				CloseServiceHandle(m_hServiceDDK);
			}
			if(m_hServiceMgr)
			{
				CloseServiceHandle(m_hServiceMgr);
			}
		}  
		else  
		{
			//服务创建失败，是由于服务已经创立过
			strInfo.Format(L"创建服务失败,因为已经创建");
			SetStatusInfo(strInfo);
		}

		// 驱动程序已经加载，只需要打开  
		m_hServiceDDK = OpenService( m_hServiceMgr, strSymName, SERVICE_ALL_ACCESS );  
		if( m_hServiceDDK == NULL )  
		{
			//如果打开服务也失败，则意味错误
			SetErrorInfo(GetLastError());

			bRet = FALSE;
			if(m_hServiceDDK)
			{
				CloseServiceHandle(m_hServiceDDK);
			}
			if(m_hServiceMgr)
			{
				CloseServiceHandle(m_hServiceMgr);
			}
		}  
		else 
		{
			strInfo.Format(L"打开驱动服务成功");
			SetStatusInfo(strInfo);
		}
	}  
	else  
	{
		strInfo.Format(L"创建驱动服务成功");
		SetStatusInfo(strInfo);
	}
}

//启动驱动
void CInstallDriverDlg::OnBnClickedBtnStart()
{
	//开启此项服务
	CString strInfo;
	if( StartService( m_hServiceDDK, NULL, NULL ) )  
	{  
		DWORD dwRtn = GetLastError();  
		if( dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_ALREADY_RUNNING )  
		{  
			SetErrorInfo(dwRtn);
		}  
		else  
		{  
			if( dwRtn == ERROR_IO_PENDING )  
			{  
				//设备被挂住
				strInfo.Format(L"启动驱动失败");
				SetStatusInfo(strInfo);
			}  
			else  
			{  
				//服务已经开启
				strInfo.Format(L"服务已经启动",dwRtn);
				SetStatusInfo(strInfo);
			}  
		}  
	}
	else
	{
		strInfo = L"驱动启动成功";
		SetStatusInfo(strInfo);
	}
	if(m_hServiceDDK)
	{
		CloseServiceHandle(m_hServiceDDK);
	}
	if(m_hServiceMgr)
	{
		CloseServiceHandle(m_hServiceMgr);
	}
}

//停止驱动
void CInstallDriverDlg::OnBnClickedBtnStop()
{
	BOOL bRet = FALSE;
	SERVICE_STATUS SvrSta;
	CString strInfo;
	//打开SCM管理器
	m_hServiceMgr = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );  
	if( m_hServiceMgr == NULL )  
	{
		//带开SCM管理器失败
		SetErrorInfo(GetLastError());

		if(m_hServiceMgr)
		{
			CloseServiceHandle(m_hServiceMgr);
		}
		return ;
	}  
	
	//打开驱动所对应的服务
	m_hServiceDDK = OpenService( m_hServiceMgr, strSymName, SERVICE_ALL_ACCESS );  

	if( m_hServiceDDK == NULL )  
	{
		//打开驱动所对应的服务失败 
		SetErrorInfo(GetLastError());

		bRet = FALSE;
		if(m_hServiceMgr)
		{
			CloseServiceHandle(m_hServiceMgr);
		}
	}  

	//停止驱动程序，如果停止失败，只有重新启动才能，再动态加载。  
	if( !ControlService( m_hServiceDDK, SERVICE_CONTROL_STOP , &SvrSta ) )  
	{  
		SetErrorInfo(GetLastError());
	}  
	else
	{
		strInfo.Format(L"停止驱动成功");
		SetStatusInfo(strInfo);
	}
}

//卸载驱动
void CInstallDriverDlg::OnBnClickedBtnUninstall()
{
	CString strInfo;
	//动态卸载驱动程序。  
	if( !DeleteService( m_hServiceDDK ) )  
	{
		//卸载失败
		SetErrorInfo(GetLastError());
	}  
	else  
	{  
		//卸载成功
		strInfo.Format(L"卸载驱动成功");
		SetStatusInfo(strInfo);
	}  
}

//退出程序
void CInstallDriverDlg::OnBnClickedBtnClose()
{
	PostQuitMessage(0);
}

void CInstallDriverDlg::OnDropFiles(HDROP hDropInfo)
{
	int nCount;
	TCHAR szFilePath[MAX_PATH] = { 0 };
	nCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if(nCount)
	{
		for (int i = 0; i < nCount; i++)
		{
			DragQueryFile(hDropInfo, i, szFilePath, sizeof(szFilePath));
			m_strDriverPath = szFilePath;
			break;
		}
		
		UpdateData(FALSE);
		SetStatusInfo(CString(L"拖拽获取文件路径成功"));
	}
	DragFinish(hDropInfo);
	
	CDialogEx::OnDropFiles(hDropInfo);
}


BOOL CInstallDriverDlg::PreTranslateMessage(MSG* pMsg)
{
	if (m_tooltip)
	{
		m_tooltip.RelayEvent(pMsg);
	}
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return FALSE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

//打开驱动文件
void CInstallDriverDlg::OnBnClickedBtnOpenfile()
{
	TCHAR szTmpFileName[MAX_PATH] = {0};
	
	OPENFILENAME openFileName = {0};
	openFileName.lStructSize = sizeof(OPENFILENAME);
	openFileName.lpstrInitialDir = NULL;
	openFileName.nMaxFile = MAX_PATH;
	openFileName.lpstrFilter = L"所有文件(*.*)\0*.*\0";
	openFileName.lpstrFile = szTmpFileName;
	openFileName.nFilterIndex = 1;
	openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	openFileName.lpstrTitle = L"请选文件";

	if (!GetOpenFileName(&openFileName))
	{
		return ;
	}
	
	m_strDriverPath = szTmpFileName;
	UpdateData(FALSE);
} 

void CInstallDriverDlg::SetStatusInfo(CString& strInfo)
{
	m_strStatus = L"状态：" + strInfo;
	UpdateData(FALSE);
}


void CInstallDriverDlg::OnBnClickedCheckStick()
{
	if (m_checkStick.GetCheck())
	{
		SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
	else
	{
		SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
}

void CInstallDriverDlg::SetErrorInfo(DWORD dwErr)
{
	CString strInfo;
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |FORMAT_MESSAGE_FROM_SYSTEM,NULL,dwErr,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR) &lpMsgBuf, 0, NULL );
	strInfo.Format(L"%s",lpMsgBuf);
	SetStatusInfo(strInfo);
}