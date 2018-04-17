// MidDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Mid.h"
#include "MidDlg.h"

#include "..\common\MyPacket.h"
#include "..\common\ThreadPool.h"
#include "..\common\ICommand.h"
#include "AcceptThread.h"
#include "GetSrvRet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMidDlg dialog

CMidDlg::CMidDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMidDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMidDlg)
	m_dwMidPort = 0;
	m_dwSrvPort = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_sockListen = INVALID_SOCKET;
    m_sockMid = INVALID_SOCKET;
    m_nCltCount = 0;
}

void CMidDlg::OnOK()
{
    return;
}

void CMidDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMidDlg)
	DDX_Control(pDX, IDC_LIST, m_LstClt);
	DDX_Control(pDX, IDC_IPADDRESS, m_IP);
	DDX_Text(pDX, EDT_MIDPORT, m_dwMidPort);
	DDX_Text(pDX, EDT_SRVPORT, m_dwSrvPort);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMidDlg, CDialog)
	//{{AFX_MSG_MAP(CMidDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BTN_START, OnStart)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMidDlg message handlers

BOOL CMidDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
    m_dwMidPort = 27149;
    m_dwSrvPort = 10086;
    m_IP.SetAddress(htonl(inet_addr("127.0.0.1")));
    
    InitListCtrl();
    UpdateData(FALSE);

    
    BOOL bRet = m_ThreadPool.Create(10);
    if (!bRet)
    {
        TRACE("ssc Mid pool.Create(5) failed[]");
    }
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMidDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMidDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMidDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CMidDlg::InitListCtrl()
{
    m_LstClt.SetExtendedStyle(m_LstClt.GetExtendedStyle()|
        LVS_EX_GRIDLINES|
        LVS_EX_FULLROWSELECT);
    
    m_LstClt.InsertColumn(0, "IP", LVCFMT_LEFT, 160);
    m_LstClt.InsertColumn(1, "�˿�", LVCFMT_LEFT, 160);
    m_LstClt.InsertColumn(2, "SOCKET", LVCFMT_LEFT, 80);
    return TRUE;
}

void CMidDlg::OnStart() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
    UpdateData(FALSE);

    BOOL bRet = FALSE;
    m_sockListen = socket(AF_INET, 
        SOCK_STREAM,//��ʽ,TCP
        0);
    if (m_sockListen == INVALID_SOCKET)
    {
        TRACE("ssc Mid socket() failed[%d]", WSAGetLastError());
        return;
    }
    //2 bind ��IP �� �˿�
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    
    //��������IP������
    DWORD dwIP = 0;
    addr.sin_addr.S_un.S_addr = htonl(dwIP);
    
    addr.sin_port = htons((USHORT)m_dwMidPort); //ת�������ֽ���
    bRet = bind(m_sockListen, (sockaddr*)&addr, sizeof(sockaddr));
    if (bRet == SOCKET_ERROR)
    {
        TRACE("ssc Mid bind() failed[%d]", WSAGetLastError());
        return;
    }
    //3 ���� listen
    bRet = listen(m_sockListen, SOMAXCONN); //SOMAXCONN��ʾϵͳ����һ��������ֵ
    if (bRet == SOCKET_ERROR)
    {
        TRACE("ssc Mid listen() failed[%d]", WSAGetLastError());
        return;
    }
        
    CAcceptThread *pAcceptThread = new CAcceptThread((LPVOID)this);
    if (pAcceptThread == NULL)
    {
        TRACE("ssc Mid pAcceptThread == NULL");
        return;
    }
    
    m_ThreadPool.Handle(pAcceptThread);    

    //�ӷ���˽��յ��߳�Ҳ�����̳߳���
    CGetSrvRet *pGetSrvRet = new CGetSrvRet((LPVOID)this);
    if (pGetSrvRet == NULL)
    {
        TRACE("ssc Mid pGetSrvRet == NULL");
        return;
    }
    m_ThreadPool.Handle(pGetSrvRet);
   
    GetDlgItem(IDC_STATE)->SetWindowText("������");
    GetDlgItem(BTN_START)->EnableWindow(FALSE);


    //���ӷ����
    //�������
    //1. �����׽��֣�socket��
    m_sockMid = socket(AF_INET,
        SOCK_STREAM, //��ʽ,TCP
        0);
    if (m_sockMid == INVALID_SOCKET)
    {
        TRACE("ssc Mid socket() failed[%d]", WSAGetLastError());
        return;
    }
    //2. �������������������connect��
    sockaddr_in addrSrv;
    addrSrv.sin_family = AF_INET;
    dwIP = 0;
    m_IP.GetAddress(dwIP);
    addrSrv.sin_addr.S_un.S_addr = htonl(dwIP); //�ͻ�����Ҫָ������IP
    
    addrSrv.sin_port = htons((USHORT)m_dwSrvPort); //���Ӷ˿�
    bRet = connect(m_sockMid, (sockaddr*)&addrSrv, sizeof(sockaddr));
    if (bRet == SOCKET_ERROR)
    {
        TRACE("ssc Mid connect() failed[%d]", WSAGetLastError());
        return;
    }

    //��ת�����˷�������֪ͨ
    CMyPacket pktOnLine;
    CMySocket mySockOnLine(m_sockMid);
    
    BYTE btType = MID_ONLINE;
    
    pktOnLine.Append((char*)&btType, sizeof(btType));
    mySockOnLine.SendPacket(pktOnLine);
}

