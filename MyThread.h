/*
 * =====================================================================================
 *
 *       Filename:  MyThread.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2019年12月28日 09时17分49秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _MY_THREAD_H
#define _MY_THREAD_H

#include <ace/Thread.h>
#include "MySvcHandler.h"
#include "LockedQueue.h"
class MyThread
{

public:
    typedef std::set< MyAceSvcHandler* > SetSockets;
    MyThread();
    ~MyThread();
public:
    bool start();
    bool wait();
    bool stop();
    void suspend();
    void resume();
    void AddPacket( Packet* pkg );
private:
    static void* RunThread( void* arg );
private:
    ACE_thread_t        m_ThreadId;
    ACE_hthread_t       m_ThreadHandle;
    ACE_Thread_Mutex    m_SockLock;
    LockedQueue< Packet*, ACE_Thread_Mutex >       m_PkgQueue;
};

#endif
