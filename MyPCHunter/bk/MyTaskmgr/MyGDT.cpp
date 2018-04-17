// MyGDT.cpp : implementation file
//

#include "stdafx.h"
#include "MyTaskmgr.h"
#include "MyGDT.h"
#include "Function.h"
#include "MyDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyGDT dialog


CMyGDT::CMyGDT(CWnd* pParent /*=NULL*/)
	: CDialog(CMyGDT::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyGDT)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMyGDT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyGDT)
	DDX_Control(pDX, LST_GDT, m_LstGDT);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyGDT, CDialog)
	//{{AFX_MSG_MAP(CMyGDT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyGDT message handlers

BOOL CMyGDT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    //����ListCtrl���
    m_LstGDT.SetExtendedStyle(m_LstGDT.GetExtendedStyle() |
                              LVS_EX_GRIDLINES |
                              LVS_EX_FULLROWSELECT);
    //����ListCtrl����
    m_LstGDT.InsertColumn(1, "CPU���", LVCFMT_LEFT, 60);
    m_LstGDT.InsertColumn(2, "��ѡ����", LVCFMT_LEFT, 60);
    m_LstGDT.InsertColumn(3, "��ַ", LVCFMT_LEFT, 200);
    m_LstGDT.InsertColumn(4, "����", LVCFMT_LEFT, 200);
    m_LstGDT.InsertColumn(5, "������", LVCFMT_LEFT, 60);
    m_LstGDT.InsertColumn(6, "����Ȩ��", LVCFMT_LEFT, 60);
    m_LstGDT.InsertColumn(7, "����", LVCFMT_LEFT, 100);
    
    UpdateData(FALSE);

    GetGDTFromSys();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

DWORD CMyGDT::GetGDTFromSys()
{
    char szBuf[MAXBYTE] = {0};

    //���CPU������
    DWORD dwNumberOfProcessors = CFunction::GetNumberOfProcessors();

    DWORD dwBytes = 0;
    //��������ȡGDT���� ����8�õ�GDT����

    DWORD dwMap = 0; //GDT������
    DWORD dwGDTL = 0; //GDT����
    
    extern CMyTaskmgrApp theApp;
    HANDLE hDrvFile = theApp.m_hDrvFile;
    if (!DeviceIoControl(hDrvFile, 
                        IOCTRL_GET_GDTLIMIT, 
                        (LPVOID)&dwGDTL, 
                        sizeof(DWORD),
                        (LPVOID)&dwGDTL,
                        sizeof(DWORD),
                        &dwBytes, 
                        NULL))
    {
        //CloseHandle(hDrvFile);
        wsprintf(szBuf, "DeviceIoControl IOCTRL_GET_GDTLIMIT ErrorCode:%d\n", GetLastError());
        AfxMessageBox(szBuf);
        return 0;
    }

    dwGDTL = dwGDTL + 1; //��0��ʼ�������Լ�1
    dwMap = dwGDTL / 8;

    //GDT������������� ������ x dwLimit �ֽ�
    //���봫������pParams����ڴ�
    char *pParams = new char[dwNumberOfProcessors * dwGDTL];
    if (pParams == NULL)
    {
        wsprintf(szBuf, "new CONTORL_PARAMS faild!");
        AfxMessageBox(szBuf);
        return FALSE;
    }
    ZeroMemory(pParams, dwNumberOfProcessors * dwGDTL);
    //���ô���������
    ((PCONTORL_PARAMS)pParams)->CPUNumber = dwNumberOfProcessors;
    
    if (!DeviceIoControl(hDrvFile, 
                        IOCTRL_GET_GDT, 
                        pParams, 
                        sizeof(CONTORL_PARAMS),
                        pParams, 
                        dwNumberOfProcessors * dwGDTL, //����ĳ��ȸ������DispatchDeviceControl--IoCompleteRequest(pIrp, IO_NO_INCREMENT)ʱ����
                        &dwBytes, 
                        NULL))
    {
        //CloseHandle(hDrvFile);
        wsprintf(szBuf, "DeviceIoControl IOCTRL_GET_GDT ErrorCode:%d\n", GetLastError());
        AfxMessageBox(szBuf);

        if (pParams != NULL)
        {
            delete pParams;
            pParams = NULL;
        }
        return 0;
    }

    char szTmpBuf[MAXBYTE] = {0};
    int j = 0;
    int k = 0;
    WORD wAttrib = 0;
    DWORD dwLimite = 0;
    DWORD dwBase = 0;
    DWORD dwP = 0;
    UCHAR chType = 0;
    //��������䵽ListCtrl
    for (DWORD i = 0; i < dwNumberOfProcessors * dwMap; i++)
    {
        //�ж�Pλ�Ƿ���Ч ��Ч����ʾ
        CFunction::GetAttribFormGDT((PGDT)(pParams + j * 8), &wAttrib);
        dwP = wAttrib & 0x80; //1000 0000B == 0x80 PΪλ1
        chType = CFunction::GetTypeValue(wAttrib);
        chType = chType & 1;
        if (dwP != 0x80 /*|| chType == 0*/) 
        {
            j++;
            continue;
        }
        else
        {
            CFunction::GetBaseFormGDT((PGDT)(pParams + j * 8), &dwBase);
            
            wsprintf(szTmpBuf, "%d", i / dwMap);
            m_LstGDT.InsertItem(k, szTmpBuf); //CPU���
            
            wsprintf(szTmpBuf, "0x%04X", i);
            m_LstGDT.SetItemText(k, 1, szTmpBuf); //��ѡ����
            
            wsprintf(szTmpBuf, "0x%08X", dwBase);
            m_LstGDT.SetItemText(k, 2, szTmpBuf); //��ַ
            
            CFunction::GetLimiteFormGDT((PGDT)(pParams + j * 8), &dwLimite);
            wsprintf(szTmpBuf, "0x%08X", dwLimite);
            m_LstGDT.SetItemText(k, 3, szTmpBuf); //����
            
            wsprintf(szTmpBuf, "%s", CFunction::GetG(wAttrib));
            m_LstGDT.SetItemText(k, 4, szTmpBuf); //������

            wsprintf(szTmpBuf, "%d", CFunction::GetDLP(wAttrib));
            m_LstGDT.SetItemText(k, 5, szTmpBuf); //����Ȩ��

            wsprintf(szTmpBuf, "%s", CFunction::GetTypeString(wAttrib));
            m_LstGDT.SetItemText(k, 6, szTmpBuf); //����;

            j++;
            k++;
        }
        

    }

    
    if (pParams != NULL)
    {
        delete pParams;
        pParams = NULL;
    }
	
    return TRUE;
}
