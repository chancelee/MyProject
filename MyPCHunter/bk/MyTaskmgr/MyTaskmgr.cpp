// MyTaskmgr.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <Winsvc.h>
#include "MyDef.h"
#include "MyTaskmgr.h"
#include "MyTaskmgrDlg.h"
#include "Function.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyTaskmgrApp

BEGIN_MESSAGE_MAP(CMyTaskmgrApp, CWinApp)
	//{{AFX_MSG_MAP(CMyTaskmgrApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyTaskmgrApp construction

CMyTaskmgrApp::CMyTaskmgrApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
    m_schSCManager = NULL;
    m_schService = NULL;
    m_hDrvFile = INVALID_HANDLE_VALUE; 
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMyTaskmgrApp object

CMyTaskmgrApp theApp;



/////////////////////////////////////////////////////////////////////////////
// CMyTaskmgrApp initialization

BOOL CMyTaskmgrApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CMyTaskmgrDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

BOOL CMyTaskmgrApp::InitApplication() 
{
	// TODO: Add your specialized code here and/or call the base class
	//ֹͣ����
    BOOL bRet = FALSE;
    m_schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (m_schSCManager == NULL)
    {
        CFunction::ShowErrorMsg();
        return FALSE;
    }
    m_schService = OpenService(m_schSCManager, DRV_SVC_NAME, SERVICE_ALL_ACCESS);
    if (m_schService != NULL)
    {
        //�����ڷ���˵����֮ǰûж�ظɾ�
        SERVICE_STATUS svcStatus;
        bRet = ControlService(m_schService, SERVICE_CONTROL_STOP, &svcStatus);
        //if (bRet == FALSE)
        //{
            //ֹͣ����ʱʧ�ܲ�����ʾ
            //CFunction::ShowErrorMsg();
        //}
        
        //�����Ƿ�ֹͣ�ɹ���Ҫж������
        bRet = DeleteService(m_schService);            
        CloseServiceHandle(m_schService);
        m_schService = NULL;
  
    }
    //��װ����    
    char szFileName[MAXWORD] = {0};
    CString	strAppPath;  // ������
    CString strDrvPath;
    TCHAR szModuleFileName[MAX_PATH]; // ȫ·����
    TCHAR drive[_MAX_DRIVE];  // �̷����ƣ�����˵C�̰���D�̰�
    TCHAR dir[_MAX_DIR]; // Ŀ¼
    TCHAR fname[_MAX_FNAME];  // ��������
    TCHAR ext[_MAX_EXT]; //��׺��һ��Ϊexe������dll
    if (NULL == GetModuleFileName(NULL, szModuleFileName, MAX_PATH)) //��õ�ǰ���̵��ļ�·��
    {
        return FALSE;
    }
    //�ָ��ַ���
    _tsplitpath(szModuleFileName, drive, dir, fname, ext);  //�ָ��·�����õ��̷���Ŀ¼���ļ�������׺��
    strAppPath = drive;
    strAppPath += dir; 
    
    //�жϵ�ǰ��x86����x64 x64����MySysX64.sys
    //���Ƿ���������������������
    
    //GetSystemInfo(&si); //x64 OS ���� 32λ app ������õ�����Ϣ�Ǵ��

    SYSTEM_INFO si;
    //GetNativeSystemInfo(&si);
    HMODULE hKrnl32 = LoadLibrary("Kernel32.dll");
    if (hKrnl32 == NULL)
    {
        CFunction::ShowErrorMsg();
        return FALSE;
    }

    PFN_GetNativeSystemInfo pfn = (PFN_GetNativeSystemInfo)GetProcAddress(hKrnl32, "GetNativeSystemInfo");
    if (pfn == NULL)
    {
        CFunction::ShowErrorMsg();
        return FALSE;;
    }
    pfn(&si);

    if (hKrnl32 != NULL)
    {
        FreeLibrary(hKrnl32);
        hKrnl32 = NULL;
    }    

    if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
    {
        strDrvPath = strAppPath + DRV_FILE_X64;
    }
    else
    {
        strDrvPath = strAppPath + DRV_FILE;
    }
        
    m_schService = CreateService(m_schSCManager,              // SCManager database 
                                    DRV_SVC_NAME,              // name of service 
                                    DRV_SVC_NAME,           // service name to display 
                                    SERVICE_ALL_ACCESS,        // desired access 
                                    SERVICE_KERNEL_DRIVER, // service type 
                                    SERVICE_DEMAND_START,      // start type 
                                    SERVICE_ERROR_NORMAL,      // error control type 
                                    strDrvPath,        // service's binary 
                                    NULL,                      // no load ordering group 
                                    NULL,                      // no tag identifier 
                                    NULL,                      // no dependencies 
                                    NULL,                      // LocalSystem account 
                                    NULL);                     // no password 
    
    if (m_schService == NULL)
    {
        CFunction::ShowErrorMsg();
        return FALSE;
    }
    
    //��������    
    bRet = StartService(m_schService, NULL, NULL);
    if(!bRet)
    {
        CFunction::ShowErrorMsg();
        return FALSE;
    }

    char szBuf[MAXBYTE] = {0};
    //��������
    theApp.m_hDrvFile = CreateFile("\\\\.\\MySys", 
                                    GENERIC_WRITE | GENERIC_READ, 
                                    0, 
                                    NULL, 
                                    OPEN_EXISTING, 
                                    FILE_ATTRIBUTE_NORMAL, 
                                    NULL);
    if (theApp.m_hDrvFile == INVALID_HANDLE_VALUE)
    {
        wsprintf(szBuf, "CreateFile ErrorCode:%d\n", GetLastError());
        AfxMessageBox(szBuf);
        return 0;
    }

	return CWinApp::InitApplication();
}

int CMyTaskmgrApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
    if (m_schService != NULL)
    {
        BOOL bRet = FALSE;
        SERVICE_STATUS svcStatus;
        bRet = ControlService(m_schService, SERVICE_CONTROL_STOP, &svcStatus);
        if (bRet == FALSE)
        {
            //ֹͣ����ʱʧ����ʾ
            CFunction::ShowErrorMsg();
        }
        
        //�����Ƿ�ֹͣ�ɹ���ж����������
        bRet = DeleteService(m_schService);            
        CloseServiceHandle(m_schSCManager);
        CloseServiceHandle(m_schService);
        m_schSCManager = NULL;
        m_schService = NULL;
    }

    //�ͷ��������ûд��
    if (m_hDrvFile != INVALID_HANDLE_VALUE)
    {
        if (m_hDrvFile != NULL)
        {
            CloseHandle(m_hDrvFile);
        }
    }

	return CWinApp::ExitInstance();
}
