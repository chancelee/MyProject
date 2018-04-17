// play_airplaneDlg.cpp : implementation file
//

#include "stdafx.h"
#include "play_airplane.h"
#include "play_airplaneDlg.h"
#include "resource.h"

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
// CPlay_airplaneDlg dialog

CPlay_airplaneDlg::CPlay_airplaneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlay_airplaneDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlay_airplaneDlg)
	m_ProcessID = 0;
	m_BulletNum = 0;
	m_Mode = -1;
	m_Shift = 0;
	m_AI = FALSE;
	m_bAways = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPlay_airplaneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPlay_airplaneDlg)
	DDX_Text(pDX, IDC_EDIT1, m_ProcessID);
	DDX_Text(pDX, IDC_EDIT2, m_BulletNum);
	DDX_Radio(pDX, IDC_RADIO1, m_Mode);
	DDX_Text(pDX, IDC_EDIT3, m_Shift);
    DDX_Check(pDX, IDC_CHECKAI, m_AI);
    DDX_Control(pDX, IDC_SPIN1, m_Spin);
	DDX_Check(pDX, IDC_CHECK_LIVEAWAYS, m_bAways);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPlay_airplaneDlg, CDialog)
	//{{AFX_MSG_MAP(CPlay_airplaneDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonFindProcess)
	ON_BN_CLICKED(IDC_RADIO1, OnRadioMode)
	ON_BN_CLICKED(IDC_RADIO2, OnRadioMode2)
	ON_BN_CLICKED(IDC_BUTTON_NUM, OnButtonSetNum)
	ON_BN_CLICKED(IDC_BUTTON_SHIFT, OnButtonShift)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpinPen)
	ON_BN_CLICKED(IDC_CHECKAI, OnCheckAI)
	ON_BN_CLICKED(IDC_BUTTON2, OnButtonSetInitZero)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_LIVEAWAYS, OnCheckLiveaways)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlay_airplaneDlg message handlers

BOOL CPlay_airplaneDlg::OnInitDialog()
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
    m_hProcess = NULL;
    m_airplaneX = 0;
    m_airplaneY = 0;
    m_IsFind = TRUE;

	m_Spin.SetBuddy(GetDlgItem(IDC_EDIT3));
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPlay_airplaneDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPlay_airplaneDlg::OnPaint() 
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
HCURSOR CPlay_airplaneDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CPlay_airplaneDlg::FindGameProc()
{
    HWND hWnd = ::FindWindow("wcTKKN", NULL);
    if (hWnd == NULL)
    {
        return FALSE;
    }
    GetWindowThreadProcessId(hWnd, &m_ProcessID);
    if (m_ProcessID == 0)
    {
        return FALSE;
    }
    
    m_hProcess = OpenProcess(
        PROCESS_ALL_ACCESS,
        FALSE,
        m_ProcessID);

    if (m_hProcess == NULL)
    {
        return FALSE;
    }

    return TRUE;
}

