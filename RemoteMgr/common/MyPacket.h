// MyPacket.h: interface for the CMyPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYPACKET_H__E12DB514_A0B3_4CB4_B3E4_B8E9721E7C31__INCLUDED_)
#define AFX_MYPACKET_H__E12DB514_A0B3_4CB4_B3E4_B8E9721E7C31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
  ��:
     ----------
     ���ݳ���
     ----------
     ��Ϣ����
     ----------
     ����
     ----------
*/


enum
{
    //���������CMD����
    SVR_STARTUP_CMD,

    //����˷���cmd����
    SVR_CMD, 

    //����˷��ͽ�������
    SVR_SCREEN, 

    //����˷�����갴������
    SVR_LBTNDOWN, 
    
    //����˷�����굯������
    SVR_LBTNUP, 

    //����˷�������ƶ�����
    SVR_MOUSEMOVE, 

    //����˷���˫������
    SVR_DOUBLE_CLICK, 

    //����˷��ͼ��̰�������
    SVR_KEYDOWN,

    //����˷��ͼ��̵�������
    SVR_KEYUP,

    //�ͻ��˷���cmd���
    CLT_CMD, 

    //�ͻ��˷��ؽ�������
    CLT_SCREEN,

    //�ͻ�������֪ͨ
    CLT_OFFLINE
};

class CMyPacket  
{
public:
    CMyPacket();
    virtual ~CMyPacket();
    inline char* GetBuf() const;
    inline int GetLength() const;
    char* Append(char *pNewBuf, int nLength);
private:
    //��������ַ
    char *m_pBuf;

    //��ǰʹ�ô�С
    int m_nLength;

    //�������ܴ�С
    int m_BufSize;
};

inline char* CMyPacket::GetBuf() const
{
    return m_pBuf;
}

inline int CMyPacket::GetLength() const
{
    return m_nLength;
}

class CMySocket
{
public:
    CMySocket(SOCKET s);
    virtual ~CMySocket();
    
    //�������ݰ�CMyPacket& pkt
    int SendPacket(CMyPacket& pkt);

    //��������
    int SendData(char *pBuf, int nLength);

    //�������ݰ�
    int RecvPacket(CMyPacket& pkt);

    //��������
    int RecvData(char *pBuf, int nLength);
private:
    SOCKET m_socket;
};

#endif // !defined(AFX_MYPACKET_H__E12DB514_A0B3_4CB4_B3E4_B8E9721E7C31__INCLUDED_)
