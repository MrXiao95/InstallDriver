
// InstallDriverDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "InstallDriver.h"
#include "InstallDriverDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CInstallDriverDlg �Ի���




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


// CInstallDriverDlg ��Ϣ�������

BOOL CInstallDriverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	m_tooltip.Create(this);
	m_tooltip.Activate(TRUE);
	SetStatusInfo(CString(L"֧����ק��ȡ�ļ�·��"));
	
	//Ĭ��������ǰ
	m_checkStick.PostMessage(BM_CLICK,1);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CInstallDriverDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CInstallDriverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//��װ����
void CInstallDriverDlg::OnBnClickedBtnInstall()
{
	TCHAR szDriverImagePath[MAX_PATH] = {0};
	//�õ�����������·��
	GetFullPathName(m_strDriverPath, MAX_PATH, szDriverImagePath, NULL);

	BOOL bRet = FALSE;
	CString strInfo;
	//�򿪷�����ƹ�����
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
		////OpenSCManager�ɹ�
		strInfo.Format(L"����������ɹ�");
		SetStatusInfo(strInfo);
	}
	strSymName = m_strDriverPath;
	int nPos = strSymName.ReverseFind('\\');
	strSymName = strSymName.Right(strSymName.GetLength() - nPos - 1);
	
	//������������Ӧ�ķ���
	m_hServiceDDK = CreateService( m_hServiceMgr,
		strSymName, //�����������ע����е�����  
		strSymName, // ע������������ DisplayName ֵ  
		SERVICE_ALL_ACCESS, // ������������ķ���Ȩ��  
		SERVICE_KERNEL_DRIVER,// ��ʾ���صķ�������������  
		SERVICE_DEMAND_START, // ע������������ Start ֵ  
		SERVICE_ERROR_IGNORE, // ע������������ ErrorControl ֵ  
		szDriverImagePath, // ע������������ ImagePath ֵ  
		NULL,  
		NULL,  
		NULL,  
		NULL,  
		NULL);  

	DWORD dwRtn;
	//�жϷ����Ƿ�ʧ��
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
			//���񴴽�ʧ�ܣ������ڷ����Ѿ�������
			strInfo.Format(L"��������ʧ��,��Ϊ�Ѿ�����");
			SetStatusInfo(strInfo);
		}

		// ���������Ѿ����أ�ֻ��Ҫ��  
		m_hServiceDDK = OpenService( m_hServiceMgr, strSymName, SERVICE_ALL_ACCESS );  
		if( m_hServiceDDK == NULL )  
		{
			//����򿪷���Ҳʧ�ܣ�����ζ����
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
			strInfo.Format(L"����������ɹ�");
			SetStatusInfo(strInfo);
		}
	}  
	else  
	{
		strInfo.Format(L"������������ɹ�");
		SetStatusInfo(strInfo);
	}
}

//��������
void CInstallDriverDlg::OnBnClickedBtnStart()
{
	//�����������
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
				//�豸����ס
				strInfo.Format(L"��������ʧ��");
				SetStatusInfo(strInfo);
			}  
			else  
			{  
				//�����Ѿ�����
				strInfo.Format(L"�����Ѿ�����",dwRtn);
				SetStatusInfo(strInfo);
			}  
		}  
	}
	else
	{
		strInfo = L"���������ɹ�";
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

//ֹͣ����
void CInstallDriverDlg::OnBnClickedBtnStop()
{
	BOOL bRet = FALSE;
	SERVICE_STATUS SvrSta;
	CString strInfo;
	//��SCM������
	m_hServiceMgr = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );  
	if( m_hServiceMgr == NULL )  
	{
		//����SCM������ʧ��
		SetErrorInfo(GetLastError());

		if(m_hServiceMgr)
		{
			CloseServiceHandle(m_hServiceMgr);
		}
		return ;
	}  
	
	//����������Ӧ�ķ���
	m_hServiceDDK = OpenService( m_hServiceMgr, strSymName, SERVICE_ALL_ACCESS );  

	if( m_hServiceDDK == NULL )  
	{
		//����������Ӧ�ķ���ʧ�� 
		SetErrorInfo(GetLastError());

		bRet = FALSE;
		if(m_hServiceMgr)
		{
			CloseServiceHandle(m_hServiceMgr);
		}
	}  

	//ֹͣ�����������ֹͣʧ�ܣ�ֻ�������������ܣ��ٶ�̬���ء�  
	if( !ControlService( m_hServiceDDK, SERVICE_CONTROL_STOP , &SvrSta ) )  
	{  
		SetErrorInfo(GetLastError());
	}  
	else
	{
		strInfo.Format(L"ֹͣ�����ɹ�");
		SetStatusInfo(strInfo);
	}
}

//ж������
void CInstallDriverDlg::OnBnClickedBtnUninstall()
{
	CString strInfo;
	//��̬ж����������  
	if( !DeleteService( m_hServiceDDK ) )  
	{
		//ж��ʧ��
		SetErrorInfo(GetLastError());
	}  
	else  
	{  
		//ж�سɹ�
		strInfo.Format(L"ж�������ɹ�");
		SetStatusInfo(strInfo);
	}  
}

//�˳�����
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
		SetStatusInfo(CString(L"��ק��ȡ�ļ�·���ɹ�"));
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

//�������ļ�
void CInstallDriverDlg::OnBnClickedBtnOpenfile()
{
	TCHAR szTmpFileName[MAX_PATH] = {0};
	
	OPENFILENAME openFileName = {0};
	openFileName.lStructSize = sizeof(OPENFILENAME);
	openFileName.lpstrInitialDir = NULL;
	openFileName.nMaxFile = MAX_PATH;
	openFileName.lpstrFilter = L"�����ļ�(*.*)\0*.*\0";
	openFileName.lpstrFile = szTmpFileName;
	openFileName.nFilterIndex = 1;
	openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	openFileName.lpstrTitle = L"��ѡ�ļ�";

	if (!GetOpenFileName(&openFileName))
	{
		return ;
	}
	
	m_strDriverPath = szTmpFileName;
	UpdateData(FALSE);
} 

void CInstallDriverDlg::SetStatusInfo(CString& strInfo)
{
	m_strStatus = L"״̬��" + strInfo;
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