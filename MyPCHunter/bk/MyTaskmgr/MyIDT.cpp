// MyIDT.cpp : implementation file
//

#include "stdafx.h"
#include "Function.h"
#include "MyDef.h"
#include "MyTaskmgr.h"
#include "MyIDT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyIDT dialog


CMyIDT::CMyIDT(CWnd* pParent /*=NULL*/)
	: CDialog(CMyIDT::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyIDT)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMyIDT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyIDT)
	DDX_Control(pDX, LST_IDT, m_LstIDT);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyIDT, CDialog)
	//{{AFX_MSG_MAP(CMyIDT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyIDT message handlers

BOOL CMyIDT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    //����ListCtrl���
    m_LstIDT.SetExtendedStyle(m_LstIDT.GetExtendedStyle() |
        LVS_EX_GRIDLINES |
        LVS_EX_FULLROWSELECT);
    //����ListCtrl����
    m_LstIDT.InsertColumn(1, "CPU���", LVCFMT_LEFT, 60);
    m_LstIDT.InsertColumn(2, "���", LVCFMT_LEFT, 60);
    m_LstIDT.InsertColumn(3, "��������", LVCFMT_LEFT, 100);
    m_LstIDT.InsertColumn(4, "��ѡ����", LVCFMT_LEFT, 60);
    m_LstIDT.InsertColumn(5, "��ǰ������ַ", LVCFMT_LEFT, 200);
    m_LstIDT.InsertColumn(6, "Hook", LVCFMT_LEFT, 100);
    m_LstIDT.InsertColumn(7, "ԭʼ������ַ", LVCFMT_LEFT, 200);
    m_LstIDT.InsertColumn(8, "��ǰ��������ģ��", LVCFMT_LEFT, 200);
    
    UpdateData(FALSE);

    GetIDTFromSys();
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

