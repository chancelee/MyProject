// MyCFFDlg.h : header file
//

#if !defined(AFX_MYCFFDLG_H__D0B7EE0D_2363_4A78_94BA_19CE5D730369__INCLUDED_)
#define AFX_MYCFFDLG_H__D0B7EE0D_2363_4A78_94BA_19CE5D730369__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMyCFFDlg dialog

class CMyCFFDlg : public CDialog
{
// Construction
public:
	CMyCFFDlg(CWnd* pParent = NULL);	// standard constructor
    virtual void OnOK()
    {
        return;
    }

    void InitControls(char *szExe);
    BOOL IsPEFile(char *szPath);
    void ShowDosHeader(CListCtrl& listCtrl);
    void ShowNTHeader(CListCtrl& listCtrl);
    void ShowFileHeader(CListCtrl& listCtrl);
    void ShowOptionalHeader(CListCtrl& listCtrl);
    void ShowDataDir(CListCtrl& listCtrl);
    void ShowSectionHeader(CListCtrl& listCtrl);
    void ShowIAT(CListCtrl& listCtrl);
    void ShowImportDirectory(CListCtrl& listCtrl);
    void ShowExportDirectory(CListCtrl& listCtrl);
    void ShowRelocationDirectory(CListCtrl& listCtrl);
    void ShowResourceDirectory(CListCtrl& listCtrl);
    

    char* GetSize(DWORD dwSize);
    char* GetHexFmt(DWORD dwSize);

    //RVA ת FA �����ڲ��� �����Ϸ��� ��Ĭ��PE�еĵ�ַ����ȷ
    BOOL RVA2FA(DWORD dwRVA, DWORD& dwFA);

    // PE����ʼλ��
    DWORD m_e_lfanew;

    //PE �ļ���С
    DWORD m_dwFileSize;
    DWORD m_dwHighSize;

    //�ڴ������ļ�����
    DWORD m_SectionAlignment;
    DWORD m_FileAlignment;

    // ��¼NumberOfSections��ֵ
    DWORD m_dwNumberOfSections;

    // ��¼NumberOfRvaAndSizes ����Ŀ¼��С
    DWORD m_NumberOfRvaAndSizes;

    // ͷ���ܴ�С
    DWORD m_SizeOfHeaders;

    // ���������ַ
    DWORD m_ImageBase;
    
    //���򿪵�exe�ļ��Ļ���
    char *m_pExeFileBuf;
    
    //�����ļ��ľ��
    HANDLE m_hExeFile;

    //listCtr�ڴ��ڵ�����
    CRect m_rcListCtrl;

    //һ��CtrlList����������һ��
    CListCtrl m_lstExtend;

// Dialog Data
	//{{AFX_DATA(CMyCFFDlg)
	enum { IDD = IDD_MYCFF_DIALOG };
	CListCtrl	m_LstMemberOfStruct;
	CTreeCtrl	m_Tree;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyCFFDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMyCFFDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOpenexe();
	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocusVa();
	afx_msg void OnSetfocusRva();
	afx_msg void OnSetfocusFileoffset();
	afx_msg void OnChangeVa();
	afx_msg void OnChangeRva();
	afx_msg void OnChangeFileoffset();
	afx_msg void OnClickStruct(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCFFDLG_H__D0B7EE0D_2363_4A78_94BA_19CE5D730369__INCLUDED_)
