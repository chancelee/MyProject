#if !defined(AFX_DIALOGREG_H__E9BF9A06_72E4_4007_AF61_10C6E4604D9C__INCLUDED_)
#define AFX_DIALOGREG_H__E9BF9A06_72E4_4007_AF61_10C6E4604D9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogReg.h : header file
//
#include "MyRegTree.h"
/////////////////////////////////////////////////////////////////////////////
// CDialogReg dialog

class CDialogReg : public CDialog
{
// Construction
public:
	CDialogReg(CWnd* pParent = NULL);   // standard constructor

public:
    //��ȡ�ӽڵ���
    LONG GetSubKeyName(HKEY hKey, int nIndex, char *szKeyName, DWORD *pdwSize);
    //������ڵ�
    void RegTreeInstNode(HKEY hKey, CTreeCtrl& TreeNode, HTREEITEM hNode);
    //���������������Զ������п�
    void AutoSet(CListCtrl& ListCtl);
// Dialog Data
	//{{AFX_DATA(CDialogReg)
	enum { IDD = DLG_REGEDIT };
	CMyRegTree	m_TreeNode;
	CListCtrl	m_LstValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogReg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogReg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGREG_H__E9BF9A06_72E4_4007_AF61_10C6E4604D9C__INCLUDED_)
