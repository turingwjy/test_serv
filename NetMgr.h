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

#include <ace/Acceptor.h>
#include <ace/Connector.h>
#include <ace/Guard_T.h>
#include "MySvcHandler.h"
#include "MyThread.h"
#include <set>
template <typename _MUTEX_TYPE>
class _LOCK

{
public:
    _LOCK(_MUTEX_TYPE& mutex) : _mutex(&mutex)
    {
        _mutex->acquire();
    }
    ~_LOCK()
    {
        _mutex->release();
    }
private:
    _MUTEX_TYPE* _mutex;
};

class NetMgr
{
public:
    friend class MyAceSvcHandler;
public:
    typedef std::set< MyAceSvcHandler* > SetSockets;
    static NetMgr* GetInstance()
    {
        return s_pInstance;
    }
    bool StartNetWork( uint16 port, const char* addr );
    void StopNetWorld();
    int OnSocketOpen( MyAceSvcHandler* pSocket );
    void OnSocketClose(MyAceSvcHandler* pSocket);
    void Update();
    void Wait();
    int ProcessMsg();
    void AddPacket(Packet* pkg);
    bool IsStop()const;
    //const SetSockets& GetSetSockets() const;
private:
    NetMgr();
    virtual ~NetMgr();
    static ACE_Thread_Mutex s_mutex;
    MyThread*   m_pResponseThread;
private:
    static  NetMgr*     s_pInstance;
    uint16              m_port;
    std::string         m_strAddr;
    SetSockets          m_setSockets;
    uint32              m_dwConnNums;
    uint32              m_dwConnCounts;
    ACE_Event_Handler*  m_pAcceptor;
    ACE_Event_Handler*  m_pConnector;
    MyThread*           m_Threads;
    ACE_Reactor*        m_Reactor;
    bool                m_bIsStop;
};


#define sNetMgrInstance NetMgr::GetInstance()

#endif

