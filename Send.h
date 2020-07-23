#ifndef SEND_H_
#define SEND_H_
 
#ifdef WIN32
#include <windows.h>
#include <WinSock.h>
#else
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <stdio.h>
#include <iostream>
 
using namespace std;
 
// 协议中加密部分使用的是base64方法
char ConvertToBase64(char c6);
void EncodeBase64(char *dbuf, char *buf128, int len);
bool SendMail(char *emailTo, const char *body,char* emailFrom, char* emailKey);
int OpenSocket(struct sockaddr *addr);
void send();
#endif