void CPlay_airplaneDlg::OnButtonFindProcess() 
{
	// TODO: Add your control notification handler code here
    if (FindGameProc() == FALSE)
    {
        AfxMessageBox("û�ҵ���Ϸ����!");
        return;
    }
        
    //��ȡ��ǰΪ��ͨģʽ�����޵�ģʽ
    char szBuf[MAXBYTE] = {0};
    SIZE_T nReadedBytes = 0;
    SIZE_T nWriteedBytes = 0;
    
    BOOL bRet = ReadProcessMemory(
                            m_hProcess,
                            (LPVOID)0x00403616,
                            szBuf,
                            1,
                            &nReadedBytes);
    if (!bRet)
    {
        AfxMessageBox("��ȡ�ڴ�ʧ��");
        m_Mode = -1;
    }
    else if (szBuf[0] == '\xEB') //��ȡ��ͨ�����޵�
    {
        m_Mode = 1;
    }
    else
    {
        m_Mode = 0;
    }

    //��ȡ�ӵ�����0x00406DA8
    DWORD dwSum = 0;
    bRet = ReadProcessMemory(
                        m_hProcess,
                        (LPVOID)0x00406DA8,
                        &dwSum,
                        4,
                        &nReadedBytes);
    m_BulletNum = dwSum;

    //����ٶ�0x004020F5
    bRet = ReadProcessMemory(
                    m_hProcess,
                    (LPVOID)0x004020F5,
                    szBuf,
                    1,
                    &nReadedBytes);

    m_Shift = szBuf[0];

    //��ǰ�ɻ�λ��
    //0x00406D6C   4   nplanX,
    //0x00406D70   4   nplanY,
    bRet = ReadProcessMemory(
                m_hProcess,
                (LPVOID)0x00406D6C,
                &m_airplaneX,
                4,
                &nReadedBytes);
    
    bRet = ReadProcessMemory(
                m_hProcess,
                (LPVOID)0x00406D70,
                &m_airplaneY,
                4,
                &nReadedBytes);

    UpdateData(FALSE); //��������
}

void CPlay_airplaneDlg::OnRadioMode() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE); //��ȡ����

    char szBuf[MAXBYTE] = {0};
    SIZE_T nReadedBytes = 0;
    SIZE_T nWriteedBytes = 0;
    BOOL bRet = FALSE;
    //��ͨģʽ
    szBuf[0] = '\x74';
    bRet = WriteProcessMemory(
                        m_hProcess,
                        (LPVOID)0x00403616,
                        szBuf,
                        1,
                        &nWriteedBytes);
    if (!bRet)
    {
        AfxMessageBox("�޸��ڴ�ʧ��");
    }

    UpdateData(FALSE); //��������
}

void CPlay_airplaneDlg::OnRadioMode2() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE); //��ȡ����

    char szBuf[MAXBYTE] = {0};
    SIZE_T nReadedBytes = 0;
    SIZE_T nWriteedBytes = 0;
    BOOL bRet = FALSE;

    szBuf[0] = '\xEB';

    bRet = WriteProcessMemory(
                        m_hProcess,
                        (LPVOID)0x00403616,
                        szBuf,
                        1,
                        &nWriteedBytes);
    if (!bRet)
    {
        AfxMessageBox("�޸��ڴ�ʧ��");
    }
    
    UpdateData(FALSE); //��������
}

void CPlay_airplaneDlg::OnButtonSetNum() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE); //��ȡ����

    //�����ӵ�����0x00406DA8
    BOOL bRet = FALSE;
    
    DWORD dwWrtSize = -1;
    bRet = WriteProcessMemory(
                    m_hProcess,
                    (LPVOID)0x00406DA8,
                    &m_BulletNum,
                    4,
                    &dwWrtSize);
    if (!bRet || dwWrtSize < 4)
    {
        AfxMessageBox("�޸��ڴ�ʧ��");
    }
    
    UpdateData(FALSE); //��������
}

void CPlay_airplaneDlg::OnButtonShift() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE); //��ȡ����
    
    //�����ٶ�0x004020F5
    BOOL bRet = FALSE;    
    DWORD dwWrtSize = -1;
    bRet = WriteProcessMemory(
        m_hProcess,
        (LPVOID)0x004020F5,
        &m_Shift,
        1,
        &dwWrtSize);
    if (!bRet || dwWrtSize < 1)
    {
        AfxMessageBox("�޸��ڴ�ʧ��");
    }
    
    UpdateData(FALSE); //��������
	
}

