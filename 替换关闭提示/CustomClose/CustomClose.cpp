// CustomClose.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "tlhelp32.h"

//��ֻ����StrStrI
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

//1000D1B3 �� 1000903F ��������ַ������Hook
//1000D1B3  |  E8 17740000     call    100145CF
//1000903F  |  E8 38410000     call    1000D17C

#define FUNOFFSET1 0xD1B3
#define FUNOFFSET2 0x903F

//���wise�ͷŵ�GLCxxx.tmp�ľ��
//GLCxxx.tmp����wise��װĿ¼�µ�WISE0132.dll
//������ش��붼д�����dll����
HANDLE GetGLCHandle()
{
    HANDLE  hProcessSnap = NULL;
    HANDLE  hRet = NULL;
    MODULEENTRY32 me32 = { 0 };
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }
    
    me32.dwSize = sizeof(MODULEENTRY32);
    
    if (Module32First(hProcessSnap, &me32))
    {
        do
        {
            if (StrStrI(me32.szModule, "GLC") != NULL)
            {
                return me32.hModule;
            }
        } while (Module32Next(hProcessSnap, &me32));
    }
    else
    {
        hRet = FALSE;  
    } 
    CloseHandle(hProcessSnap);
    return hRet;
}

void MyFun()
{
    MessageBox(NULL, "Fuck!!!!", NULL, MB_OK);
    ExitProcess(0);
    //�����Ҫ�����ӵĹ��ܣ����ԣ�
    //LoadLibrary
    //GetProcAddress()
}

DWORD SetNewAddr(char *pOld, char *pNew)
{
    BOOL bRet = FALSE;
    char szBuf[MAXBYTE] = {0};

    //�޸��ڴ�ҳ����Ϊ�ɶ���д��ִ��
    DWORD OldProtect = 0;
    bRet = VirtualProtect((LPVOID)pOld, 
                            0x1000, 
                            PAGE_EXECUTE_READWRITE, 
                            &OldProtect);

    if (bRet == FALSE)
    {
        wsprintf(szBuf, "VirtualProtect failed [%d]", GetLastError());
        OutputDebugString(szBuf);
        return FALSE;
    }
    
    //addr = Ŀ���ַ - (ԭ��ַ + 5 )
    DWORD dwNewOffset = (DWORD)pNew - ((DWORD)pOld + 5);
    //pOld[0] = 0xE8; //��һ���ֽھ���CALL���ù�

    //���µ�ƫ�Ƶ�ַд�� call ָ�����
    memcpy(&pOld[1], &dwNewOffset, sizeof(DWORD));
    

    //��ԭ�ڴ�ҳ����
    bRet = VirtualProtect((LPVOID)pOld, 
                            0x1000, 
                            OldProtect, 
                            &OldProtect);
    
    if (bRet == FALSE)
    {
        wsprintf(szBuf, "VirtualProtect failed [%d]", GetLastError());
        OutputDebugString(szBuf);
        return FALSE;
    }

    return TRUE;
}


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    char szBuf[MAXBYTE] = {0};

    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        wsprintf(szBuf, "dll load!!");
        OutputDebugString(szBuf);

        //������Ҫ��д�ĺ�����ַ
        HANDLE hGLC = GetGLCHandle();
        DWORD dwFunAddr = (DWORD)hGLC + FUNOFFSET1;

        //�����µ�ַ
        SetNewAddr((char *)dwFunAddr, (char *)MyFun);

    }
    if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {
        wsprintf(szBuf, "dll free!!");
        OutputDebugString(szBuf);
	}

    return TRUE;
}