BOOL CMidDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	TRACE("ssc Mid CMidDlg::DestroyWindow() begin!");
    //����ת��������֪ͨ
    CMyPacket pkt;
    CMySocket mySock(m_sockMid);
    
    BYTE btType = MID_OFFLINE;
    
    pkt.Append((char*)&btType, sizeof(btType));
    mySock.SendPacket(pkt);
    TRACE("ssc Mid send MID_OFFLINE");

    closesocket(m_sockMid);
    m_sockMid = INVALID_SOCKET;

    closesocket(m_sockListen);
    m_sockListen = INVALID_SOCKET;

    m_ThreadPool.Destory();

    TRACE("ssc Mid CMidDlg::DestroyWindow() end!");
	return CDialog::DestroyWindow();
}

BOOL CMidDlg::RemoveOfflineClt(SOCKET sClient)
{
    TRACE("ssc Mid CMidDlg::RemoveOfflineClt(%p) begin!", sClient);
    if (sClient == INVALID_SOCKET)
    {
        TRACE("ssc Mid sClient is error");
        return FALSE;
    }
    TRACE("ssc Mid sClient is %p", sClient);

    //��������б��еĿͻ���
    char szSocket[MAXBYTE] = {0};
    wsprintf(szSocket, "%08X", sClient);
    
    CString csFind = "";
    
    int nCount = m_LstClt.GetItemCount();
    
    for (int i = 0; i < nCount; i++)
    {
        csFind = m_LstClt.GetItemText(i, 2);
        if (csFind == szSocket)
        {
            m_LstClt.DeleteItem(i);
            m_nCltCount--;
            break;
        }
    }

    TRACE("ssc Mid m_nCltCount is %d", m_nCltCount);
    
    //��ʱ��Ҫ�رտͻ���socket
    closesocket(sClient);
    sClient = INVALID_SOCKET;
    
    return TRUE;
}

