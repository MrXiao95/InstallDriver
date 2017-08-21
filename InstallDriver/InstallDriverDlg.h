
// InstallDriverDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include <winsvc.h> 


// CInstallDriverDlg �Ի���
class CInstallDriverDlg : public CDialogEx
{
// ����
public:
	CInstallDriverDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_INSTALLDRIVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	CButton m_checkStick;//�ö�
	CString m_strDriverPath;// �����ļ���·��
	CToolTipCtrl m_tooltip;
	CEdit m_editDriverPath;
	SC_HANDLE m_hServiceMgr;//SCM�������ľ��
	SC_HANDLE m_hServiceDDK;//NT��������ķ�����
	CString m_strStatus;//��������״̬
	CString strSymName;
public:
	afx_msg void OnBnClickedCheckStick();
};
