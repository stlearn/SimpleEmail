// Email.cpp: 定义应用程序的入口点。
//

#include "Email.h"
#include"Menu/menu.h"
#include"Receive.h"
#include"Send.h"
#include<sstream>

using namespace std;


bool isNum(string str); 

//主函数，控制主菜单的选择和调用进一步处理的模块
int main()
{
	//主菜单选项
	string option;
	int menu_option;
	print_title();
	print_menu();
	while (true) {
		cin >> option;
		if (isNum(option)) {
			menu_option = atoi(option.c_str());
		}
		else{
			print_title();
			print_menu();
			cout << "选项输入有误，请重新输入：" << endl;
			continue;
		};

		switch (menu_option) {
			case 1:send(); break;
			case 2:receive(); break;
			case 0:exit(0); break;
			default: {
				print_title();
				print_menu();
				cout << "选项输入有误，请重新输入：" << endl; 
				continue;
			}
		}
		print_title();
		print_menu();
	}
	

	return 0;
}

bool isNum(string str)
{
	stringstream sin(str);
	double d;
	char c;
	if (!(sin >> d))
	{
		return false;
	}
	if (sin >> c)
	{
		return false;
	}
	return true;
}