DWORD CMyIDT::GetIDTFromSys()
{
    char szBuf[MAXBYTE] = {0};

    //���CPU������
    DWORD dwNumberOfProcessors = CFunction::GetNumberOfProcessors();
    
    DWORD dwBytes = 0;
    //��������ȡIDT���� ��������0x7ff���Ǳ���������ǻ�ȡ��һ�º�

    DWORD dwMap = 0; //IDT������
    DWORD dwIDTL = 0; //IDT����
    
    extern CMyTaskmgrApp theApp;
    HANDLE hDrvFile = theApp.m_hDrvFile;
    if (!DeviceIoControl(hDrvFile, 
                        IOCTRL_GET_IDTLIMIT, 
                        (LPVOID)&dwIDTL, 
                        sizeof(DWORD),
                        (LPVOID)&dwIDTL,
                        sizeof(DWORD),
                        &dwBytes, 
                        NULL))
    {
        //CloseHandle(hDrvFile);
        wsprintf(szBuf, "DeviceIoControl IOCTRL_GET_IDTLIMIT ErrorCode:%d\n", GetLastError());
        AfxMessageBox(szBuf);
        return 0;
    }

    dwIDTL = dwIDTL + 1; //��0��ʼ�������Լ�1
    dwMap = dwIDTL / 8; //��������Ӧ����256

    //IDT������������� ������ x dwIDTL �ֽ�
    //���봫������pParams����ڴ�
    char *pParams = new char[dwNumberOfProcessors * dwIDTL];
    if (pParams == NULL)
    {
        wsprintf(szBuf, "new CONTORL_PARAMS faild!");
        AfxMessageBox(szBuf);
        return FALSE;
    }
    ZeroMemory(pParams, dwNumberOfProcessors * dwIDTL);
    //���ô���������
    ((PCONTORL_PARAMS)pParams)->CPUNumber = dwNumberOfProcessors;
    
    if (!DeviceIoControl(hDrvFile, 
                        IOCTRL_GET_IDT, 
                        pParams, 
                        sizeof(CONTORL_PARAMS),
                        pParams, 
                        dwNumberOfProcessors * dwIDTL, 
                        &dwBytes, 
                        NULL))
    {
        //CloseHandle(hDrvFile);
        wsprintf(szBuf, "DeviceIoControl IOCTRL_GET_IDT ErrorCode:%d\n", GetLastError());
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
    WORD wAttrib = 0;
    DWORD dwSelector = 0;
    DWORD dwOffset = 0;
    DWORD dwP = 0;
    UCHAR chType = 0;
    //��������䵽ListCtrl
    for (DWORD i = 0; i < dwNumberOfProcessors * dwMap; i++)
    {
        wsprintf(szTmpBuf, "%d", i / dwMap);
        m_LstIDT.InsertItem(i, szTmpBuf); //CPU���
        
        wsprintf(szTmpBuf, "0x%02X", i / dwNumberOfProcessors);
        m_LstIDT.SetItemText(i, 1, szTmpBuf); //���
        
        wsprintf(szTmpBuf, "%s", g_szIDTFuncName[i]);
        m_LstIDT.SetItemText(i, 2, szTmpBuf); //��������
        
        CFunction::GetSelectorFormIDT((PIDT)(pParams + j * 8), &dwSelector);
        wsprintf(szTmpBuf, "0x%02X", dwSelector);
        m_LstIDT.SetItemText(i, 3, szTmpBuf); //��ѡ����
        
        
        CFunction::GetOffsetFormIDT((PIDT)(pParams + j * 8), &dwOffset);
        wsprintf(szTmpBuf, "0x%08X", dwOffset);
        m_LstIDT.SetItemText(i, 4, szTmpBuf); //��ǰ������ַ

        wsprintf(szTmpBuf, "%s", "-");
        m_LstIDT.SetItemText(i, 5, szTmpBuf); //Hook

        wsprintf(szTmpBuf, "0x%08X", dwOffset);
        m_LstIDT.SetItemText(i, 6, szTmpBuf); //ԭʼ������ַ;

        wsprintf(szTmpBuf, "%s", "ntoskrnl.exe");
        m_LstIDT.SetItemText(i, 7, szTmpBuf); //��ǰ������ַ����ģ��;

        j++;
    }

    
    if (pParams != NULL)
    {
        delete pParams;
        pParams = NULL;
    }
	
    return TRUE;
}

//static char *g_szIDTFuncName[MAXBYTE] = {""};

static char *g_szIDTFuncName[MAXBYTE + 1] = {
    "Divide error",
    "Debug",
    "Not used",
    "Breakpoint",
    "Overflow",
    "Bounds check",
    "Invalid opcode",
    "Device not available",
    "Double fault",
    "Coprocessor segment overrun",
    "Invalid TSS",
    "Segment not present",
    "Stack segment fault",
    "General protection",
    "Page Fault",
    "Reserved by Intel",
    "Floating-point error",
    "Alignment check",
    "Machine check",
    "SIMD floating point exception",
    "Reserved by Intel",
    "Reserved by Intel",
    "Reserved by Intel",
    "Reserved by Intel",
    "Reserved by Intel",
    "Reserved by Intel",
    "Reserved by Intel",
    "Reserved by Intel",
    "Reserved by Intel",
    "Reserved by Intel",
    "Reserved by Intel",
    "Reserved by Intel",
    "Not used",
    "Not used",
    "Not used",
    "Not used",
    "Not used",
    "Not used",
    "Not used",
    "Not used",
    "Not used",
    "Not used",
    "KiGetTickCount",
    "KiCallbackReturn",
    "KiSetLowWaitHighThread",
    "KiDebugService",
    "KiSystemService",
    "Reserved for APIC",
    "KiStartUnexpectedRange",
    "KiUnexpectedInterrupt1",
    "KiUnexpectedInterrupt2",
    "KiUnexpectedInterrupt3",
    "KiUnexpectedInterrupt4",
    "KiUnexpectedInterrupt5",
    "KiUnexpectedInterrupt6",
    "KiUnexpectedInterrupt7",
    "KiUnexpectedInterrupt8",
    "KiUnexpectedInterrupt9",
    "KiUnexpectedInterrupt10",
    "KiUnexpectedInterrupt11",
    "KiUnexpectedInterrupt12",
    "KiUnexpectedInterrupt13",
    "KiUnexpectedInterrupt14",
    "KiUnexpectedInterrupt15",
    "KiUnexpectedInterrupt16",
    "KiUnexpectedInterrupt17",
    "KiUnexpectedInterrupt18",
    "KiUnexpectedInterrupt19",
    "KiUnexpectedInterrupt20",
    "KiUnexpectedInterrupt21",
    "KiUnexpectedInterrupt22",
    "KiUnexpectedInterrupt23",
    "KiUnexpectedInterrupt24",
    "KiUnexpectedInterrupt25",
    "KiUnexpectedInterrupt26",
    "KiUnexpectedInterrupt27",
    "KiUnexpectedInterrupt28",
    "KiUnexpectedInterrupt29",
    "KiUnexpectedInterrupt30",
    "KiUnexpectedInterrupt31",
    "KiUnexpectedInterrupt32",
    "KiUnexpectedInterrupt33",
    "KiUnexpectedInterrupt34",
    "KiUnexpectedInterrupt35",
    "KiUnexpectedInterrupt36",
    "KiUnexpectedInterrupt37",
    "KiUnexpectedInterrupt38",
    "KiUnexpectedInterrupt39",
    "KiUnexpectedInterrupt40",
    "KiUnexpectedInterrupt41",
    "KiUnexpectedInterrupt42",
    "KiUnexpectedInterrupt43",
    "KiUnexpectedInterrupt44",
    "KiUnexpectedInterrupt45",
    "KiUnexpectedInterrupt46",
    "KiUnexpectedInterrupt47",
    "KiUnexpectedInterrupt48",
    "KiUnexpectedInterrupt49",
    "KiUnexpectedInterrupt50",
    "KiUnexpectedInterrupt51",
    "KiUnexpectedInterrupt52",
    "KiUnexpectedInterrupt53",
    "KiUnexpectedInterrupt54",
    "KiUnexpectedInterrupt55",
    "KiUnexpectedInterrupt56",
    "KiUnexpectedInterrupt57",
    "KiUnexpectedInterrupt58",
    "KiUnexpectedInterrupt59",
    "KiUnexpectedInterrupt60",
    "KiUnexpectedInterrupt61",
    "KiUnexpectedInterrupt62",
    "KiUnexpectedInterrupt63",
    "KiUnexpectedInterrupt64",
    "KiUnexpectedInterrupt65",
    "KiUnexpectedInterrupt66",
    "KiUnexpectedInterrupt67",
    "KiUnexpectedInterrupt68",
    "KiUnexpectedInterrupt69",
    "KiUnexpectedInterrupt70",
    "KiUnexpectedInterrupt71",
    "KiUnexpectedInterrupt72",
    "KiUnexpectedInterrupt73",
    "KiUnexpectedInterrupt74",
    "KiUnexpectedInterrupt75",
    "KiUnexpectedInterrupt76",
    "KiUnexpectedInterrupt77",
    "KiUnexpectedInterrupt78",
    "KiUnexpectedInterrupt79",
    "KiUnexpectedInterrupt80",
    "KiUnexpectedInterrupt81",
    "KiUnexpectedInterrupt82",
    "KiUnexpectedInterrupt83",
    "KiUnexpectedInterrupt84",
    "KiUnexpectedInterrupt85",
    "KiUnexpectedInterrupt86",
    "KiUnexpectedInterrupt87",
    "KiUnexpectedInterrupt88",
    "KiUnexpectedInterrupt89",
    "KiUnexpectedInterrupt90",
    "KiUnexpectedInterrupt91",
    "KiUnexpectedInterrupt92",
    "KiUnexpectedInterrupt93",
    "KiUnexpectedInterrupt94",
    "KiUnexpectedInterrupt95",
    "KiUnexpectedInterrupt96",
    "KiUnexpectedInterrupt97",
    "KiUnexpectedInterrupt98",
    "KiUnexpectedInterrupt99",
    "KiUnexpectedInterrupt100",
    "KiUnexpectedInterrupt101",
    "KiUnexpectedInterrupt102",
    "KiUnexpectedInterrupt103",
    "KiUnexpectedInterrupt104",
    "KiUnexpectedInterrupt105",
    "KiUnexpectedInterrupt106",
    "KiUnexpectedInterrupt107",
    "KiUnexpectedInterrupt108",
    "KiUnexpectedInterrupt109",
    "KiUnexpectedInterrupt110",
    "KiUnexpectedInterrupt111",
    "KiUnexpectedInterrupt112",
    "KiUnexpectedInterrupt113",
    "KiUnexpectedInterrupt114",
    "KiUnexpectedInterrupt115",
    "KiUnexpectedInterrupt116",
    "KiUnexpectedInterrupt117",
    "KiUnexpectedInterrupt118",
    "KiUnexpectedInterrupt119",
    "KiUnexpectedInterrupt120",
    "KiUnexpectedInterrupt121",
    "KiUnexpectedInterrupt122",
    "KiUnexpectedInterrupt123",
    "KiUnexpectedInterrupt124",
    "KiUnexpectedInterrupt125",
    "KiUnexpectedInterrupt126",
    "KiUnexpectedInterrupt127",
    "KiUnexpectedInterrupt128",
    "KiUnexpectedInterrupt129",
    "KiUnexpectedInterrupt130",
    "KiUnexpectedInterrupt131",
    "KiUnexpectedInterrupt132",
    "KiUnexpectedInterrupt133",
    "KiUnexpectedInterrupt134",
    "KiUnexpectedInterrupt135",
    "KiUnexpectedInterrupt136",
    "KiUnexpectedInterrupt137",
    "KiUnexpectedInterrupt138",
    "KiUnexpectedInterrupt139",
    "KiUnexpectedInterrupt140",
    "KiUnexpectedInterrupt141",
    "KiUnexpectedInterrupt142",
    "KiUnexpectedInterrupt143",
    "KiUnexpectedInterrupt144",
    "KiUnexpectedInterrupt145",
    "KiUnexpectedInterrupt146",
    "KiUnexpectedInterrupt147",
    "KiUnexpectedInterrupt148",
    "KiUnexpectedInterrupt149",
    "KiUnexpectedInterrupt150",
    "KiUnexpectedInterrupt151",
    "KiUnexpectedInterrupt152",
    "KiUnexpectedInterrupt153",
    "KiUnexpectedInterrupt154",
    "KiUnexpectedInterrupt155",
    "KiUnexpectedInterrupt156",
    "KiUnexpectedInterrupt157",
    "KiUnexpectedInterrupt158",
    "KiUnexpectedInterrupt159",
    "KiUnexpectedInterrupt160",
    "KiUnexpectedInterrupt161",
    "KiUnexpectedInterrupt162",
    "KiUnexpectedInterrupt163",
    "KiUnexpectedInterrupt164",
    "KiUnexpectedInterrupt165",
    "KiUnexpectedInterrupt166",
    "KiUnexpectedInterrupt167",
    "KiUnexpectedInterrupt168",
    "KiUnexpectedInterrupt169",
    "KiUnexpectedInterrupt170",
    "KiUnexpectedInterrupt171",
    "KiUnexpectedInterrupt172",
    "KiUnexpectedInterrupt173",
    "KiUnexpectedInterrupt174",
    "KiUnexpectedInterrupt175",
    "KiUnexpectedInterrupt176",
    "KiUnexpectedInterrupt177",
    "KiUnexpectedInterrupt178",
    "KiUnexpectedInterrupt179",
    "KiUnexpectedInterrupt180",
    "KiUnexpectedInterrupt181",
    "KiUnexpectedInterrupt182",
    "KiUnexpectedInterrupt183",
    "KiUnexpectedInterrupt184",
    "KiUnexpectedInterrupt185",
    "KiUnexpectedInterrupt186",
    "KiUnexpectedInterrupt187",
    "KiUnexpectedInterrupt188",
    "KiUnexpectedInterrupt189",
    "KiUnexpectedInterrupt190",
    "KiUnexpectedInterrupt191",
    "KiUnexpectedInterrupt192",
    "KiUnexpectedInterrupt193",
    "KiUnexpectedInterrupt194",
    "KiUnexpectedInterrupt195",
    "KiUnexpectedInterrupt196",
    "KiUnexpectedInterrupt197",
    "KiUnexpectedInterrupt198",
    "KiUnexpectedInterrupt199",
    "KiUnexpectedInterrupt200",
    "KiUnexpectedInterrupt201",
    "KiUnexpectedInterrupt202",
    "KiUnexpectedInterrupt203",
    "KiUnexpectedInterrupt204",
    "KiUnexpectedInterrupt205",
    "KiUnexpectedInterrupt206",
    "KiUnexpectedInterrupt207"
};
