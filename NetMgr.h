/*
 * =====================================================================================
 *
 *       Filename:  NetMgr.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2019年12月23日 22时18分10秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _NET_MGR_H
#define _NET_MGR_H
#include <ace/Thread_Mutex.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Connector.h>
#include <ace/Acceptor.h>
#include <ace/Connector.h>
#include "MySvcHandler.h"
#include "MyThread.h"
#include <set>
template <typename _MUTEX_TYPE>
class _LOCK

{
public:
    _LOCK(_MUTEX_TYPE& mutex) : _mutex(mutex)
    {
        _mutex.acquire();
    }
    ~_LOCK()
    {
        _mutex.release();
    }
private:
    _MUTEX_TYPE _mutex;
};

class NetMgr
{
public:
    friend class MyAceSvcHandler;
public:
    typedef std::set< MyAceSvcHandler* > SetSockets;
    static NetMgr* GetInstance()
    {
        _LOCK<ACE_Thread_Mutex> lock( s_mutex );
        static NetMgr _mgr;
        return &_mgr;
    }
    bool StartNetWork( uint16 port, const char* addr );
    void StopNetWorld();
    int OnSocketOpen( MyAceSvcHandler* pSocket );
    void OnSocketClose(MyAceSvcHandler* pSocket);
    void Update();
    void Wait();
    int ProcessMsg();
private:
    NetMgr();
    virtual ~NetMgr();
    static ACE_Thread_Mutex s_mutex;
    MyThread*   m_pThread;
private:
    uint16              m_port;
    std::string         m_strAddr;
    SetSockets          m_setSockets;
    uint32              m_dwConnNums;
    uint32              m_dwConnCounts;
    ACE_Event_Handler*  m_pAcceptor;
    ACE_Event_Handler*  m_pConnector;
    MyThread*           m_Threads;
    ACE_Reactor*        m_Reactor;
};

#define sNetMgrInstance NetMgr::GetInstance():

#endif

