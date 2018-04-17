// Function.cpp: implementation of the CFunction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Function.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma comment(lib, "Shlwapi.lib")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFunction::CFunction()
{

}

CFunction::~CFunction()
{

}

/*
typedef struct _tagGDT 
{
    unsigned __int64 m_LimiteLow: 16;
    unsigned __int64 m_BaseLow: 24;
    
    //Attributes
    unsigned __int64 m_TYPE:  4; 
    unsigned __int64 m_DT1:   1;
    unsigned __int64 m_DPL:   2;
    unsigned __int64 m_P:     1;
    unsigned __int64 m_LimiteHigh:  4; 
    unsigned __int64 m_AVL:  1; 
    unsigned __int64 m_Saved:  1; 
    unsigned __int64 m_D:  1; 
    unsigned __int64 m_G:  1; 
    
      unsigned __int64 m_BaseHigh: 8;
}GDT, *PGDT;
*/

DWORD CFunction::GetBaseFormGDT(PGDT pValue, DWORD *pdwBase)
{
    if (pValue == NULL || pdwBase == NULL)
    {
        return FALSE;
    }
    DWORD dwBaseLow = (DWORD)(pValue->m_BaseLow);
    DWORD dwBaseHigh = (DWORD)((pValue->m_BaseHigh) << 24);

    *pdwBase = dwBaseLow + dwBaseHigh;

    return TRUE;
}

DWORD CFunction::GetLimiteFormGDT(PGDT pValue, DWORD *pwLimite)
{
    if (pValue == NULL || pwLimite == NULL)
    {
        return FALSE;
    }

    DWORD dwLimiteLow = (DWORD)(pValue->m_LimiteLow);
    DWORD dwLimiteHigh = (DWORD)((pValue->m_LimiteHigh) << 16);

    *pwLimite = dwLimiteLow + dwLimiteHigh;

    return TRUE;
}

DWORD CFunction::GetAttribFormGDT(PGDT pValue, WORD *pwAttrib)
{
    if (pValue == NULL || pwAttrib == NULL)
    {
        return FALSE;
    }

    *pwAttrib = *(WORD*)((char *)pValue + 5);

    return TRUE;
}

DWORD CFunction::GetDLP(WORD wAttrib)
{
    PATTRIB pAttrib = (PATTRIB)(&wAttrib);
    return pAttrib->m_DPL;
}

//�����ж��Ǵ洢��(1)����ϵͳ��(0)
DWORD CFunction::GetDT(WORD wAttrib)
{
    PATTRIB pAttrib = (PATTRIB)(&wAttrib);
    return pAttrib->m_DT1;
}
    

char *CFunction::GetG(WORD wAttrib)
{
    PATTRIB pAttrib = (PATTRIB)(&wAttrib);
    if (pAttrib->m_G == 1)
    {
        return "Page"; //4k
    }
    else
    {
        return "Byte"; 
    }
}

char *g_pDataszBuf[] = {
    "Read-Only",
    "Read-Only AC",
    "Read/Write",
    "Read/Write AC",
    "Read-Only ED",
    "Read-Only ED AC",
    "Read/Write ED",
    "Read/Write ED AC"  //ʵ���ϲ���AC�ĸ���������ʾ
};

char *g_pCodeszBuf[] = {
    "Execute-Only",
    "Execute-Only AC",
    "Execute/Read",
    "Execute/Read AC",
    "Execute-Only conforming",
    "Execute-Only conforming AC",
    "Execute/Read-Only conforming",
    "Execute/Read-Only conforming AC" //ʵ���ϲ���AC�ĸ���������ʾ     
};

char *g_pSysszBuf[] = {
    "Reserved",
    "16-Bit TSS (Available)",
    "LDT",
    "16-Bit TSS (Busy)",
    "16-Bit Call Gate",
    "Task Gate",
    "16-Bit Interrupt Gate",
    "16-Bit Trap Gate",
    "Reserved",
    "32-Bit TSS (Available)",
    "Reserved",
    "32-Bit TSS (Busy)",
    "32-Bit Call Gate",
    "Reserved",
    "32-Bit Interrupt Gate",
    "32-Bit Trap Gate"
};

