/**
 * @file udpserver.c
 * @author holylong (mrhlingchen@163.com)
 * @brief udp server
 * @version 0.1
 * @date 2022-07-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define DEFAULT_PORT 9090

#ifdef _WIN32
// #include <windows.h>
#include <WinSock2.h>


void init_wsa(){
  WSADATA wsa;
  WORD wVersionRequested = MAKEWORD(2,2);
  int err = WSAStartup(wVersionRequested, &wsa);
  if(err != 0){
    printf("Init Socket Erorr");
  }
}

void release_wsa(){
  WSACleanup();
}
#else
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#endif



#ifdef _WIN32
void dg_cli(SOCKET sock, int revlen, SOCKADDR* cltaddr)
#else
void dg_cli(int sock, int revlen,const struct sockaddr* cltaddr)
#endif
{
  char revbuf[2048];
  int fromlen = sizeof(*cltaddr);
  while(1){
    memset(revbuf, '\0', 2048);
    printf("wait a msg...\n");
    revlen = recvfrom(sock, revbuf, 2048, 0, cltaddr, &fromlen);
    if(revlen >= 0){
      printf("revbuf:%s\n", revbuf);
#ifdef _WIN32
    }else Sleep(1);
#else
    }else usleep(1);
#endif

    // strcpy(revbuf, "12343434");
    revlen = sendto(sock, revbuf, strlen(revbuf)+1, 0, cltaddr, fromlen);
    if(revlen >= 0){
      printf("sendbuf:%s\n", revbuf);
#ifdef _WIN32
    }else Sleep(1);
#else
    }else usleep(1);
#endif
  }
}

int main(){
#ifdef _WIN32
  init_wsa();
#endif
  int ret = -1, revlen = 0;
  struct sockaddr_in seraddr, cltaddr;
#ifdef _WIN32
  SOCKET sock = INVALID_SOCKET;
#else
  int sock = -1;
#endif

#ifdef _WIN32
  sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if(sock == INVALID_SOCKET){
    printf("init socket error:%d\n", GetLastError());
    return -1;
  }
#else
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock == -1){
    printf("init socket error:%d\n", errno);
    return -1;
  }
#endif

  //port
  seraddr.sin_port = htons(DEFAULT_PORT);
  //ip
  seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  //or
  //seraddr.sin_addr.S_un.S_addr = INADDR_ANY;
  //type
  seraddr.sin_family = AF_INET;

#ifdef _WIN32
  ret = bind(sock, (SOCKADDR*)&seraddr, sizeof(SOCKADDR));
#else
  ret = bind(sock, (const struct sockaddr*)&seraddr, sizeof(struct sockaddr));
#endif
  if(ret != 0){
#ifdef WIN32
    printf("bind port:%d error:%d\n", DEFAULT_PORT, GetLastError());
#else
    printf("bind port:%d error:%d\n", DEFAULT_PORT, errno);
#endif
  }else{
    printf("bind port:%d ok\n", DEFAULT_PORT);
  }
  printf("wait clients");

#ifdef _WIN32 
  dg_cli(sock, revlen, (SOCKADDR*)&cltaddr);
#else
  dg_cli(sock, revlen, (struct sockaddr*)&cltaddr);
#endif

#ifdef _WIN32
  closesocket(sock);
  release_wsa();
#endif
  return 0;
}
