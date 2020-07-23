#include"Send.h"
#include<iostream>
#include"Menu/menu.h"
#pragma comment(lib, "ws2_32.lib")
#define MAX_STR 100

#ifdef __linux__
#define Sleep sleep
#define closesocket close

#define strcpy_s(buffer, size, arg) \
	do{ strcpy(buffer, arg); } while (0) 

#define sprintf_s(buffer, size, arg...) \
	do{ sprintf(buffer, ##arg); } while (0) 
#endif // __linux__

using namespace std;

//���õ�¼״̬ȫ�ֱ���
extern bool islogin;
extern string user;
extern string pass;
extern bool smtp;
extern string SmtpAddress;

//�����ʼ�����ں���
void send()
{
    char EmailTo[MAX_STR];  //�˴����ʹ������
    char EmailFrom[MAX_STR]; //��������
    char EmailKey[MAX_STR];       //�������������

    string subject; //�ʼ�����
    string content; //�ʼ�����

    print_title();
    //δ�����˻�������
    if (!islogin) {
        islogin = true;
        smtp = true;
        cout << "������smtp��������ַ:" << endl;
        cin >> SmtpAddress;
        cout << "���������������ַ:" << endl;
        cin >> user;
        cout << "��������������:" << endl;
        cin >> pass;
    }
    if(!smtp){
        smtp = true;
        cout << "������smtp��������ַ:" << endl;
        cin >> SmtpAddress;
    }
    print_title();

    memset(EmailFrom, 0, MAX_STR);
    memset(EmailKey, 0, MAX_STR);
    user.copy(EmailFrom, user.size(), 0);
    pass.copy(EmailKey, pass.size(), 0);

    cout << "�������ռ���:" << endl;
    cin >> EmailTo;
    cout << "�������ʼ�����:" << endl;
    cin >> subject;
    cout << "�������ʼ����ģ�" << endl;
    cin >> content;

    string EmailContents = "From:<" + string(EmailFrom) + ">\r\n"
        + string("To:<" + string(EmailTo) + ">\r\n")
        + "Subject:"+subject+"\r\n\r\n"
        + content +"\n";
    printf("send comment:\n%s", EmailContents.c_str());
    bool res = SendMail(EmailTo, EmailContents.c_str(), EmailFrom, EmailKey);
    system("pause");
    return;
}


struct Base64Date6
{
    unsigned int d4 : 6;
    unsigned int d3 : 6;
    unsigned int d2 : 6;
    unsigned int d1 : 6;
};


char ConvertToBase64(char uc)
{
    if (uc < 26)
    {
        return 'A' + uc;
    }
    if (uc < 52)
    {
        return 'a' + (uc - 26);
    }
    if (uc < 62)
    {
        return '0' + (uc - 52);
    }
    if (uc == 62)
    {
        return '+';
    }
    return '/';
}

// base64��ʵ��
void EncodeBase64(char* dbuf, char* buf128, int len)
{
    struct Base64Date6* ddd = NULL;
    int      i = 0;
    char     buf[256] = { 0 };
    char* tmp = NULL;
    char     cc = '\0';

    memset(buf, 0, 256);
    strcpy_s(buf, 256, buf128);

    for (i = 1; i <= len / 3; i++)
    {
        tmp = buf + (i - 1) * 3;
        cc = tmp[2];
        tmp[2] = tmp[0];
        tmp[0] = cc;
        ddd = (struct Base64Date6*)tmp;
        dbuf[(i - 1) * 4 + 0] = ConvertToBase64((unsigned int)ddd->d1);
        dbuf[(i - 1) * 4 + 1] = ConvertToBase64((unsigned int)ddd->d2);
        dbuf[(i - 1) * 4 + 2] = ConvertToBase64((unsigned int)ddd->d3);
        dbuf[(i - 1) * 4 + 3] = ConvertToBase64((unsigned int)ddd->d4);
    }
    if (len % 3 == 1)
    {
        tmp = buf + (i - 1) * 3;
        cc = tmp[2];
        tmp[2] = tmp[0];
        tmp[0] = cc;
        ddd = (struct Base64Date6*)tmp;
        dbuf[(i - 1) * 4 + 0] = ConvertToBase64((unsigned int)ddd->d1);
        dbuf[(i - 1) * 4 + 1] = ConvertToBase64((unsigned int)ddd->d2);
        dbuf[(i - 1) * 4 + 2] = '=';
        dbuf[(i - 1) * 4 + 3] = '=';
    }
    if (len % 3 == 2)
    {
        tmp = buf + (i - 1) * 3;
        cc = tmp[2];
        tmp[2] = tmp[0];
        tmp[0] = cc;
        ddd = (struct Base64Date6*)tmp;
        dbuf[(i - 1) * 4 + 0] = ConvertToBase64((unsigned int)ddd->d1);
        dbuf[(i - 1) * 4 + 1] = ConvertToBase64((unsigned int)ddd->d2);
        dbuf[(i - 1) * 4 + 2] = ConvertToBase64((unsigned int)ddd->d3);
        dbuf[(i - 1) * 4 + 3] = '=';
    }
    return;
}
// �����ʼ�
bool SendMail(char* emailTo, const char* body, char* emailFrom, char* emailKey)
{
    int   sockfd = { 0 };
    char  buf[1500] = { 0 };
    char  rbuf[1500] = { 0 };
    char  login[128] = { 0 };
    char  pass[128] = { 0 };

    struct sockaddr_in their_addr = { 0 };
#ifdef WIN32
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2, 2), &WSAData);
#endif // WIN32
    memset(&their_addr, 0, sizeof(their_addr));

    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(25);  // һ����25�˿ڲ���Ҫ��
    hostent* hptr = gethostbyname(SmtpAddress.c_str());     // �˿ںͷ�����
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
    // �����ʼ���������������Ӻ�û����Ӧ����2 �����������
    sockfd = OpenSocket((struct sockaddr*)&their_addr);
    memset(rbuf, 0, 1500);
    while (recv(sockfd, rbuf, 1500, 0) == 0)
    {
        cout << "reconnect..." << endl;
        return false;
        Sleep(2);
        sockfd = OpenSocket((struct sockaddr*)&their_addr);
        memset(rbuf, 0, 1500);
    }

    cout << rbuf << endl;

    // EHLO
    memset(buf, 0, 1500);
    sprintf_s(buf, 1500, "EHLO HYL-PC\r\n");
    send(sockfd, buf, strlen(buf), 0);
    memset(rbuf, 0, 1500);
    recv(sockfd, rbuf, 1500, 0);
    cout << "EHLO REceive: " << rbuf << endl;

    //��������: AUTH LOGIN �����½��
    memset(buf, 0, 1500);
    sprintf_s(buf, 1500, "AUTH LOGIN\r\n");
    send(sockfd, buf, strlen(buf), 0);
    memset(rbuf, 0, 1500);
    recv(sockfd, rbuf, 1500, 0);
    cout << "Auth Login Receive: " << rbuf << endl;

    // USER �����Լ��˻����û�����base64����
    memset(buf, 0, 1500);
    sprintf_s(buf, 1500, "%s", emailFrom);//��������˺�
    memset(login, 0, 128);
    EncodeBase64(login, buf, strlen(buf));
    sprintf_s(buf, 1500, "%s\r\n", login);
    send(sockfd, buf, strlen(buf), 0);
    cout << "Base64 UserName: " << buf << endl;
    memset(rbuf, 0, 1500);
    recv(sockfd, rbuf, 1500, 0);
    cout << "User Login Receive: " << rbuf << endl;

    // PASSWORD �����Լ��������base64����
    sprintf_s(buf, 1500, "%s", emailKey);//�����������
    memset(pass, 0, 128);
    EncodeBase64(pass, buf, strlen(buf));
    sprintf_s(buf, 1500, "%s\r\n", pass);
    send(sockfd, buf, strlen(buf), 0);
    cout << "Base64 Password: " << buf << endl;

    //��֤��Ȩȷ��
    memset(rbuf, 0, 1500);
    recv(sockfd, rbuf, 1500, 0);
    cout << "Send Password Receive: " << rbuf << endl;

    // MAIL FROM ��ʼ�����ʼ����ȷ����ʼ�˵��:MAIL FROM 
    memset(buf, 0, 1500);
    sprintf_s(buf, 1500, "MAIL FROM: <%s>\r\n", emailFrom); //�˴�Ҫ�ͷ��ʼ������䱣��һ��
    send(sockfd, buf, strlen(buf), 0);
    memset(rbuf, 0, 1500);
    recv(sockfd, rbuf, 1500, 0);
    cout << "set Mail From Receive: " << rbuf << endl;

    // RCPT TO ��һ���ռ���,����Ŀ������˵��:RCPT TO
    sprintf_s(buf, 1500, "RCPT TO:<%s>\r\n", emailTo);
    send(sockfd, buf, strlen(buf), 0);
    memset(rbuf, 0, 1500);
    recv(sockfd, rbuf, 1500, 0);
    cout << "Tell Sendto Receive: " << rbuf << endl;

    // DATA ׼����ʼ�����ʼ�����
    /*
      �������ݸ�ʽ��
       From: **_test@163.com
       To: **_a_test@163.com
       Subject: ����һ��
       MIME-Version: 1.0
    */
    sprintf_s(buf, 1500, "DATA\r\n");
    send(sockfd, buf, strlen(buf), 0);
    memset(rbuf, 0, 1500);
    recv(sockfd, rbuf, 1500, 0);
    cout << "Send Mail Prepare Receive: " << rbuf << endl;

    // �����ʼ����ݣ�\r\n.\r\n���ݽ������
    sprintf_s(buf, 1500, "%s\r\n.\r\n", body);
    send(sockfd, buf, strlen(buf), 0);
    memset(rbuf, 0, 1500);
    recv(sockfd, rbuf, 1500, 0);
    cout << "Send Mail Receive: " << rbuf << endl;

    // QUIT quit����ر��������������
    sprintf_s(buf, 1500, "QUIT\r\n");
    send(sockfd, buf, strlen(buf), 0);
    memset(rbuf, 0, 1500);
    recv(sockfd, rbuf, 1500, 0);
    cout << "Quit Receive: " << rbuf << endl;

    //������
    closesocket(sockfd);
#ifdef WIN32
    WSACleanup();
#endif
    return true;
}
// ��TCP Socket����
int OpenSocket(struct sockaddr* addr)
{
    int sockfd = 0;
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        cout << "Open sockfd(TCP) error!" << endl;
        //exit(-1);
    }
    if (connect(sockfd, addr, sizeof(struct sockaddr)) < 0)
    {
        cout << "Connect sockfd(TCP) error!" << endl;
        // exit(-1); 
    }
    return sockfd;
}
