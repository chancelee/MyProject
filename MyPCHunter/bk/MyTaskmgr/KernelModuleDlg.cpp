// KernelModuleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyTaskmgr.h"
#include "KernelModuleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKernelModuleDlg dialog


CKernelModuleDlg::CKernelModuleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKernelModuleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKernelModuleDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CKernelModuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKernelModuleDlg)
	DDX_Control(pDX, LST_MODULE, m_LstModule);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKernelModuleDlg, CDialog)
	//{{AFX_MSG_MAP(CKernelModuleDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKernelModuleDlg message handlers

BOOL CKernelModuleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    //����ListCtrl���
    m_LstModule.SetExtendedStyle(m_LstModule.GetExtendedStyle() |
                                LVS_EX_GRIDLINES |
                                LVS_EX_FULLROWSELECT);
    //����ListCtrl����
    m_LstModule.InsertColumn(1, "������", LVCFMT_LEFT, 70);
    m_LstModule.InsertColumn(2, "����ַ", LVCFMT_LEFT, 70);
    m_LstModule.InsertColumn(3, "��С", LVCFMT_LEFT, 70);
    m_LstModule.InsertColumn(4, "��������", LVCFMT_LEFT, 70);
    m_LstModule.InsertColumn(5, "����·��", LVCFMT_LEFT, 200);
    m_LstModule.InsertColumn(6, "������", LVCFMT_LEFT, 70);
    m_LstModule.InsertColumn(7, "����˳��", LVCFMT_LEFT, 60);
    //m_LstModule.InsertColumn(8, "�ļ�����", LVCFMT_LEFT, 200);
    
    UpdateData(FALSE);

    GetKrnlModuleFromSys();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

DWORD CKernelModuleDlg::GetKrnlModuleFromSys()
{
    return TRUE;
}
