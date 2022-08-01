/**
 * @file udpclient.c
 * @author holylong (mrhlingchen@163.com)
 * @brief udpclient
 * @version 0.1
 * @date 2022-07-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdio.h>

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
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#endif

#ifdef _WIN32
void dg_cli(SOCKET sock, SOCKADDR* targetaddr, int sklen){
#else
void dg_cli(int sock, const struct sockaddr* targetaddr, int sklen){
#endif
  int ret = 0, sclen = sizeof(*targetaddr);
  const int on = 1;
  char revbuf[2048];
  setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (const char*)&on, sizeof(on));
  while(1){
    printf("please input string...\n");
    memset(revbuf, '\0', 2048);
    scanf("%s", revbuf);
    ret = sendto(sock, revbuf, strlen(revbuf), 0, targetaddr, sclen);
    if(ret >= 0){ 
      printf("broadcast send ok%s\n", revbuf);
    }else{
      printf("broadcast send failed\n");
      break;
    }

    ret = recvfrom(sock, revbuf, sizeof(revbuf), 0, targetaddr, &sclen);
    
    if(ret >= 0){ 
      printf("broadcast recv:%s\n", revbuf);
    }else{
      printf("broadcast recv failed\n");
      break;
    }
  }
}

void print_help(char* argv[]){
  printf("\n==>> please input arguments like:%s 192.168.1.255\n", argv[0]);
}

int main(int argc, char* argv[]){
  if(argc == 1){
    print_help(argv);
    return -1;
  } 
#ifdef _WIN32
  init_wsa();
#endif

  int ret = -1;
#ifdef _WIN32
  SOCKET sock;
#else
  int sock;
#endif
  struct sockaddr_in targetaddr;

  sock = socket(AF_INET, SOCK_DGRAM, 0);
#ifdef _WIN32
  if(sock == INVALID_SOCKET){
#else
  if(sock == -1){
#endif
    printf("init socked error\n");
    return -1;
  }

  targetaddr.sin_family = AF_INET;
  targetaddr.sin_port = htons(DEFAULT_PORT);
  //windows not enabled
  // targetaddr.sin_addr.s_addr = inet_addr("172.19.0.255");
  targetaddr.sin_addr.s_addr = inet_addr(argv[1]);
  //linux enabled
  // targetaddr.sin_addr.s_addr = inet_addr("255.255.255.255");
  //or
  // targetaddr.sin_addr.S_un.S_addr = inet_addr("255.255.255.255");
  // targetaddr.sin_addr.S_un.S_addr = htonl(INADDR_BROADCAST);
  // targetaddr.sin_addr.S_un.S_addr = inet_addr("172.19.0.255");

  //std::string input;
  int sclen = sizeof(targetaddr);
  
#ifdef _WIN32
  dg_cli(sock, (SOCKADDR*)&targetaddr, sclen);
#else
  dg_cli(sock, (struct sockaddr *)&targetaddr, sclen);
#endif

#ifdef _WIN32
  closesocket(sock);
  release_wsa();
#endif
  return 0;
}
