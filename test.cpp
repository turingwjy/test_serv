/*
 * =====================================================================================
 *
 *       Filename:  test.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年12月16日 16时37分09秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <iostream>
#include <list>
#include <vector>
#include <algorithm>

using namespace std;

//#include <tr1/memory>
//class Test
//{
//public:
//    Test()
//    {
//        std::cout<<"Test"<<std::endl;
//    }
//    ~Test()
//    {
//        std::cout<<"~Test"<<std::endl;
//    }
//};
//
//void Deleter( Test* t )
//{
//    std::cout<<"Call Deleter()"<<std::endl;
//    delete t;
//    t = NULL;
//}
//
//int main()
//{
//    Test* p = new Test();
//    std::tr1::shared_ptr< Test > ptrTest ( p, Deleter);
//    return 0;
//}


#define override
#define DATA_SIZE 1024
#include <ace/Message_Block.h>

//#include "NetMgr.h"
//struct PacketHeader
//{
//    uint32  size;
//    uint32  cmd;
//};
//
//struct PacketBody
//{
//    std::string str;
//};
//
//class MyAceSvcHandler : public ACE_Svc_Handler< ACE_SOCK_STREAM, ACE_NULL_SYNCH >
//{
//public:
//    MyAceSvcHandler() : data(NULL)
//    {
//        data = new char[1024];
//        msg_queue()->high_water_mark( 2 * 1024 );
//        msg_queue()->low_water_mark( 2 * 1024 );
//        std::cout<<"MyAceSvcHandler()"<<std::endl;
//    }
//    virtual ~MyAceSvcHandler()
//    {
//        if ( data != NULL )
//        {
//            delete []data;
//            data = NULL;
//        }
//        std::cout<<"~MyAceSvcHandler()"<<std::endl;
//    }
//    virtual int open( void* arg ) override;
//
//    virtual int handle_input(ACE_HANDLE fd) override;
//
//    virtual int handle_output( ACE_HANDLE fd ) override;
//
//    virtual int handle_close(ACE_HANDLE h = ACE_INVALID_HANDLE, ACE_Reactor_Mask mask = ACE_Event_Handler::ALL_EVENTS_MASK ) override;
//
//private:
//    char *data;
//};
//
//int MyAceSvcHandler::open( void* arg )
//{
//    if ( ACE_Reactor::instance()->register_handler( this, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::WRITE_MASK )  == -1)
//    {
//        return -1;
//    }
//    std::cout<<"MyAceSvcHandler::open"<<std::endl;
//    return 0;
//}
//
//int MyAceSvcHandler::handle_output( ACE_HANDLE fd )
//{
//    printf("errno: %s(errno: %d)\n",strerror(errno),errno);
//    if ( -1 == reactor()->cancel_wakeup(this, ACE_Event_Handler::WRITE_MASK) )
//    {
//        return -1;
//    }
////    if ( -1 == reactor()->cancel_wakeup(this, ACE_Event_Handler::READ_MASK) )
////    {
////        return -1;
////    }
//
//    return 0;
//}
//
//int MyAceSvcHandler::handle_input( ACE_HANDLE fd )
//{
//    if ( closing_ )
//    {
//        return -1;
//    }
////    ACE_Message_Block *pBlock = new ACE_Message_Block(15);
////    if( msg_queue()->enqueue_tail( pBlock, (ACE_Time_Value*)&ACE_Time_Value::zero ) == -1 )
////    {
////        pBlock->release();
////    }
//    size_t n = peer().recv_n( data, DATA_SIZE );
//    if ( n == 0 )
//    {
//        std::cout<<"error = "<<errno<<"lose connection"<<std::endl;
//        return -1;
//    }
//    std::cout<<"recv bytes num:"<<n<<std::endl;
//    ACE_OS::printf( "recv data: %s\n", data );
//    memset( data, sizeof(data), '\0' );
//    return 0;
//}
//
// int MyAceSvcHandler::handle_close(ACE_HANDLE h , ACE_Reactor_Mask )
//{
//     ACE_Reactor::instance()->remove_handler( this, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::WRITE_MASK | ACE_Event_Handler::DONT_CALL );
//     std::cout<<"client closed"<<std::endl;
//     return 0;
//}
//
//typedef ACE_Acceptor< MyAceSvcHandler, ACE_SOCK_ACCEPTOR > MyAcceptor;
//
//int func( ACE_Reactor* reactor )
//{
//    std::cout<<"func"<<std::endl;
//    return 0;
//}
#include <typeinfo>
#include <map>
class ARG_TYPE_BASE
{
public:
    virtual ~ARG_TYPE_BASE(){}
    virtual void Show() = 0;
    virtual void SetArgName( const std::string& str )
    {
        if ( str != "" )
            m_ArgName = str;
    }
    std::string& ArgName(){return m_ArgName;}
private:
    std::string m_ArgName;
};

class ART_TYPE_A : public ARG_TYPE_BASE
{
public:
    virtual ~ART_TYPE_A(){}
    virtual void Show()
    {
        std::cout<<"This is ART_TYPE_A"<<std::endl;
    }
    virtual void SetArgName( const std::string& str )
    {
        ArgName() += str+"A";
    }
};

class ART_TYPE_B : public ARG_TYPE_BASE
{
public:
    virtual ~ART_TYPE_B(){}
    virtual void Show()
    {
        std::cout<<"This is ART_TYPE_B"<<std::endl;
    }
    virtual void SetArgName( const std::string& str )
    {
        ArgName() += str+"B";
    }
};

class ART_TYPE_C : public ARG_TYPE_BASE
{
public:
    virtual ~ART_TYPE_C(){}
    virtual void Show()
    {
        std::cout<<"This is ART_TYPE_C"<<std::endl;
    }
    virtual void SetArgName( const std::string& str )
    {
        ArgName() += str+"C";
    }
};

void SetArgName( ARG_TYPE_BASE& a )
{
    a.SetArgName("ttt");
}

class HandlerBase
{
public:
    HandlerBase(){}
    virtual ~HandlerBase(){}
    virtual void Call() = 0;
};
typedef std::map< int, HandlerBase* > HandlerMap;
template < typename OBJ, typename ARG_TYPE>
class Handler : public HandlerBase
{
public:
    typedef void (OBJ::*FUNC)(ARG_TYPE& );
    typedef std::pair< OBJ*, FUNC > PairHandleType;
    Handler(): HandlerBase() {}
    Handler( OBJ* obj,FUNC fun  ) : HandlerBase()
    {
        m_Hanlde = std::make_pair( obj, fun );
    }
    virtual ~Handler(){}
    virtual void Call()
    {
        if ( m_Hanlde.first )
        {
            ARG_TYPE base;
            SetArgName(base);
            ( (m_Hanlde.first)->*m_Hanlde.second)(base);
        }
    }
private:
    PairHandleType m_Hanlde;
};
#define MK( k, v )  std::make_pair( k, v )
HandlerMap mapHandlers;

class Student
{
public:
    Student() : m_Name("abc")
    {
        InitHandler();
    }
    void InitHandler();

    void Call1( ART_TYPE_A& a )
    {
        a.Show();
        std::cout<<a.ArgName()<<std::endl;
    }
    void Call2( ART_TYPE_B& b )
    {
        b.Show();
        std::cout<<b.ArgName()<<std::endl;
    }
    void Call3( ART_TYPE_C& c )
    {
        c.Show();
        std::cout<<c.ArgName()<<std::endl;
    }
private:
    std::string m_Name;
};

int cmd_id = 1;

template< typename OBJ, typename ARG_TYPE >
void AddHandler( OBJ* obj, void (OBJ::*func)(ARG_TYPE&)  )
{
    HandlerBase* pBase = new Handler<OBJ, ARG_TYPE>( obj, func );
    mapHandlers.insert( MK( cmd_id++,  pBase) );
}
void Student::InitHandler()
{
    AddHandler( this, &Student::Call1 );
    AddHandler( this, &Student::Call2 );
    AddHandler( this, &Student::Call3 );
}


int main( int argc, char **argv )
{
    Student s1;

    for ( HandlerMap::iterator it = mapHandlers.begin(); it != mapHandlers.end();  )
    {
        it->second->Call();
        delete it->second;
        it++;
    }
    mapHandlers.clear();

    //smapHandlers.insert( std::make_pair( 1, new Handler<int>(1)));
    //mapHandlers.insert( std::make_pair( 2, new Handler< float >(2.0f) ));


    //NetMgr::GetInstance()->StartNetWork( 7777, "127.0.0.1" );

//    //ACE_Reactor_Impl* reactor =  new ACE_Dev_Poll_Reactor();
//    MyAcceptor acceptor( addr, ACE_Reactor::instance() );
//    ACE_Time_Value val( 0, 50000 );
//    while( !ACE_Reactor::instance()->reactor_event_loop_done() )
//    {
//        if ( -1 == ACE_Reactor::instance()->run_reactor_event_loop(val ) )
//        {
//            break;
//        }
//    }
//    int t = 166;
//    ACE_Message_Block* pBlock = new (::std::nothrow) ACE_Message_Block(30);
//    //pBlock->copy( (char*)&t, sizeof(t) );
//    pBlock->copy( "123456", 6);
//    pBlock->rd_ptr(2);
//    char tt[128] = "abcdef";
//    pBlock->base( tt, 4  );
//    std::cout<<pBlock->rd_ptr()<<std::endl;

//    pBlock->rd_ptr( 2 );
//    pBlock->crunch();
//    //ACE_OS::sprintf( pBlock->wr_ptr(), "%s", "hello" );
//    //pBlock->wr_ptr(2);
//    std::cout<<pBlock->rd_ptr()<<std::endl;
//    std::cout<<"pBlock->capacity()="<<pBlock->capacity()<<std::endl;
//    std::cout<<"pBlock->space()="<<pBlock->space()<<std::endl;
//   std::cout<<"pBlock->length()="<<pBlock->length()<<std::endl;
//    pBlock->release();
//    //delete pBlock;
//    std::cout<<std::endl;
    return 0;
}

