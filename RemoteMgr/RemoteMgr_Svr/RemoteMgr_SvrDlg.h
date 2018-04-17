// RemoteMgr_SvrDlg.h : header file
//

#if !defined(AFX_REMOTEMGR_SVRDLG_H__25E34BF6_B313_4979_B18D_995A2B5F2F18__INCLUDED_)
#define AFX_REMOTEMGR_SVRDLG_H__25E34BF6_B313_4979_B18D_995A2B5F2F18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyCMD_srvDlg.h"
#include "ShowCltScreen.h"
#include <AFXTEMPL.H>
/////////////////////////////////////////////////////////////////////////////
// CRemoteMgr_SvrDlg dialog


class CRemoteMgr_SvrDlg : public CDialog
{
// Construction
public:
	CRemoteMgr_SvrDlg(CWnd* pParent = NULL);	// standard constructor

public:
    static UINT AcceptThread(LPVOID lParam);
    static UINT RecvThread(LPVOID lParam);
    BOOL GetClientScreen(SOCKET sClient, char* pszBuf, int nBufLength);
    BOOL GetClientCmd(SOCKET sClient, char* pszBuf, int nBufLength);
    BOOL RemoveOfflineClt(SOCKET sClient);
    
    static CRemoteMgr_SvrDlg *m_pMainDlg;
    //static CMyCMD_srvDlg *m_pCmdDlg; 

    //ͨ��HWND��socket
    CMap<HWND, HWND, SOCKET, SOCKET> m_mapHwndWithsClt;

    //ͨ��socket��cmd����,delete��
    CMap<SOCKET, SOCKET, CMyCMD_srvDlg*, CMyCMD_srvDlg*> m_mapsCltWithCmdDlg;

    //ͨ��socket����Ļ����ָ��,delete��
    CMap<SOCKET, SOCKET, CShowCltScreen*, CShowCltScreen*> m_mapsCltWithScrnDlg;

    BOOL InitSocekt();
    SOCKET m_sockListen;

    //CList<SOCKET, SOCKET> m_LstCltSock;
    //SOCKET m_sClient;

    //���߿ͻ����б�    
    BOOL InitListCtrl();
    //���߿ͻ�������
    int m_nCltCount;

    //���ﲻ��!!���ܰ�socket��IP!!��Ϊͬһ̨�����������ж���ͻ���!!
    //����IP����Ψһ��,�˿ڲ���Ψһ��!!����Ҫ�󶨶˿�!!
    //ѡ���б��IP
    //CString m_csSelIP;
    static SOCKET m_SelSocket;
    
// Dialog Data
	//{{AFX_DATA(CRemoteMgr_SvrDlg)
	enum { IDD = IDD_REMOTEMGR_SVR_DIALOG };
	CListCtrl	m_CltLstCtrl;
	CIPAddressCtrl	m_IP;
	DWORD	m_dwPort;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRemoteMgr_SvrDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CRemoteMgr_SvrDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStart();
	afx_msg BOOL OnGetscreen();
	afx_msg BOOL OnCmd();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnItemchangedListclt(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnCleanSubDlg(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REMOTEMGR_SVRDLG_H__25E34BF6_B313_4979_B18D_995A2B5F2F18__INCLUDED_)
