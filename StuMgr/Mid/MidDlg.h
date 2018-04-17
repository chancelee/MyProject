// MidDlg.h : header file
//

#if !defined(AFX_MIDDLG_H__518A84D1_BE1E_4231_8981_F88B2EC9B255__INCLUDED_)
#define AFX_MIDDLG_H__518A84D1_BE1E_4231_8981_F88B2EC9B255__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\common\ThreadPool.h"

/////////////////////////////////////////////////////////////////////////////
// CMidDlg dialog

class CMidDlg : public CDialog
{
// Construction
public:
	CMidDlg(CWnd* pParent = NULL);	// standard constructor
    virtual void OnOK();

    //��ʼ���ͻ����б�
    BOOL InitListCtrl();

    //���߿ͻ�������
    int m_nCltCount;

    //�ͻ����б��������ߵĿͻ���
    BOOL RemoveOfflineClt(SOCKET sClient);

    //����ͻ��˵Ĳ�ѯ���
    BOOL RecvCltSQL(SOCKET sClient, char* pszBuf, int nBufLength, int nSQLType);

    //�������˵ķ��صĽ��
    BOOL GetSrvSelectSQL(SOCKET sClient, char* pszBuf, int nBufLength);

    //����SOCKET
    SOCKET m_sockListen;

    //��ת���ӷ���˵�SOCKET
    SOCKET m_sockMid;

    //�̳߳�
    CThreadPool m_ThreadPool;

    //�����������صĲ�ѯ�������Ϊ����
    void SaveCache(char *pFimeName, char* pszBuf, int nBufSize);

// Dialog Data
	//{{AFX_DATA(CMidDlg)
	enum { IDD = IDD_MID_DIALOG };
	CListCtrl	m_LstClt;
	CIPAddressCtrl	m_IP;
	DWORD	m_dwMidPort;
	DWORD	m_dwSrvPort;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMidDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMidDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStart();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIDDLG_H__518A84D1_BE1E_4231_8981_F88B2EC9B255__INCLUDED_)
