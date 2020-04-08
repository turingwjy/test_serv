/*
 * =====================================================================================
 *
 *       Filename:  NetMgr.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2019年12月23日 22时18分49秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "NetMgr.h"

ACE_Thread_Mutex NetMgr::s_mutex;

NetMgr::NetMgr():
m_dwConnNums(0),
m_pAcceptor(NULL),
m_pConnector(NULL),
m_port(0),
m_Threads(NULL) ,
m_Reactor(NULL),
m_dwConnCounts(0),
m_pResponseThread(NULL),
m_bIsRunning(0)
{

}

NetMgr::~NetMgr()
{
    delete m_pAcceptor; m_pAcceptor = NULL;
    delete m_pConnector; m_pConnector = NULL;
    delete m_pResponseThread; m_pResponseThread = NULL;
}

bool NetMgr::StartNetWork( uint16 port, const char* addr )
{
   //    ACE_Time_Value val( 0, 50000 );
   //    while( !ACE_Reactor::instance()->reactor_event_loop_done() )
   //    {
   //        if ( -1 == ACE_Reactor::instance()->run_reactor_event_loop(val ) )
   //        {
   //            break;
   //        }
   //    }

    m_port = port;
    m_strAddr = std::string(addr);
    m_Reactor = ACE_Reactor::instance();
    ACE_INET_Addr _addr( port, addr );
    MyAcceptor* acc = new MyAcceptor( _addr, m_Reactor );
    m_pAcceptor = acc;
    m_pResponseThread = new MyThread;
    //
    std::cout<<"Start TestServer"<<std::endl;
    ACE_Time_Value val( 0, 50000 );


    m_pResponseThread->start();
    m_bIsRunning = 1;
    //main loop
    while( !m_Reactor->reactor_event_loop_done() )
    {
        if ( -1 == m_Reactor->run_reactor_event_loop(val ) )
        {
            break;
        }
        Update();
    }
    m_bIsRunning = 0;
    //m_Threads = new MyThread[2];// 0for read 1for write

    std::cout<<"TestServer Exit"<<std::endl;
    return true;
}

void NetMgr::StopNetWorld()
{
    if( m_pAcceptor )
    {
        MyAcceptor* acc = dynamic_cast<  MyAcceptor*> (m_pAcceptor);
        acc->close();

    }
    if ( m_pConnector )
    {
        MyConnector* con = dynamic_cast< MyConnector* >(m_pConnector);
        con->close();
    }
//    for ( int i = 0; i < 2; ++i )
//    {
//        m_Threads[i].stop();
//    }
    m_bIsRunning = 0;
    m_pResponseThread->wait();
}

void NetMgr::Wait()
{
//    for ( int i = 0; i < 2; ++i )
//    {
//        m_Threads[i].wait();
//    }
    m_pResponseThread->wait();
}

int NetMgr::ProcessMsg()
{
    for ( SetSockets::iterator it = m_setSockets.begin(); it != m_setSockets.end(); ++it )
    {
        //if ( !(*it)->IsClosed() && (*it)->m)
    }
    return 0;
}

void NetMgr::AddPacket( Packet* pkg )
{
    std::cout<<"cmd_id:"<<pkg->msg_header.cmd<<
            "pkg_size:"<<pkg->msg_header.size<<
            "pkg:"<<pkg->msg_body<<std::endl;
}

void NetMgr::Update()
{
    _LOCK<ACE_Thread_Mutex> lock;
    if( m_setSockets.empty() )
    {
        return;
    }
    for ( SetSockets::iterator it = m_setSockets.begin(); it != m_setSockets.end();  )
    {
        assert( *it );
        if( (*it)->Update() )
        {
            ++it;
        }
        else
        {
            (*it)->CloseSock();
            --m_dwConnNums;
            m_setSockets.erase(it++);
        }
    }
}


int NetMgr::OnSocketOpen( MyAceSvcHandler* pSocket )
{
    if ( !pSocket )
    {
        return -1;
    }
    if ( pSocket->IsClosed() )
    {
        return -1;
    }
    _LOCK<ACE_Thread_Mutex> lock;
    //if (sock->peer().set_option(SOL_SOCKET, SO_SNDBUF, (void*)&m_SockOutKBuff, sizeof(int)) == -1 && errno != ENOTSUP)
    int sndBuf = -1;
    if ( pSocket->peer().set_option( SOL_SOCKET, SO_SNDBUF, (void*)&sndBuf, sizeof(int) ) == -1 && errno != ENOTSUP )
    {
        std::cout<<"set_option error"<<std::endl;
        return -1;
    }
    m_setSockets.insert( pSocket );
    pSocket->m_dwSockId = ++m_dwConnCounts;
    ++m_dwConnNums;
    return 0;
}

void NetMgr::OnSocketClose(MyAceSvcHandler* pSocket)
{
    _LOCK<ACE_Thread_Mutex> lock;
    SetSockets::iterator it = m_setSockets.find( pSocket );
    if ( it == m_setSockets.end() )
    {
        return;
    }
    if ( pSocket->m_dwSockId )
    {
        m_setSockets.erase( it );
        --m_dwConnNums;
    }
}

NetMgr::SetSockets& NetMgr::GetSetSockets() const
{
    return m_setSockets;
}

