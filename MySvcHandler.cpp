/*
 * =====================================================================================
 *
 *       Filename:  MySvcHandler.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2019年12月27日 16时11分11秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "MySvcHandler.h"
#include "NetMgr.h"


MyAceSvcHandler::MyAceSvcHandler() :
    m_dwSockId(0),
    m_MsgHeader( sizeof( PacketHeader ) ),
    m_MsgBody(),
    m_bOutActive( true )
{
    msg_queue()->high_water_mark( 2 * 1024 );
    msg_queue()->low_water_mark( 2 * 1024 );
}

MyAceSvcHandler::~MyAceSvcHandler()
{
   sNetMgrInstance->OnSocketClose(this);
   closing_ = true;
   peer().close();
   std::cout<<"~MyAceSvcHandler()"<<std::endl;
}

int MyAceSvcHandler::Update()
{
    //todo
    std::cout<<"MyAceSvcHandler::Update"<<std::endl;
    return 0;
}

void MyAceSvcHandler::CloseSock()
{
    if (closing_)
    {
        return;
    }
    closing_ = true;
    peer().close_writer();
}

bool MyAceSvcHandler::IsClosed() const
{
    return closing_;
}

int MyAceSvcHandler::open( void* arg )
{
    m_bOutActive = true;
    if ( NetMgr::GetInstance()->OnSocketOpen( this ) == -1 )
    {
        return -1;
    }
    if ( ACE_Reactor::instance()->register_handler( this, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::WRITE_MASK )  == -1)
    {
        return -1;
    }
    std::cout<<"MyAceSvcHandler::open"<<std::endl;
    return 0;
}

int MyAceSvcHandler::handle_output( ACE_HANDLE fd )
{
    //not out now
    printf("errno: %s(errno: %d)\n",strerror(errno),errno);
    if ( -1 == reactor()->cancel_wakeup(this, ACE_Event_Handler::WRITE_MASK) )
    {
        return -1;
    }
    m_bOutActive = false;
//    if ( -1 == reactor()->cancel_wakeup(this, ACE_Event_Handler::READ_MASK) )
//    {
//        return -1;
//    }

    return 0;
}

int MyAceSvcHandler::handle_input( ACE_HANDLE fd )
{
    if ( closing_ )
    {
        return -1;
    }
    char buf [4096] = {0};

     ACE_Data_Block db(sizeof(buf),
                       ACE_Message_Block::MB_DATA,
                       buf,
                       0,
                       0,
                       ACE_Message_Block::DONT_DELETE,
                       0);
     ACE_Message_Block message_block(&db,
                                     ACE_Message_Block::DONT_DELETE,
                                     0);
     const ssize_t msg_len = message_block.space();
     const ssize_t n = peer().recv( message_block.wr_ptr(), msg_len );
     //disconnect
     if ( n <= 0 )
     {
         return -1;
     }
     message_block.wr_ptr( n );
     while( message_block.length() > 0 )
     {
         //msg header
         if ( m_MsgHeader.space() > 0 )
         {
             const size_t cur_header_len = ( message_block.length() > m_MsgHeader.space() ) ? m_MsgHeader.space() : message_block.length();
             m_MsgHeader.copy( message_block.rd_ptr(), cur_header_len  );
             message_block.rd_ptr( cur_header_len );
             if ( m_MsgHeader.space() > 0 )
             {
                 //Couldn`t recv the whole header this time
                 assert( message_block.length() == 0 );
                 errno = EWOULDBLOCK;
                 break;
             }

             //already received the whole header
              assert( sizeof( PacketHeader ) == m_MsgHeader.length() );
              PacketHeader& header = *( (PacketHeader*)m_MsgHeader.rd_ptr() );
              header.size -= 8;
              if ( header.size > 0 )
              {
                  m_MsgBody.base( message_block.rd_ptr(), header.size );
              }
              else
              {
                  assert( m_MsgBody.space() == 0 );
              }
         }

         //msg body
         if ( m_MsgBody.space() > 0 )
         {
             const size_t cur_body_len = (message_block.length() > m_MsgBody.space()) ? m_MsgBody.space() : message_block.length();
             m_MsgBody.copy( message_block.rd_ptr(), cur_body_len );
             message_block.rd_ptr( cur_body_len );

             if ( m_MsgBody.space() > 0 )
             {
                 assert( message_block.length() == 0 );
                 errno = EWOULDBLOCK;
                 break;
             }
         }
         //got one full msg
//         assert( m_MsgHeader.space() == 0 );
//         assert( m_MsgBody.space() == 0 );
         if ( ProcessPkg() == -1 )
         {
             return -1;
         }
         m_MsgHeader.reset();
         m_MsgBody.base( NULL, 0 );

     }
     return 0;
}

int MyAceSvcHandler::ProcessPkg( )
{
    assert( m_MsgHeader.space() == 0 );
    assert( m_MsgBody.space() == 0 );
    assert( m_MsgHeader.length() == sizeof(PacketHeader) );
    Packet* pkg = new Packet;
    pkg->msg_header = *( (PacketHeader*)m_MsgHeader.rd_ptr() );
    pkg->msg_header.size -= 8;
    pkg->msg_body = m_MsgBody.rd_ptr();

    sNetMgrInstance->AddPacket( pkg );
    //todo
    std::cout<<"pkg_id="<<pkg->msg_header.cmd<<std::endl<<std::cout<<"pkg_size="<<pkg->msg_header.size<<":";
    std::cout<<"pkg_msg="<<pkg->msg_body<<std::endl;

    return 0;
}

 int MyAceSvcHandler::handle_close(ACE_HANDLE h , ACE_Reactor_Mask )
{
     closing_ = true;
     ACE_Reactor::instance()->remove_handler( this, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::WRITE_MASK | ACE_Event_Handler::DONT_CALL );
     //std::cout<<"client closed"<<std::endl;
     return 0;
}