void CPlay_airplaneDlg::OnDeltaposSpinPen(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    UpdateData(TRUE);
    if(pNMUpDown->iDelta == 1)        // �����ֵΪ1 , ˵�������Spin�����¼�ͷ
    {    
        //���ַ�����ش��� , ���罫"b" ��Ϊ "a"    
        if (m_Shift == 0)
        {
            //Ĭ���ٶ�0���ô���;
        } 
        if (m_Shift >= 1)
        {
            m_Shift--;
        } 
        if (m_Shift > 100)
        {
            m_Shift = 100;
        }
    }
    else if(pNMUpDown->iDelta == -1) // �����ֵΪ-1 , ˵�������Spin�����ϼ�ͷ
    {
        //���ַ�����ش��� , ���罫"a" ��Ϊ "b"
        if (m_Shift < 100)
        {
            m_Shift++;
        }
        else
        {
            m_Shift = 100;
        }
    }
    UpdateData(FALSE);
	*pResult = 0;
}

void CPlay_airplaneDlg::OnCheckAI() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
    if (m_AI == TRUE)
    {
        SetTimer(MYTIMER, 50, NULL);
    }
    else
    {
        //����ģʽ
        AIFun(FALSE);
        KillTimer(MYTIMER);
    }
    UpdateData(FALSE);    
}

BOOL CPlay_airplaneDlg::CheckInRgn(stBullet *pBuf, 
                                   int nCount, 
                                   DWORD airplaneX, 
                                   DWORD airplaneY,
                                   DWORD nRgn)
{
    
    BOOL bRet = FALSE;
    POINT bPoint = {0}; 

    //�����ɻ���������
    POINT ptAry[2] = {0};
    
    ptAry[0].x = airplaneX + nRgn;
    ptAry[0].y = airplaneY + nRgn;
    
    ptAry[1].x = airplaneX - nRgn;
    ptAry[1].y = airplaneY - nRgn;
    

    DWORD dwX = 0;
    DWORD dwY = 0;
    
    for (int i = 0; i < nCount; i++)
    {
        dwX = (pBuf[i].dwX) >> 6;
        dwX -= 4;

        dwY = (pBuf[i].dwY) >> 6;
        dwY -= 4;

        bPoint.x = dwX;   
        bPoint.y = dwY;
        
        //�ж�bPoint���Ƿ��ھ��������ڣ��������Ч�ʵ�
        //if (rgn.PtInRegion(bPoint))
        if (bPoint.x >= ptAry[1].x && bPoint.x <= ptAry[0].x &&
            bPoint.y <= ptAry[0].y && bPoint.y >= ptAry[1].y)
        {
            return TRUE;
        }        
    }
    return FALSE;
}

