/*
 * =====================================================================================
 *
 *       Filename:  MyThread.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2019年12月28日 09时18分09秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "MyThread.h"
#include "NetMgr.h"

MyThread::MyThread() : m_ThreadId(0), m_ThreadHandle(0)
{}

MyThread::~MyThread()
{
}

bool MyThread::start()
{
    //bool res = (ACE_Thread::spawn(&Thread::ThreadTask, (void*)m_task, THREADFLAG, &m_iThreadId, &m_hThreadHandle) == 0);
    bool res = ( ACE_Thread::spawn( &MyThread::RunThread, NULL, THR_JOINABLE | THR_NEW_LWP, &m_ThreadId, &m_ThreadHandle ) == 0 );
    return res;
}

bool MyThread::wait()
{
    bool res = (ACE_Thread::join( m_ThreadId ) == 0);
    m_ThreadId = 0;
    m_ThreadHandle = 0;
    return res;
}

bool MyThread::stop()
{
    if ( !m_ThreadId )
    {
        return false;
    }
    bool res = (ACE_Thread::kill( m_ThreadId, -1 ) == 0);
    return res;
}

void MyThread::suspend()
{
    ACE_Thread::suspend( m_ThreadId );
}

void MyThread::resume()
{
    ACE_Thread::resume( m_ThreadId );
}

void* MyThread::RunThread( void* arg )
{

    return NULL;
}

void MyThread::AddPacket( Packet* pkg )
{
    if( !pkg ) return ;
    m_PkgQueue.add( pkg );
}