char *CFunction::GetTypeString(WORD wAttrib)
{
    UCHAR chType = GetTypeValue(wAttrib);

    //���ж���ϵͳ�λ��Ǵ洢��
    DWORD dwType = GetDT(wAttrib);
    if (dwType == 0)
    {
        return g_pSysszBuf[GetTypeValue(wAttrib)];
    }
    
    //ȡ Type �� ��4λ(�±�3)
    DWORD dwDataOrCode = 0;
    dwDataOrCode = chType >> 3;

    //��ȡ Type ��������Ϣ
    DWORD dwDescription = 0;
    dwDescription = chType & 7;

    if (dwDataOrCode == 1) //�����
    {
        return g_pCodeszBuf[dwDescription];
    }
    else //���ݶ�
    {
        return g_pDataszBuf[dwDescription];
    }
}

UCHAR CFunction::GetTypeValue(WORD wAttrib)
{
    return wAttrib & 0xF;
}



DWORD CFunction::AnalyseGDT(const char *szGDTBuf,
                            DWORD *pdwBase,
                            DWORD *pwLimite,
                            WORD *pwAttributes)
{
    if (szGDTBuf == NULL)
    {
        return FALSE;
    }

    if (strlen(szGDTBuf) > 16)
    {
        return FALSE;
    }

    DWORD dwRet = 0;
    
    QWORD qwValue = 0;
    char szHexBuf[MAXBYTE] = {0};

    //Ϊ��ת 16����ǰ�����0x
    wsprintf(szHexBuf, "0x%s", szGDTBuf);
    dwRet = StrToInt64ExA(szHexBuf, STIF_SUPPORT_HEX, &qwValue);
    if (dwRet == FALSE)
    {
        return FALSE;
    }

    dwRet = GetBaseFormGDT((PGDT)&qwValue, pdwBase);
    if (dwRet == FALSE)
    {
        return FALSE;
    }

    dwRet = GetLimiteFormGDT((PGDT)&qwValue, pwLimite);
    if (dwRet == FALSE)
    {
        return FALSE;
    }

    dwRet = GetAttribFormGDT((PGDT)&qwValue, pwAttributes);
    if (dwRet == FALSE)
    {
        return FALSE;
    }

    return TRUE;
}

void CFunction::ShowErrorMsg()
{
    //��ȡ���������Ϣ
    LPVOID lpMsgBuf;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM | 
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        (LPTSTR) &lpMsgBuf,
        0,
        NULL 
        );//�����MSDN�е����ӣ�����ֱ������
    
    SetTip((char *)lpMsgBuf);
}

void CFunction::SetTip(char *szBuf)
{
    AfxMessageBox(szBuf);
}

DWORD CFunction::GetNumberOfProcessors()
{
    //���CPU������
    SYSTEM_INFO SystemInfo;
    GetSystemInfo(&SystemInfo);
    
    return SystemInfo.dwNumberOfProcessors;

}

//IDT�л�ȡ������ַ
DWORD CFunction::GetOffsetFormIDT(PIDT pValue, DWORD *pdwOffset)
{
    if (pValue == NULL || pdwOffset == NULL)
    {
        return FALSE;
    }

    DWORD  dwLow = (DWORD)(pValue->m_OffsetLow);
    DWORD  dwHigh = (DWORD)((pValue->m_OffsetHigh) << 16);
    *pdwOffset = dwLow + dwHigh;

    return TRUE;
}

//IDT�л�ȡѡ����
DWORD CFunction::GetSelectorFormIDT(PIDT pValue, DWORD *pdwSelector)
{
    if (pValue == NULL || pdwSelector == NULL)
    {
        return FALSE;
    }
    *pdwSelector = (DWORD)(pValue->m_Selector_Index);
    return TRUE;
}