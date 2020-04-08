/*
 * =====================================================================================
 *
 *       Filename:  MySvcHandler.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2019年12月27日 16时09分44秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __MY_SVC_HANDLER_H
#define __MY_SVC_HANDLER_H

#include "ace/Thread.h"
#include "ace/Synch.h"
#include <ace/Svc_Handler.h>
#include <ace/SOCK_Stream.h>
#include "LockedQueue.h"
#include <google/protobuf/message.h>
#define override
#define DATA_SIZE 1024
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;


struct PacketHeader
{
    uint32  size;
    uint32  cmd;
};

struct Packet
{
    PacketHeader    msg_header;
    std::string     msg_body;
};


class MyAceSvcHandler : public ACE_Svc_Handler< ACE_SOCK_STREAM, ACE_NULL_SYNCH >
{
public:
    MyAceSvcHandler() ;
    virtual ~MyAceSvcHandler();
public:
    int Update();
    bool IsClosed()const;
    virtual int open( void* arg ) override;

    virtual int handle_input(ACE_HANDLE fd) override;

    virtual int handle_output( ACE_HANDLE fd ) override;

    virtual int handle_close(ACE_HANDLE h = ACE_INVALID_HANDLE, ACE_Reactor_Mask mask = ACE_Event_Handler::ALL_EVENTS_MASK ) override;

    int ProcessPkg(  );

    void CloseSock();

private:
    uint32                  m_dwSockId;
    ACE_Message_Block       m_MsgHeader;
    ACE_Message_Block       m_MsgBody;
    bool                    m_bOutActive;
    LockedQueue< ACE_Message_Block*, ACE_Thread_Mutex > m_SndBufQueue;
    LockedQueue< ACE_Message_Block*, ACE_Thread_Mutex > m_RcvBufQueue;
};

typedef ACE_Acceptor< MyAceSvcHandler, ACE_SOCK_ACCEPTOR > MyAcceptor;
typedef ACE_Connector< MyAceSvcHandler, ACE_SOCK_CONNECTOR > MyConnector;
#endif
