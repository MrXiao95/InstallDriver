
// InstallDriverDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include <winsvc.h> 


// CInstallDriverDlg 对话框
class CInstallDriverDlg : public CDialogEx
{
// 构造
public:
	CInstallDriverDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_INSTALLDRIVER_DIALOG };

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
	afx_msg void OnBnClickedBtnInstall();
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnUninstall();
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnOpenfile();
private:
	void SetStatusInfo(CString& strInfo);
	void SetErrorInfo(DWORD dwErr);
private:
	CButton m_checkStick;//置顶
	CString m_strDriverPath;// 驱动文件的路径
	CToolTipCtrl m_tooltip;
	CEdit m_editDriverPath;
	SC_HANDLE m_hServiceMgr;//SCM管理器的句柄
	SC_HANDLE m_hServiceDDK;//NT驱动程序的服务句柄
	CString m_strStatus;//程序运行状态
	CString strSymName;
public:
	afx_msg void OnBnClickedCheckStick();
};
