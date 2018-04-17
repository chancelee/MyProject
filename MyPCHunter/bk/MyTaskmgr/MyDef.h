#if !defined(AFX_MYDEF_H__0E5E0B98_89D6_4FDF_88D7_7C4CCA0BE8B7__INCLUDED_)
#define AFX_MYDEF_H__0E5E0B98_89D6_4FDF_88D7_7C4CCA0BE8B7__INCLUDED_

#define DRV_SVC_NAME "MySys"

#define DRV_FILE DRV_SVC_NAME##".sys"
#define DRV_FILE_X64 DRV_SVC_NAME##"X64"##".sys"

typedef struct _CONTORL_PARAMS
{
    ULONG CPUNumber;
    DWORD dwSave;
}CONTORL_PARAMS, *PCONTORL_PARAMS;

typedef struct _KERNELMODULE_INFO
{
    char m_szDriverName[MAXBYTE]; //������
    DWORD m_dwBaseAddress;
    DWORD m_dwSize;
    DWORD m_DeviceObject;
    char m_szDriverPath[MAXBYTE]; //����·��
    char m_szServiceName[MAXBYTE]; //����·��
    DWORD m_dwLoadIndex; //����˳��
    char m_szFileCompany[MAXBYTE]; //�ļ�����
}KERNELMODULE_INFO, *PKERNELMODULE_INFO;

#endif // !defined(AFX_MYDEF_H__0E5E0B98_89D6_4FDF_88D7_7C4CCA0BE8B7__INCLUDED_)