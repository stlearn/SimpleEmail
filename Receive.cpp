#include"Receive.h"
#include"Menu/menu.h"
#include"Send.h"
#include<iostream>
#include<stdlib.h>
#include<time.h>


#define MAX_STR 100

using namespace std;

//引用登录状态全局变量
extern string user;
extern bool islogin;
extern bool pop3;
extern string pass;
extern string Pop3Address;
void ReceiveEmail(const char* emailFrom, const char* emailKey);
//接收邮件的入口函数
void receive() 
{
    char EmailAddress[MAX_STR]; //发送邮箱
    char EmailKey[MAX_STR];       //发送邮箱的密码

    string subject; //邮件主题
    string content; //邮件内容

    print_title();
    //未保存账户和密码
    if (!islogin) {
        islogin = true;
        pop3 = true;
        cout << "请输入pop3服务器地址:" << endl;
        cin >> Pop3Address;
        cout << "请输入您的邮箱地址:" << endl;
        cin >> user;
        cout << "请输入邮箱密码:" << endl;
        cin >> pass;
    }
    if (!pop3) {
        pop3 = true;
        cout << "请输入imap服务器地址:" << endl;
        cin >> Pop3Address;
    }
    print_title();

    memset(EmailAddress, 0, MAX_STR);
    memset(EmailKey, 0, MAX_STR);
    user.copy(EmailAddress, user.size(), 0);
    pass.copy(EmailKey, pass.size(), 0);

    ReceiveEmail(EmailAddress, EmailKey);

    return;
}

void ReceiveEmail(const char* emailFrom, const char* emailKey)
{
    int   sockfd = { 0 };
    char  buf[1500] = { 0 };
    char  rbuf[100000] = { 0 };
    char  login[128] = { 0 };
    char  pass[128] = { 0 };

    struct sockaddr_in their_addr = { 0 };
#ifdef WIN32
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2, 2), &WSAData);
#endif // WIN32
    memset(&their_addr, 0, sizeof(their_addr));

    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(143);  // 一般是25端口不需要改
    hostent* hptr = gethostbyname(Pop3Address.c_str());     // 端口和服务器
#ifdef WIN32
    memcpy(&their_addr.sin_addr.S_un.S_addr, hptr->h_addr_list[0], hptr->h_length);
    printf("IP of smpt.qq.com is : %d:%d:%d:%d\n",
        their_addr.sin_addr.S_un.S_un_b.s_b1,
        their_addr.sin_addr.S_un.S_un_b.s_b2,
        their_addr.sin_addr.S_un.S_un_b.s_b3,
        their_addr.sin_addr.S_un.S_un_b.s_b4);
#else
    memcpy(&their_addr.sin_addr.s_addr, hptr->h_addr_list[0], hptr->h_length);
    printf("IP of smpt.163.com is : %d:%d:%d:%d\n",
        their_addr.sin_addr.s_addr & 0XFF,
        their_addr.sin_addr.s_addr >> 8 & 0XFF,
        their_addr.sin_addr.s_addr >> 16 & 0XFF,
        their_addr.sin_addr.s_addr >> 24 & 0XFF);
#endif
    // 连接邮件服务器，如果连接后没有响应，则2 秒后重新连接
    sockfd = OpenSocket((struct sockaddr*)&their_addr);
    memset(rbuf, 0, 1500);
    while (recv(sockfd, rbuf, 1500, 0) == 0)
    {
        cout << "reconnect..." << endl;
        Sleep(2);
        sockfd = OpenSocket((struct sockaddr*)&their_addr);
        memset(rbuf, 0, 1500);
    }

    cout << rbuf << endl;


    //登录
    memset(buf, 0, 1500);
    sprintf_s(buf, 1500, "a001 LOGIN %s %s\r\n",emailFrom,emailKey);
    send(sockfd, buf, strlen(buf), 0);
    memset(rbuf, 0, 1500);
    recv(sockfd, rbuf, 1500, 0);
    cout << "接收: " << rbuf << endl;
    
    //登录
    memset(buf, 0, 1500);
    sprintf_s(buf, 1500, "a002 select inbox\r\n");
    send(sockfd, buf, strlen(buf), 0);
    memset(rbuf, 0, 1500);
    recv(sockfd, rbuf, 1500, 0);

    cout << "接收: " << rbuf << endl;

    //邮件总数
    int count;

    int front, last;
    string a = string(rbuf);
    front = a.find(" ", 0);
    last = a.find(" ", front + 1);
    string res = a.substr(front+1, last - front);

    count = atoi(res.c_str());

    print_title();
    cout << "近20封邮件:" << endl;

    //获取邮件时间
    for (int i = count; i > count-20; i--) {
        memset(buf, 0, 1500);
        sprintf_s(buf, 1500, "a011 FETCH %d BODY[HEADER.FIELDS (DATE FROM SUBJECT)]\r\n", i);
        send(sockfd, buf, strlen(buf), 0);
        memset(rbuf, 0, 1500);
        recv(sockfd, rbuf, 1500, 0);

        string date, from, subject;
        string a = string(rbuf);
        int b, e;
        b = a.find('<', 0);
        if (b != -1) {
            e = a.find('>', b);
            from = a.substr(b, e - b + 1);
        }
        else {
            from = "";
        }

        b = a.find("Subject", 0);
        if (b != -1) {
            e = a.find("\r\n", b);
            subject = a.substr(b, e - b);
        }
        else {
            subject = "";
        } 
        
        b = a.find("Date", 0);
        if (b != -1) {
            e = a.find("\r\n", b);
            date = a.substr(b, e - b);
        }
        else {
            date = "";
        }
        



        cout <<i<<"\t";
        cout << from << "\t";
        cout << date << "\t";
        cout << subject << endl;
    }
    int p;

    cout << "请输入要查看的邮件号码：" << endl;
    cin >> p;

    memset(buf, 0, 1500);
    sprintf_s(buf, 1500, "a012 FETCH %d BODY[TEXT]\r\n",p);
    send(sockfd, buf, strlen(buf), 0);
    memset(rbuf, 0, 100000);
    recv(sockfd, rbuf, 100000, 0);

    string text;
    string s = string(rbuf);
    int b, e;
    b = s.find("\r\n", 0);
    if (b != -1) {
        e = s.find("\r\n",b+2);
        text = s.substr(b + 2, e - b - 2);
    }
    else {
        text = "";
    }

    cout << "邮件内容：" <<text<< endl;
    system("pause");
}

