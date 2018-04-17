// AcceptThread.cpp: implementation of the CAcceptThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Server.h"
#include "AcceptThread.h"
#include "ServerDlg.h"
#include "RecvThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAcceptThread::CAcceptThread(LPVOID lParam)
{
    m_lParam = lParam;
}

CAcceptThread::~CAcceptThread()
{
    m_lParam = NULL;
}

BOOL CAcceptThread::Exec()
{
    TRACE("ssc Server CAcceptThread::Exec() begin!");
    CServerDlg *pServerDlg = (CServerDlg *)m_lParam;
    if (pServerDlg->m_sockListen == INVALID_SOCKET)
    {
        return FALSE;
    }
    if (m_lParam == NULL)
    {
        return FALSE;
    }

    int nLength = sizeof(sockaddr);
    sockaddr_in addr;

    while (TRUE)
    {        
        if (pServerDlg->m_sockListen == INVALID_SOCKET)
        {
            return FALSE;
        }

        SOCKET sMid = accept(pServerDlg->m_sockListen, (sockaddr*)&addr, &nLength);    
        if (sMid != INVALID_SOCKET)
        {
            CRecvThread *pRecvThread = new CRecvThread(m_lParam, sMid);
            if (pRecvThread == NULL)
            {
                TRACE("ssc Server CRecvThread new failed!");
                return FALSE;
            }
            
            pServerDlg->m_ThreadPool.Handle(pRecvThread); 

            //������ת�����б�
            pServerDlg->m_LstMid.InsertItem(pServerDlg->m_nMidCount, inet_ntoa(addr.sin_addr));
            char szTmpBuf[MAXBYTE] = {0};
            wsprintf(szTmpBuf, "%d", ntohs(addr.sin_port));
            pServerDlg->m_LstMid.SetItemText(pServerDlg->m_nMidCount, 1, szTmpBuf);
            
            wsprintf(szTmpBuf, "%08X", sMid);
            pServerDlg->m_LstMid.SetItemText(pServerDlg->m_nMidCount, 2, szTmpBuf);
            //������ת������1,��ת�ر�ʱ�ǵü���
            pServerDlg->m_nMidCount++;
        }
        else
        {
            TRACE("ssc Server accept failed!");
            return FALSE;
        }
    }
    
    return TRUE;
}
