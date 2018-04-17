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
    //�ͻ�������֪ͨ
    CLT_ONLINE,

    //�ͻ�������֪ͨ
    CLT_OFFLINE,

    //��ת����֪ͨ
    MID_ONLINE,

    //��ת����֪ͨ
    MID_OFFLINE,

    //��ѯ���
    SQL_SELECT,

    //ɾ�����
    SQL_DEL,

    //�������
    SQL_ADD,

    //�������
    SQL_UPDATE,

    //���صĲ�ѯ���
    SQL_RESULT,

    //��ɾ�ĵ�ִ�гɹ�
    SQL_SUCCESS
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
