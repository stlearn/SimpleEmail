#include"menu.h"
#include<iostream>
using namespace std;

//�����¼״̬ȫ�ֱ���
bool islogin = false;
bool smtp = false;
bool pop3 = false;
string user;
string pass;
string SmtpAddress;
string Pop3Address;

void print_menu()
{
	cout << "----------------MENU----------------" << endl;
	cout << "|1.�����ʼ�                        |" << endl;
	cout << "|2.�����ʼ�                        |" << endl;
	cout << "|0.�˳�                            |" << endl;
	cout << "------------------------------------" << endl;
}

//��ӡ����
void print_title()
{
	system("cls");
	string stars = "************************************\n";
	string title = "*        ��ӭʹ�ü�������          *\n";
	string userstr;
	string smtpstr;
	string popstr;
	if (islogin) {
		userstr = "�ѵ�¼�����"+user+"\n";
	}
	else {
		userstr = "*             δ��¼               *\n";
	}
	if (smtp) {
		smtpstr = "smtp��������ַ��"+SmtpAddress+"\n";
	}
	else {
		smtpstr = "";
	}
	if (pop3) {
		popstr = "pop3��������ַ��" + Pop3Address + "\n";
	}
	else {
		popstr = "";
	}

	string main_title = stars + title + stars + userstr + smtpstr + popstr + stars;

	cout << main_title << endl;
}