BOOL CMidDlg::RecvCltSQL(SOCKET sClient, char* pszBuf, int nBufLength, int nSQLType)
{
    TRACE("ssc Mid RecvCltSQL(%p, %p, %d)", sClient, pszBuf, nBufLength);
    if (sClient == INVALID_SOCKET)
    {
        TRACE("ssc Mid sClient is INVALID_SOCKET!");
        return FALSE;
    }

    if (pszBuf == NULL)
    {
        TRACE("ssc Mid pszBuf is NULL!");
        return FALSE;
    }

    if (nBufLength <= 0)
    {
        TRACE("ssc Mid nBufLength <= 0!");
        return FALSE;
    }

    
    CString csFile = "data";

    //��ɾ�Ĳ���Ҫɾ������
    if (nSQLType == SQL_DEL ||
        nSQLType == SQL_ADD ||
        nSQLType == SQL_UPDATE)
    {
        BOOL bRet = DeleteFile(csFile);
        TRACE("ssc Mid Need del cache ret [%d] error[%d]", bRet, GetLastError());
    }

    //����л��� ����Ҫ�����˷���sql���
    WIN32_FILE_ATTRIBUTE_DATA attrs = { 0 };
    BOOL bExist = GetFileAttributesEx(csFile, GetFileExInfoStandard, &attrs);
    if (bExist == TRUE && nSQLType == SQL_SELECT)
    {
        //�л����ļ�ֱ�ӷ����ͻ��ˣ����پ��������
        HANDLE hFile = INVALID_HANDLE_VALUE;        
        hFile  = CreateFile(csFile,
                            GENERIC_ALL,
                            FILE_SHARE_READ,
                            NULL,
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL);

        DWORD dwReadedBytes = 0;

        DWORD dwFileSizeHigh = 0;
        DWORD dwBufSize = GetFileSize(hFile, &dwFileSizeHigh);

        char *pBuf = new char[dwBufSize];
        if (pBuf == NULL)
        {
            TRACE("ssc Mid RecvCltSQL new char failed!!");
            return FALSE;
        }

        
        BOOL bRet= ReadFile(hFile,
                            pBuf,
                            MAXBYTE,
                            &dwReadedBytes,
                            NULL);
        TRACE("ssc Mid Read Cache %s", csFile);

        CloseHandle(hFile);

        //��ͻ���ת��SQL���
        CMySocket skt(sClient);
        CMyPacket sendPkt;
        
        if (sendPkt.Append(pBuf, dwBufSize) == NULL )
        {
            TRACE("ssc Mid sendPkt.Append failed!");
            return FALSE;
        }
        
        skt.SendPacket(sendPkt);

        if (pBuf != NULL)
        {
            delete[] pBuf;
            pBuf = NULL;
        }

        return TRUE;
    }

    //������ת��SQL���
    CMySocket skt(m_sockMid);
    CMyPacket sendPkt;

    BYTE btType = nSQLType;
    int nBufSize = sizeof(BYTE);
    int nRet = 0;
    if (sendPkt.Append((char*)&btType, sizeof(btType)) == NULL )
    {
        TRACE("ssc Mid sendPkt.Append failed!");
        return FALSE;
    }

    //�����˷��Ϳͻ��˵�SQLʱ������һ�ݿͻ��˵��׽��֣�����˼ǵ�Ҫ����һ��
    if (sendPkt.Append((char*)&sClient, sizeof(sClient)) == NULL )
    {
        TRACE("ssc Mid sendPkt.Append failed!");
        return FALSE;
    }
    TRACE("ssc Mid sendPkt.Append sClient is %p", sClient);
        
    if (sendPkt.Append(pszBuf, nBufLength) == NULL )
    {
        TRACE("ssc Mid sendPkt.Append failed!");
        return FALSE;
    }
    
    skt.SendPacket(sendPkt);

    return TRUE;
}

BOOL CMidDlg::GetSrvSelectSQL(SOCKET sClient, char* pszBuf, int nBufLength)
{
    TRACE("ssc mid CMidDlg::GetSrvSelectSQL(%p, %p, %d)", sClient, pszBuf, nBufLength);
    if (sClient == INVALID_SOCKET)
    {
        TRACE("ssc Mid sClient is INVALID_SOCKET!");
        return FALSE;
    }
    
    if (pszBuf == NULL)
    {
        TRACE("ssc Mid pszBuf is NULL!");
        return FALSE;
    }
    
    if (nBufLength <= 0)
    {
        TRACE("ssc Mid nBufLength <= 0!");
        return FALSE;
    }

    //��ͻ���ת��SQL���
    CMySocket skt(sClient);
    CMyPacket sendPkt;
    
    BYTE btType = SQL_RESULT;
    int nBufSize = sizeof(BYTE);
    int nRet = 0;
    if (sendPkt.Append((char*)&btType, sizeof(btType)) == NULL )
    {
        TRACE("ssc Mid sendPkt.Append failed!");
        return FALSE;
    }
    
    if (sendPkt.Append(pszBuf, nBufLength) == NULL )
    {
        TRACE("ssc Mid sendPkt.Append failed!");
        return FALSE;
    }
    
    skt.SendPacket(sendPkt);

    //�����ݰ�����Ϊ����
    CString csFile = "data";
    SaveCache(csFile.GetBuffer(0), sendPkt.GetBuf(), sendPkt.GetLength());

    return TRUE;
}

void CMidDlg::SaveCache(char *pFimeName, char* pszBuf, int nBufSize)
{
    TRACE("ssc Mid CMidDlg::SaveCache(%s, %p, %d) begin", pFimeName, pszBuf, nBufSize);
    if (pszBuf == NULL || pFimeName == NULL)
    {
        return;
    }
    if (nBufSize < 0)
    {
        return;
    }

    HANDLE hFile = CreateFile(pFimeName, 
                                GENERIC_ALL,
                                FILE_SHARE_WRITE,         
                                NULL,                    
                                CREATE_ALWAYS,            
                                FILE_ATTRIBUTE_NORMAL,     
                                NULL);  

    DWORD dwNumberOfBytesWritten = 0;
    WriteFile(
                hFile,
                (LPCVOID)pszBuf,
                (DWORD)nBufSize, 
                &dwNumberOfBytesWritten,
                NULL);

    CloseHandle(hFile);
}
