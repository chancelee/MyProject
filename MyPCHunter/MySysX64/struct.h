/***************************************************************************************
* AUTHOR : schip
* DATE   : 2018-2-4
* MODULE : struct.h
* 
* Command: 
*   ������ͷ�ļ�
*
* Description:
*   ����һЩ����,�����ظ��Ͷ�; �������ڴ������Ҫ�ĺ���/�ṹ��
*
****************************************************************************************

Copyright (C) 2010 schip.
****************************************************************************************/

#pragma once

#include <ntddk.h> 

typedef long LONG;
typedef unsigned char  BOOL, *PBOOL;
typedef unsigned char  BYTE, *PBYTE;
typedef unsigned long  DWORD, *PDWORD;
typedef unsigned short WORD, *PWORD;

// My def
typedef ULONGLONG QWORD;

typedef void  *HMODULE;
typedef long NTSTATUS, *PNTSTATUS;
typedef unsigned long DWORD;
typedef DWORD * PDWORD;
typedef ULONG *PULONG;
typedef unsigned short WORD;
typedef unsigned char BYTE; 
typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef void *PVOID;
typedef BYTE BOOLEAN;
#define SEC_IMAGE    0x01000000


//�����ṹ��
typedef struct _KAPC_STATE
{
    LIST_ENTRY ApcListHead[2];
    PKPROCESS Process;
    UCHAR KernelApcInProgress;
    UCHAR KernelApcPending;
    UCHAR UserApcPending;
} KAPC_STATE, *PKAPC_STATE;

typedef struct _LDR_DATA_TABLE_ENTRY
{
    LIST_ENTRY64	InLoadOrderLinks;
    LIST_ENTRY64	InMemoryOrderLinks;
    LIST_ENTRY64	InInitializationOrderLinks;
    PVOID			DllBase;
    PVOID			EntryPoint;
    ULONG			SizeOfImage;
    UNICODE_STRING	FullDllName;
    UNICODE_STRING 	BaseDllName;
    ULONG			Flags;
    USHORT			LoadCount;
    USHORT			TlsIndex;
    PVOID			SectionPointer;
    ULONG			CheckSum;
    PVOID			LoadedImports;
    PVOID			EntryPointActivationContext;
    PVOID			PatchInformation;
    LIST_ENTRY64	ForwarderLinks;
    LIST_ENTRY64	ServiceTagLinks;
    LIST_ENTRY64	StaticLinks;
    PVOID			ContextInformation;
    ULONG64			OriginalBase;
    LARGE_INTEGER	LoadTime;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

#define MAXBYTE 0xff

//���������Ϣ�Ľṹ��
typedef struct _tagProcessInfo
{
    UCHAR m_szBuf[MAXBYTE];
    UCHAR m_szFullPath[MAXBYTE];
    __int64 m_dwPID; //x64 �� QWORD
    __int64 qwEPROCESS;
}ProcessInfo, *PProcessInfo;


//ö��ģ��ṹ��
typedef struct _tagEnumModule
{
    ULONGLONG m_qwImageBase;
    ULONG m_ImageSize;
    CHAR szFullPathName[MAXBYTE];
}EnumModule, *PEnumModule;
