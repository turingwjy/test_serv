/*
 * =====================================================================================
 *
 *       Filename:  client.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2019年11月25日 20时58分23秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __CLINET_CPP
#define __CLINET_CPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
using namespace std;

typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;
struct PacketHeader
{
    uint32  cmd;
    uint32  size;
};

struct PacketBody
{
    std::string str;
};
#define MAXLINE 4096
#define LOG(STR) std::cout<<(STR)<<std::endl

//int main ( int argc, char **argv)
//{
//    char buf[64] = {0};
//    char str[] = "12345";
//    //memcpy( (char*)(senddata + wpos), (uint8*)header.cmd, sizeof( header.cmd ) );
//    uint8 cmd = 3;
//    memcpy( buf , (uint8*)&cmd, sizeof(cmd) );
//    return 0;
//}

int main( int argc, char** argv )
{
    int sockfd, n , rec_len;
    char recvdata[1024], senddata[1024];
    char buf[ MAXLINE ];

    struct sockaddr_in ser_addr;
    printf( "argc = %d\n", argc );
    if ( argc != 2 )
    {
        LOG("argc = ");
        return 0;
    }
    printf("argv[0] = %s\n", argv[0]);
    printf("argv[1] = %s\n", argv[1]);
    sockfd = socket( AF_INET, SOCK_STREAM, 0 );
    if ( sockfd < 0 )
    {
        LOG( "create socket error" );
        return 0;
    }
    memset( &ser_addr, 0, sizeof( ser_addr ) );

    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons( 8888 );
    if ( inet_pton( AF_INET, argv[1], &ser_addr.sin_addr ) <= 0 )
    {
        printf( "inet_pton error for %s\n", argv[1] );
        return 0;
    }

    if ( connect( sockfd, ( struct sockaddr* )&ser_addr, sizeof( ser_addr ) ) < 0 )
    {
        printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }
    //memset( senddata, 'a', sizeof(senddata) );
    LOG("send msg to server");

    memset( senddata, '0', sizeof(senddata) );
    memset( recvdata, '0', sizeof(recvdata) );


    while(1)
    {
        //std::string msgBody = "1234";
        std::string msgBody ;
        std::cin >> msgBody;

        if ( msgBody == "close" )
        {
            close(sockfd);
            return 0;
        }

        //write one packet to sendbuf
        PacketHeader header;

        header.cmd = 1;
        header.size = sizeof( header ) + strlen( msgBody.c_str() );
        uint32 rpos = 0, wpos = 0;

        memcpy( (char*)(senddata + wpos), (uint8*)&header.cmd, sizeof( header.cmd ) );
        wpos += sizeof( header.cmd );

        memcpy( (char*)(senddata + wpos), (uint8*)&header.size, sizeof( header.size ) );
        wpos += sizeof( header.size );

        memcpy( (char*)(senddata + wpos), msgBody.c_str(), msgBody.size() );
        wpos += msgBody.size();

        //------------------------------------------------------------------

        if ( send( sockfd, senddata, ( wpos - rpos ), MSG_NOSIGNAL  ) < 0 )
        {
            printf("send error: %s(errno: %d)\n",strerror(errno),errno);
            return 0;
        }
        std::cout<<"sent data: " << senddata<<std::endl;
        memset( senddata, '0', sizeof(senddata) );
        wpos = 0;
//        char a[] = "12345";
//        if ( send( sockfd, a, sizeof(a), MSG_NOSIGNAL  ) < 0 )
//        {
//            printf("send error: %s(errno: %d)\n",strerror(errno),errno);
//            return 0;
//        }

       // break;
    }

    close(sockfd);

    return 0;
}

#endif
