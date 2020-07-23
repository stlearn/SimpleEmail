#include"menu.h"
#include<iostream>
using namespace std;

//定义登录状态全局变量
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
	cout << "|1.发送邮件                        |" << endl;
	cout << "|2.接收邮件                        |" << endl;
	cout << "|0.退出                            |" << endl;
	cout << "------------------------------------" << endl;
}

//打印标题
void print_title()
{
	system("cls");
	string stars = "************************************\n";
	string title = "*        欢迎使用简易邮箱          *\n";
	string userstr;
	string smtpstr;
	string popstr;
	if (islogin) {
		userstr = "已登录，你好"+user+"\n";
	}
	else {
		userstr = "*             未登录               *\n";
	}
	if (smtp) {
		smtpstr = "smtp服务器地址："+SmtpAddress+"\n";
	}
	else {
		smtpstr = "";
	}
	if (pop3) {
		popstr = "pop3服务器地址：" + Pop3Address + "\n";
	}
	else {
		popstr = "";
	}

	string main_title = stars + title + stars + userstr + smtpstr + popstr + stars;

	cout << main_title << endl;
}