void CPlay_airplaneDlg::AIFun(BOOL bIsAuto)
{  
    if (bIsAuto == FALSE)
    {
        return;
    }
    BOOL bRet = FALSE;
    
    int nCount = -1;
    DWORD dwWrtSize = -1;
    DWORD nReadedBytes = -1;
    
    while (TRUE)
    {
        if (FindGameProc() == FALSE)
        {
            return;
        }
        //��ȡ�ӵ�����0x00406DA8
        bRet = ReadProcessMemory(
            m_hProcess,
            (LPVOID)0x00406DA8,
            &nCount,
            4,
            &dwWrtSize);
        
        //�ӵ�����
        //[DWORD]x [DWORD]y [char[7]]δ֪����
        stBullet *pBuf = new stBullet[nCount];
        ASSERT(pBuf);
        
        //��Ontimer���ͷ�pBuf
        if (pBuf != NULL)
        {
            m_LstRelease.AddTail(pBuf);
        }
        //�����ͷ�
        //memset(pBuf, 0, nCount * sizeof(stBullet));
        
        //��ȡ�ӵ������׵�ַ0x00406E10
        bRet = ReadProcessMemory(
            m_hProcess,
            (LPVOID)0x00406E10,
            pBuf,
            nCount * sizeof(stBullet),
            &dwWrtSize);
        
        //���÷ɻ�����
        //0x00406D6C   4   nplanX,
        //0x00406D70   4   nplanY,     
        //         
        // ���Ͻ� x == 0�� y == e0
        // ���Ͻ� x == 130�� y == e0
        // ���½� x == 0�� y == 0
        // ���½� x == 130  y == 0
        //
        //�ɻ����ӵ���ԭ������Ϸ�����½�
        //
        //��ǰ�ɻ�λ��
        //0x00406D6C   4   nplanX,
        //0x00406D70   4   nplanY,
        bRet = ReadProcessMemory(
            m_hProcess,
            (LPVOID)0x00406D6C,
            &m_airplaneX,
            4,
            &nReadedBytes);
        
        bRet = ReadProcessMemory(
            m_hProcess,
            (LPVOID)0x00406D70,
            &m_airplaneY,
            4,
            &nReadedBytes);
        
        DWORD airplaneX = m_airplaneX;
        DWORD airplaneY = m_airplaneY;
        m_IsFind = CheckInRgn(pBuf, nCount, airplaneX, airplaneY, 15);

        while (m_IsFind == TRUE)
        { 
            //�ɻ����������У���Ҫ������
            airplaneX += 5;
            if (airplaneX > 0x130)
            {
                airplaneY += 5;
                airplaneX = 10;
            }
            if (airplaneY > 0xDE)
            {
                airplaneY = 10;
            }        
            
            m_IsFind = CheckInRgn(pBuf, nCount, airplaneX, airplaneY, 15);
        } // while (m_IsFind == TRUE) 
        
        //˵�������������ӵ����궼û���зɻ������÷ɻ�����
        m_airplaneX = airplaneX;
        m_airplaneY = airplaneY;
        
        //���÷ɻ�����
        //0x00406D6C   4   nplanX,
        //0x00406D70   4   nplanY,
        bRet = WriteProcessMemory(
            m_hProcess,
            (LPVOID)0x00406D6C,
            &m_airplaneX,
            4,
            &dwWrtSize);
        
        bRet = WriteProcessMemory(
            m_hProcess,
            (LPVOID)0x00406D70,
            &m_airplaneY,
            4,
            &dwWrtSize);
        return;
    } //end while (TRUE)    
    return;
}

void CPlay_airplaneDlg::OnButtonSetInitZero() 
{
	// TODO: Add your control notification handler code here
	//0x0040469E   4    ��ʼ�ӵ�����
    DWORD dwWrtSize = -1;
    DWORD dwNum = 0;
    BOOL bRet = WriteProcessMemory(
                            m_hProcess,
                            (LPVOID)0x0040469E,
                            &dwNum,
                            4,
                            &dwWrtSize);

}

void CPlay_airplaneDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	KillTimer(MYTIMER);
	// TODO: Add your message handler code here
}

void CPlay_airplaneDlg::OnOK() 
{
	// TODO: Add extra validation here
    KillTimer(MYTIMER);
	CDialog::OnOK();
}

void CPlay_airplaneDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	KillTimer(MYTIMER);
	CDialog::OnCancel();
}

void CPlay_airplaneDlg::OnTimer(UINT nIDEvent) 
{
    if (FindGameProc() == FALSE)
    {
        return;
    }
    AIFun(TRUE);
    POSITION pos = m_LstRelease.GetHeadPosition();
    while (pos)
    {
        stBullet *pTmp = m_LstRelease.GetNext(pos);
        if (pTmp != NULL)
        {
            delete[] pTmp;
            pTmp = NULL;
        }
    }
    m_LstRelease.RemoveAll();
   
}

void CPlay_airplaneDlg::OnCheckLiveaways() 
{
	// TODO: Add your control notification handler code here
    if (FindGameProc() == FALSE)
    {
        return;
    }

    UpdateData(TRUE);
	m_bAways =  TRUE;
    UpdateData(FALSE);
    KillTimer(MYTIMER);

    while (m_bAways)
    {
        AIFun(TRUE);
        POSITION pos = m_LstRelease.GetHeadPosition();
        while (pos)
        {
            stBullet *pTmp = m_LstRelease.GetNext(pos);
            if (pTmp != NULL)
            {
                delete[] pTmp;
                pTmp = NULL;
            }
        }
        m_LstRelease.RemoveAll();
    }   
}
