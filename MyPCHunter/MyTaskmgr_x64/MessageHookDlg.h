#if !defined(AFX_MESSAGEHOOKDLG_H__005E8092_F44E_4FE8_9E5C_CF23098A97B8__INCLUDED_)
#define AFX_MESSAGEHOOKDLG_H__005E8092_F44E_4FE8_9E5C_CF23098A97B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MessageHookDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMessageHookDlg dialog


typedef struct _HANDLEENTRY
{
    UINT64	phead;
    UINT64	pOwner;
    UCHAR	bType;
    UCHAR	bFlags;
    USHORT	wUniq;
}HANDLEENTRY,*PHANDLEENTRY;


typedef struct _HOOK_INFO
{
    HANDLE       hHandle;                    //���ӵľ��   �����ȫ�ֵ� ����UnhookWindowsHookEx  �ѹ���ж��
    int          Unknown1;
    PVOID        Win32Thread;                 //һ��ָ�� win32k!_W32THREAD �ṹ���ָ��
    PVOID        Unknown2;
    PVOID        SelfHook;                   //ָ��ṹ����׵�ַ
    PVOID        NextHook;                   //ָ����һ�����ӽṹ��
    int          iHookType;                 //���ӵ����ͣ� winuser.h ���ж���
    PVOID        OffPfn;                    //���Ӻ����ĵ�ַƫ�ƣ����������ģ���ƫ��
    int          iHookFlags;
    int          iMod;                      //���Ӻ�������ģ����������룬ͨ����ѯ WowProcess �ṹ���Եõ�ģ��Ļ���ַ��
    PVOID        Win32ThreadHooked;         // �������������̵߳Ľṹָ�룬��֪��
    //���滹�У�ʡ�ԡ�����
} HOOK_INFO,*PHOOK_INFO;

class CMessageHookDlg : public CDialog
{
// Construction
public:
	CMessageHookDlg(CWnd* pParent = NULL);   // standard constructor

    void RKM(UINT64 Address, PVOID Buffer, SIZE_T Length);
    void WKM(UINT64 Address, PVOID Buffer, SIZE_T Length);
    UINT64 GetQWORD(UINT64 address);
    UINT32 GetDWORD(UINT64 address);
    PUCHAR GetPNbyET(UINT64 ethread);
    char *GetHookType(int Id);
    char *GetHookFlagString(int Flag);
    void EnumMsgHook();

    DWORD CTL_CODE_GEN(DWORD lngFunction)
    {
        //const DWORD FILE_DEVICE_UNKNOWN = 0x22;
        //const DWORD METHOD_BUFFERED = 0;
        //const DWORD FILE_ANY_ACCESS = 0;
        return (FILE_DEVICE_UNKNOWN * 65536) | (FILE_ANY_ACCESS * 16384) | (lngFunction * 4) | METHOD_BUFFERED;
    }
    
    BOOL IoControl(HANDLE hDrvHandle, DWORD dwIoControlCode, PVOID lpInBuffer, DWORD nInBufferSize, PVOID lpOutBuffer, DWORD nOutBufferSize)
    {
        DWORD lDrvRetSize;
        return DeviceIoControl(hDrvHandle, dwIoControlCode, lpInBuffer, nInBufferSize, lpOutBuffer, nOutBufferSize, &lDrvRetSize, 0);
    }


// Dialog Data
	//{{AFX_DATA(CMessageHookDlg)
	enum { IDD = DLG_MSGHOOK };
	CListCtrl	m_LstMsgHook;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMessageHookDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMessageHookDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESSAGEHOOKDLG_H__005E8092_F44E_4FE8_9E5C_CF23098A97B8__INCLUDED_)
