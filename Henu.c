/*
	项目名称：河大餐饮管理系统
	组员：宋吉胜，魏亚强，史浩楠（组长）
*/

//当注册用户名为：root，密码为：root的账号时，系统默认为初始管理员
#define _CRT_SECURE_NO_WARNINGS     //这个宏定义最好要放到.c文件的第一行
#pragma warning(disable:4996)
//头文件
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <winsock.h>
#include <string.h>
#include<stdint.h>

//宏定义区
#define M sizeof(struct User)			//用户表的大小
#define N sizeof(struct stapleFood)		//主食 表大小
#define Y sizeof(struct Form)			//订单表大小
#define Z sizeof(struct Pay_info)			//用户表的大小
//结构体

struct User //用户表
{
	char login_account[20];	 //用户名
	char login_type[5];	//用户类型
	char login_pwd[20];//登陆密码
	int pick_number;//取单序号
	char if_qualified[5];//是否合格
	struct User* next;	 //指针域
};
typedef struct User user;
typedef user* pUser;

struct stapleFood //主食
{
	char name[20];
	float price;
	struct stapleFood* next;
};
typedef struct stapleFood staple_food;
typedef staple_food* pStapleFood;

struct Pay_info //流水
{
	int Pay_number;//流水号
	char name[20];//下单菜品
	float price;//价格
	char ordering_time[30];//下单时间
	char login_account[20];//下单账户
	struct Pay_info* next;
};
typedef struct Pay_info Pay_infoForm;
typedef Pay_infoForm* pPay;

struct Form //订单
{
	int Ordering_number;//订单号
	char name[20];//下单菜品
	float price;//价格
	char ordering_time[30];//下单时间
	char login_account[20];//下单账户
	char ordering_remarks[30];//备注
	char if_pay[4];//是否已付:yes/no
	char if_complete[4];//是否已完成:yes/no
	struct Form* next;
};
typedef struct Form orderForm;
typedef orderForm* pOrder;

void set_tm(pOrder po)
{
	strcpy(po->ordering_remarks, "");

	time_t timep;
	struct tm* p;

	time(&timep);
	p = localtime(&timep);
	strcpy(po->ordering_time, "");
	char temp[30];
	sprintf(temp, "%d", p->tm_year + 1900);
	strcat(po->ordering_time, temp);
	strcat(po->ordering_time, "年");

	sprintf(temp, "%d", 1 + p->tm_mon);
	strcat(po->ordering_time, temp);
	strcat(po->ordering_time, "月");

	sprintf(temp, "%d", p->tm_mday);
	strcat(po->ordering_time, temp);
	strcat(po->ordering_time, "日");

	sprintf(temp, "%d", p->tm_hour);
	strcat(po->ordering_time, temp);
	strcat(po->ordering_time, ":");

	sprintf(temp, "%d", p->tm_min);
	strcat(po->ordering_time, temp);
}
//自定义函数区
/*国内习惯显示日期时间*/
char* now(char* datetime)
{
	time_t timep;
	struct tm* p;

	time(&timep);
	p = localtime(&timep);
	sprintf(datetime, "%4d-%02d-%02d %02d:%02d:%02d", (1900 + p->tm_year), (1 + p->tm_mon), p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
	return datetime;
}
//用户客户端

void Default();						 //欢迎界面
void User_Login();					 //用户登录界面
void Logon();						 //注册界面
pUser readUserFile();				 //从文件中读取用户信息，返回一个表头地址
void save_User(pUser p1);			 //将用户信息保存到文件中
void User_Menu();					 //用户菜单
void User_Order();					 //点餐
void Order_StapleFood();			 //主食
void Order_Form(int ID, int number); //订单
void Updata_User(pUser head);		 //更新用户表

//管理员客户端
int file_size(char* filename);					//获取文件长度
void Administrator_Menu();						//管理员菜单
void Administrator_Login();						//管理员登录
void All_User();								//查看所有用户
void Setup_administrator();						//设置管理员
void Food();									//菜品信息
void Add_Food();								//添加菜单
void Remove_Food();								//删除菜单
int max_pick_number();							//已有的最大pick_number
void save_stapleFood(pStapleFood p);			//保存主食信息
void Check_income();							//查看流水
void Setup_chef();								//设置厨师
void check_OrderForm();							//清除已经加入流水的订单
pStapleFood readStapleFoodFile();				//从主食文件中读取用户信息，返回一个表头地址
void resave_stapleFood(); //保存主食信息
//后厨客户端
void Cook_Menu();//后厨菜单
void Cook_login();//后厨登录
void Cook_list();//订单
void Cook_serving();//上菜
void save_pOrder(pOrder p);
void save_pPay(pPay p);
void save_OrderForm(pOrder p);//保存订单
int resave_2_stapleFood(pStapleFood p2, char foodName[20]);//删除foodName的重写
void resave_OrderForm(pOrder p);//重写订单 参数为pOrder 头
pOrder readOrederFormFile();//读取订单信息
pPay readPay_infoForm();
void add_pay_info_form();//将订单中已经完成和支付的加入流水

//公共函数
void toxy(int x, int y); //将光标移动到x，y坐标处
void HideCursor(int x);	 //隐藏光标
char* HidePassword();	 //隐藏密码
void Exit();			 //退出系统
void Change();			 //切换账号
void Amend_passWord();	 //修改密码
void List_Food();		 //菜单列表

//全局变量区
char _userName[20];
char _passWord[20];					//记录当前用户
pOrder orderHead = NULL, p2 = NULL; //记录当前用户的订单
int num = 0;

//函数实现区

void toxy(int x, int y) //将光标移动到x，y坐标处
{
	COORD pos = { x, y };
	HANDLE Out = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(Out, pos);
}

void HideCursor(int x) //隐藏光标 ,当x为0时，隐藏，为1时，显示
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, x };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void Default() //欢迎界面
{
	char t; //用于选择
	do
	{
		HideCursor(0); //隐藏光标
		system("color 72");
		printf("             ---------------------------\n");
		printf("            |    欢迎光临河南大学餐厅    |\n");
		printf("            |       请选择执行操作       |\n");
		printf("             ---------------------------\n");
		printf("            |      1      顾客登录       |\n");
		printf("            |      2      厨师登录       |\n");
		printf("            |      3      商家登录       |\n");
		printf("            |      4      顾客注册       |\n");
		printf("             ---------------------------\n");
		printf("            输入序号：");
		while (1) //死循环为防止其他按键干扰
		{
			t = _getch();  //不回显函数
			if (t == '1') //如果按1，则进入登录界面
				User_Login();
			else if (t == '2') //如果按2，则进入注册界面
				Cook_login();
			else if (t == '3')
				Administrator_Login();
			else if (t == '4')
				Logon();
		}		 //如果既不是1也不是2和3，则循环输入
	} while (1); //永远为真
}

void User_Menu() //用户菜单
{

	char t; //用于选择菜单选项
	do
	{
		system("cls");
		HideCursor(0); //隐藏光标
		system("color 74");

		printf("             ---------------------------\n");
		printf("            |    欢迎光临河南大学餐厅    |\n");
		printf("            |    当前用户：%s           |\n", _userName);
		printf("             ---------------------------\n");
		printf("            |      1      立即点餐       |\n");
		printf("            |      2      修改密码       |\n");
		printf("            |      3      切换账号       |\n");
		printf("            |      4      退出系统       |\n");
		printf("             ---------------------------\n");
		printf("            输入序号：");

		t = _getch(); //不回显函数，输入一个值
		switch (t)
		{
		case '1':
			User_Order();
			break;
		case '2':
			Amend_passWord();
			break;
		case '3':
			Change();
			break;
		case '4':
			Exit();
			break;
		default:
			break;
		}
	} while (1); //永远 为真
}

char* HidePassword() //隐藏密码
{
	char password[18]; //密码
	char* p;		   //指向密码的指针
	int i = 0;		   //记录密码位数
	char t;			   //输入密码
	for (;;)		   //此处为输入密码不回显操作
	{
		t = _getch();   //输入k
		if (t == '\r') //如果输入k为回车，则跳出循环
		{
			break;
		}
		else if (t == '\b') //如果输入k为删除键
		{
			if (i > 0) //如若密码还没完全删除
			{
				printf("\b");
				printf(" ");
				printf("\b");
				i--;
			}
		}
		else //如果输入的k既不是删除键，也不是回车键
		{
			password[i] = t; //把k的值赋给_password[i];
			printf("*");	 //输出*号，保护用户隐私
			i++;			 //密码位数加1
		}
	}
	password[i] = '\0'; //在数组末端加一个结束符
	p = password;		//让p指向该数组
	return p;			//返回一个指向密码的指针
}

pUser readUserFile() //从文件中读取用户信息，返回一个表头地址
{
	FILE* fp;		   //文件指针
	int n = 0;		   //记录链表 的结点数
	pUser head = NULL; //定义头指针
	pUser p2, p, pre;
	p2 = p = pre = NULL;
	fp = fopen("User.txt", "a+"); //以只读的方式打开文件
	if (fp == NULL)
	{
		printf("cannot open file\n");
	}
	else
	{
		while (!feof(fp)) //判断文件位置标志是否移动到文件末尾
		{
			n++;
			p = (pUser)malloc(M); //向内存申请一段空间
			fread(p, M, 1, fp);	  //将fp所指向的文件中的内容赋给p
			if (n == 1)
			{
				head = p;
				p2 = p;
			}
			else //创建链表
			{
				pre = p2;
				p2->next = p;
				p2 = p;
			}
		}
		p2->next = NULL;
	}
	if (n <= 2)
		head->next = NULL;
	else
		pre->next = NULL;
	fclose(fp);	 //关闭文件
	return head; //返回头指针
}

void save_pPay(pPay p1) //将p1所指向的内容存储到文件中
{

	//选择最大的Pay_number
	pPay p = readPay_infoForm();
	int maxs = 0;
	while (p)
	{
		maxs = max(p->Pay_number, maxs);
		p = p->next;
	}
	maxs++;
	p1->Pay_number = maxs;


	FILE* fp;					  //文件指针
	fp = fopen("Pay_info.txt", "a+"); //以追加的方式打开文件
	if (fp == NULL)
	{
		printf("cannot open file\n");
	}
	if (fwrite(p1, Z, 1, fp) != 1) //将p1所指向的1个大小为N的数据结构写入fp所指向的文件中
	{
		printf("file write error\n");
	}
	fclose(fp); //关闭文件
}

void save_User(pUser p1) //将p1所指向的内容存储到文件中
{
	FILE* fp;					  //文件指针
	fp = fopen("User.txt", "a+"); //以追加的方式打开文件
	if (fp == NULL)
	{
		printf("cannot open file\n");
	}
	if (fwrite(p1, M, 1, fp) != 1) //将p1所指向的1个大小为N的数据结构写入fp所指向的文件中
	{
		printf("file write error\n");
	}
	fclose(fp); //关闭文件
}

void User_Login() //登录页面
{
	char* q, t, c;			  // q指针用于接收 *HidePassword()函数返回的指向密码的指针，t和c均用于选择
	pUser p = readUserFile(); //从文件中读取用户信息，返回一个表头地址
	do
	{
		HideCursor(1);		//显示管光标
		system("cls");		//清屏
		system("color 73"); //设置颜色
		printf("             ---------------------------\n");
		printf("            |    欢迎光临河南大学餐厅    |\n");
		printf("            |     请输入用户名密码       |\n");
		printf("             ---------------------------\n");
		printf("            |      用户名：              |\n");
		printf("            |      密  码：              |\n");
		printf("             ---------------------------\n");
		toxy(27, 4);
		scanf("%s", _userName);
		toxy(27, 5);
		q = HidePassword();										 //输入密码
		strcpy(_passWord, q);									 //将获得的密码复制给_passWord[]数组
		while (p != NULL && strcmp(p->login_account, _userName) != 0) //遍历链表，寻找当前输入的账户的账号
		{
			p = p->next;
		}
		if (p != NULL) //如果p不为空
		{
			if (strcmp(p->login_pwd, _passWord) == 0) //如果输入的密码正确
			{
				toxy(19, 8);
				printf("正在登陆....");
				Sleep(500); //暂停0.5秒
				system("cls");
				toxy(20, 4);
				printf("登陆成功！");
				break; //跳出循环
			}
			else //如果输入的密码错误
			{
				toxy(19, 8);
				printf("正在登陆....");
				Sleep(500);
				system("cls");
				toxy(30, 8);
				printf("------------------------------");
				toxy(30, 9);
				printf("|                            |");
				toxy(30, 10);
				printf("|密码错误!是否继续?        |");
				toxy(30, 11);
				printf("|                            |");
				toxy(30, 12);
				printf("|  1.是                2.否  |");
				toxy(30, 13);
				printf("|                            |");
				toxy(30, 14);
				printf("------------------------------");
				while (1) //无限循环为防止其他按键干扰
				{
					t = _getch(); //输入t
					if (t == '1')
					{
						system("cls"); //清屏
						break;
					}
					else if (t == '2') //如果输入t为n，进入开始界面
					{
						system("cls"); //清屏
						Default();
						break;
					}
				}
			}
		}
		else //如果p为空，即输入的账号不正确
		{
			toxy(19, 8);
			printf("正在登陆....");
			Sleep(500);
			system("cls");
			toxy(30, 8);
			printf("------------------------------");
			toxy(30, 9);
			printf("|                            |");
			toxy(30, 10);
			printf("|账号错误!是否继续?          |");
			toxy(30, 11);
			printf("|                            |");
			toxy(30, 12);
			printf("|  1.是                2.否  |");
			toxy(30, 13);
			printf("|                            |");
			toxy(30, 14);
			printf("------------------------------");
			while (1) //死循环防止其他按键干扰
			{
				c = _getch();
				if (c == '1')
				{
					system("cls");
					break;
				}
				else if (c == '2')
				{
					system("cls"); //清屏
					Default();
				}
			}
		}
	} while (1);
	Sleep(500);	 //暂停0.5秒
	User_Menu(); //账号密码均正确，进入用户主菜单
}

int max_pick_number()//已有的最大pick_number
{
	pUser sele = readUserFile();
	int ret = 0;
	while (sele != NULL)
	{
		ret = max(ret, sele->pick_number);
		sele = sele->next;
	}
	return ret;
}

void Logon() //注册界面
{
	pUser p1;
	pUser p = readUserFile();									 //从文件中读取用户信息，返回一个表头地址
	p1 = (pUser)malloc(M);										 //向内存申请一段空间
	char* q1, * q2;												 // q1，q2分别用于接收 *HidePassword()函数返回的值
	char userName[20], passWord[20], againPassword[20], c, k, t; // c，k，t均用于选择
	do
	{
		system("cls");
		system("color 73");
		HideCursor(1); //显示光标
		int flag = 0;  //标记信息库中是否存在要注册用户名
		printf("             ---------------------------\n");
		printf("            |    欢迎光临河南大学餐厅    |\n");
		printf("            |     请输入用户名密码       |\n");
		printf("             ---------------------------\n");
		printf("            |      用 户 名：            |\n");
		printf("            |      密    码：            |\n");
		printf("            |      确认密码：            |\n");
		printf("             ---------------------------\n");
		toxy(29, 4);
		scanf("%s", userName); //输入用户名
		toxy(29, 5);
		q1 = HidePassword(); //输入密码
		strcpy(passWord, q1);
		toxy(29, 6);
		q2 = HidePassword(); //输入确认密码
		strcpy(againPassword, q2);
		while (p != NULL) //遍历链表，寻找当前输入的账户的账号
		{
			if (strcmp(p->login_account, userName) == 0) //如果信息库中存在该用户名
			{
				system("cls");
				toxy(22, 4);
				printf("用户名已存在，请重新注册！");
				flag = 1; // flag为1时表示用户名已存在
				break;	  //跳出循环
			}
			p = p->next;
		}
		if (flag) //如果用户名已存在
		{
			HideCursor(0); //隐藏光标
			toxy(22, 6);
			printf("是否继续？"); //询问是否继续
			toxy(22, 8);
			printf("1.是\t2.否");
			while (1)
			{
				t = _getch();
				if (t == '1')
					break;
				else if (t == '2')
					break;
			}
			if (t == '1') //如果继续，则重新注册
				continue;
			else if (t == '2') //如果不继续，则回到首页
			{
				system("cls"); //清屏
				Default();	   //回到首页
			}
		}

		if (strcmp(passWord, againPassword) == 0) //如果输入的两次密码均正确
		{
			strcpy(p1->login_account, userName);
			strcpy(p1->login_pwd, passWord);
			strcpy(_userName, userName);
			strcpy(_passWord, passWord);
			strcpy(p1->login_type, "客户"); //p1->login_type = "客户";
			if (strcmp(userName, "root") == 0 && strcmp(passWord, "root") == 0)
				strcpy(p1->login_type, "商家");
			//else		
			strcpy(p1->if_qualified, "no");
			p1->pick_number = max_pick_number() + 1;
			toxy(32, 16);
			printf("正在注册....");
			Sleep(500);
			break; //跳出循环
		}
		else //如果输入的两次密码不同
		{
			toxy(32, 16);
			printf("正在注册....");
			Sleep(500);
			system("cls");
			toxy(30, 8);
			printf("------------------------------");
			toxy(30, 9);
			printf("|                            |");
			toxy(30, 10);
			printf("|两次密码不一致!是否继续注册?|");
			toxy(30, 11);
			printf("|                            |");
			toxy(30, 12);
			printf("|  1.是                2.否  |");
			toxy(30, 13);
			printf("|                            |");
			toxy(30, 14);
			printf("------------------------------");
			while (1)
			{
				c = _getch();
				if (c == '1')
				{
					system("cls");
					break;
				}
				else if (c == '2')
				{
					system("cls");
					Default();
					break;
				}
			}
		}
	} while (1);
	save_User(p1); //调用函数，将p1所指向的内容存储到文件中
	system("cls");
	toxy(32, 10);
	printf("注册成功！");
	Sleep(500); //暂停0.5秒
	toxy(32, 12);
	printf("正在自动为您登陆....");
	Sleep(500);
	User_Menu(); //注册成功后进入主菜单
}

void User_Order() //点餐
{
	char ch = '0';
	char t = '0';
	int ID = 0;
	int number = 0;
	do
	{

		system("cls");
		toxy(32, 6);
		printf("点餐");
		toxy(24, 8);
		printf("-----------------");
		toxy(28, 10);
		printf("1) 菜  单");
		toxy(28, 12);
		printf("2）订  单");
		toxy(28, 14);
		printf("3）返  回");
		t = _getch();
		switch (t)
		{
		case '1':
			Order_StapleFood();
			break;
		case '2':
			Order_Form(ID, number);
			break;
		case '3':
			User_Menu();
			break;
		default:
			break;
		}
		if (t == '3')
			break;
	} while (1);
}

int max_Ordering_number()//最大的订单号
{
	pOrder sele = readOrederFormFile();
	int ret = 0;
	while (sele != NULL)
	{
		ret = max(ret, sele->Ordering_number);
		sele = sele->next;
	}
	return ret;
}

void Order_StapleFood() //主食
{
	char t;
	do
	{
		system("cls");
		if (file_size("StapleFood.txt") == 0)
		{
			toxy(26, 8);
			printf("当前无菜品(按任意键返回)");
			_getch();
			User_Order();
			return;
		}
		pStapleFood p, head = readStapleFoodFile();
		pOrder p1;
		p = head;
		char foodName[20];
		int i = 8;
		toxy(10, 4);
		printf("主食菜单");
		toxy(6, 6);
		printf("菜名          价格");
		toxy(6, 7);
		printf("----------------------");
		while (p != NULL)
		{
			toxy(6, i);
			printf("%s", p->name);
			toxy(20, i);
			printf("%.2f", p->price);
			i++;
			p = p->next;
		}
		toxy(40, 8);
		printf("请输入您要点的菜名:");
		scanf("%s", foodName);
		p = head;
		while (p != NULL && strcmp(p->name, foodName) != 0)
		{
			p = p->next;
		}
		if (p)
		{
			toxy(40, 10);
			printf("请输入备注信息:");
			char Beizhu[40] = "";
			toxy(55, 10);
			scanf("%s", Beizhu);

			toxy(40, 12);
			printf("点餐成功！是否继续？1.是   2.否");

			p1 = (pOrder)malloc(Y);
			strcpy(p1->login_account, _userName);
			p1->Ordering_number = max_Ordering_number() + 1;
			set_tm(p1);
			strcpy(p1->if_pay, "no");
			strcpy(p1->if_complete, "no");
			strcpy(p1->name, foodName);
			strcpy(p1->ordering_remarks, Beizhu);
			p1->price = p->price;
			save_OrderForm(p1);
			while (1)
			{
				t = _getch();
				if (t == '1')
					break;
				else if (t == '2')
					break;
			}
			if (t == '2')
				break;

		}
		else
		{
			toxy(36, 10);
			printf("------------------------------");
			toxy(36, 11);
			printf("|                            |");
			toxy(36, 12);
			printf("|菜名错误!是否继续点餐?      |");
			toxy(36, 13);
			printf("|                            |");
			toxy(36, 14);
			printf("|  1.是                2.否  |");
			toxy(36, 15);
			printf("|                            |");
			toxy(36, 16);
			printf("------------------------------");
			while (1) //无限循环为防止其他按键干扰
			{
				t = _getch(); //输入t
				if (t == '1')
				{
					system("cls"); //清屏
					Order_StapleFood();
					break;
				}
				else if (t == '2') //如果输入t为n，进入开始界面
				{
					system("cls"); //清屏
					User_Order();
					break;
				}
			}


		}
	} while (1);
}

pPay readPay_infoForm()
{
	FILE* fp;				 //文件指针
	int n = 0;				 //记录链表 的结点数
	pPay head = NULL; //定义头指针
	pPay p2 = NULL, p, pre = NULL;
	fp = fopen("Pay_info.txt", "a+"); //以只读的方式打开文件
	if (fp == NULL)
	{
		printf("cannot open file\n");
	}
	else
	{
		while (!feof(fp)) //判断文件位置标志是否移动到文件末尾
		{
			n++;
			p = (pPay)malloc(Z); //向内存申请一段空间
			fread(p, Z, 1, fp);			//将fp所指向的文件中的内容赋给p
			if (n == 1)
			{
				head = p;
				p2 = p;
			}
			else //创建链表
			{
				pre = p2;
				p2->next = p;
				p2 = p;
			}
		}
		p2->next = NULL;
	}
	if (n <= 2)
		head->next = NULL;
	else
		pre->next = NULL;
	fclose(fp);	 //关闭文件
	return head; //返回头指针
}

pOrder readOrederFormFile()
{
	FILE* fp;				 //文件指针
	int n = 0;				 //记录链表 的结点数
	pOrder head = NULL; //定义头指针
	pOrder p2 = NULL, p, pre = NULL;
	fp = fopen("OrderForm.txt", "a+"); //以只读的方式打开文件
	if (fp == NULL)
	{
		printf("cannot open file\n");
	}
	else
	{
		while (!feof(fp)) //判断文件位置标志是否移动到文件末尾
		{
			n++;
			p = (pOrder)malloc(Y); //向内存申请一段空间
			fread(p, Y, 1, fp);			//将fp所指向的文件中的内容赋给p
			if (n == 1)
			{
				head = p;
				p2 = p;
			}
			else //创建链表
			{
				pre = p2;
				p2->next = p;
				p2 = p;
			}
		}
		p2->next = NULL;
	}
	if (n <= 2)
		head->next = NULL;
	else
		pre->next = NULL;
	fclose(fp);	 //关闭文件
	return head; //返回头指针
}

void Order_Form(int ID, int number) //订单
{
	do
	{
		system("cls");
		int i = 10;
		char t = "";
		float totalPrice = 0;
		pOrder p = readOrederFormFile();

		pUser q, head;
		//q = head;
		if (p == NULL)
		{
			toxy(26, 8);
			printf("您还没有点任何东西，赶紧去点吧！(按任意键返回)");
			_getch();
			break;
		}
		else
		{
			toxy(26, 6);
			printf("我的订单");
			toxy(22, 8);
			printf("菜品名称        价格           备注");//改成:订单号 菜品名 菜品价格 下单时间 下单账号 备注信息 是否已付 是否完成
			toxy(20, 9);
			printf("***************************************");
			while (p != NULL)
			{
				if (strcmp(p->login_account, _userName) == 0 && strcmp(p->if_pay, "no") == 0)//是当前账户
				{
					toxy(22, i);
					printf("%s", p->name);
					toxy(38, i);
					printf("%.2f", p->price);
					toxy(53, i);
					printf("%s", p->ordering_remarks);
					i++;
					if (strcmp(p->if_pay, "yes") != 0)//未付
					{
						totalPrice += p->price;
					}

				}
				p = p->next;
			}
			head = readUserFile();
			q = head;
			toxy(20, i);
			printf("***************************************");
			while (q != NULL && strcmp(q->login_account, _userName) != 0)
			{
				q = q->next;
			}
			if (q)
			{
				toxy(24, ++i);
				printf("总价：  %.2f", totalPrice);
			}
			toxy(22, ++i);
			printf("1.确认支付   2.取消订单   3.返回");

			while (1)
			{
				t = _getch();
				if (t == '1' || t == '2' || t == '3')
					break;
			}
			if (t == '1')
			{

				pOrder p2 = readOrederFormFile();
				pOrder head_p2 = p2;
				while (p2)
				{
					if (strcmp(p2->login_account, _userName) == 0 && strcmp(p2->if_pay, "no") == 0)//是当前账户
					{
						strcpy(p2->if_pay, "yes");
					}

					p2 = p2->next;
				}
				resave_OrderForm(head_p2);
				add_pay_info_form();
				check_OrderForm();
				toxy(24, ++i);
				printf("正在支付....");
				Updata_User(head);
				Sleep(1500);
				system("cls");
				toxy(24, 10);
				printf("支付成功！按任意键返回");
				orderHead = NULL;
				_getch();
				break;
			}
			else if (t == '2')
			{
				pOrder p2 = readOrederFormFile();
				while (strcmp(p2->login_account, _userName) == 0 && strcmp(p2->if_pay, "no") == 0)
				{
					p2 = p2->next;
				}
				pOrder head_p2 = p2;
				pOrder p2_before = p2;
				p2 = p2->next;
				while (p2)
				{
					if (strcmp(p2->login_account, _userName) == 0 && strcmp(p2->if_pay, "no") == 0)//是当前账户
					{
						p2_before->next = p2->next;
						p2 = p2->next;
						continue;
					}
					p2 = p2->next;
					p2_before = p2_before->next;
				}
				resave_OrderForm(head_p2);
				break;
			}
			else if (t == '3')
				break;
		}
	} while (1);
}

void Updata_User(pUser head) //更新文件
{
	FILE* fp;
	pUser p = head->next;		 //使p指向第二个结点
	fp = fopen("User.txt", "w"); //先清空原文件内容再将修改后的信息写入文件
	if (fp == NULL)
	{
		printf("cannot open file\n");
	}
	if (fwrite(head, M, 1, fp) != 1) //将head所指向的数据写入fp所指向的文件
	{
		printf("file write error\n");
	}
	fclose(fp);					 //关闭文件
	fp = fopen("User.txt", "a"); // 打开User.txt文件，方式为追加
	while (p != NULL)			 // p不为空就执行循环体
	{
		if (fwrite(p, M, 1, fp) != 1) //将p所指向的1个大小为N的数据结构写入fp所指向的文件
		{
			printf("file write error\n");
		}
		p = p->next; //使p指向下一个结点
	}
	fclose(fp); //关闭文件
}

void Amend_passWord() //修改密码
{
	system("cls"); //清屏
	HideCursor(0); //隐藏光标，看着更舒服
	pUser p, head;
	head = readUserFile();
	do
	{
		p = head;
		char password[20], newpassword[20], t, k, * q1, * q2;
		int i = 0;
		toxy(34, 5); //将光标移动到坐标为（50，5）的位置
		printf("河大餐饮点餐系统");
		toxy(34, 8);
		printf("修改密码");
		toxy(34, 10);
		printf("请输入旧密码：");
		q1 = HidePassword();
		strcpy(password, q1);
		toxy(34, 12);
		printf("请输入新密码：");
		q2 = HidePassword();
		strcpy(newpassword, q2);
		while (p != NULL && strcmp(p->login_account, _userName) != 0) //遍历链表，寻找当前登陆账户的账号
		{
			p = p->next;
		}
		if (p != NULL) //如果p不为空
		{
			if (strcmp(p->login_pwd, password) == 0) //如果旧密码输入正确
			{
				strcpy(p->login_pwd, newpassword); //将旧密码改为新密码
				break;							  //跳出循环
			}
			else //如果旧密码输入错误
			{
				system("cls");
				toxy(30, 8);
				printf("--------------------------");
				toxy(30, 9);
				printf("|                        |");
				toxy(30, 10);
				printf("|  密码错误！是否继续？  |");
				toxy(30, 11);
				printf("|                        |");
				toxy(30, 12);
				printf("|  1.是           2.否   |");
				toxy(30, 13);
				printf("|                        |");
				toxy(30, 14);
				printf("--------------------------");
				while (1)
				{
					t = _getch(); //输入t
					if (t == '1')
					{
						system("cls");
						break;
					}
					else if (t == '2')
					{
						if (strcmp(p->login_type, "客户") == 0)
						{
							User_Menu();
						}
						else if (strcmp(p->login_type, "厨师") == 0)
						{
							Cook_Menu();
						}
						else if (strcmp(p->login_type, "商家") == 0)
						{
							Administrator_Menu();
						}

					}
				}
			}
		}
	} while (1);	   //永远为真
	Updata_User(head); //修改密码
	toxy(32, 14);
	printf("修改成功！正在跳转...");
	Sleep(500); //暂停0.5秒
}


void Change() //切换账号
{
	system("cls");
	Default();
}

void Exit() //退出点餐系统
{
	char t;
	toxy(30, 10);
	printf("-----------------------");
	toxy(30, 11);
	printf("|   您确定要退出吗？  |");
	toxy(30, 13);
	printf("| 1.确定     2.取消   |");
	toxy(30, 14);
	printf("-----------------------");
	while (1)
	{
		t = _getch(); //输入t
		switch (t)
		{
		case '1':
			system("cls");
			toxy(32, 10);
			printf("正在安全退出....");
			Sleep(1000); //暂停1秒
			system("cls");
			toxy(32, 10);
			printf("您已安全退出！");
			toxy(32, 12);
			printf("河大餐饮欢迎下次光临");
			exit(0);
			break; //终止程序
		case '2':
			break; //调用函数，进入菜单
		default:
			break;
		}
		if (t == '2')
			break;
	}
}

void Administrator_Menu() //管理员菜单
{
	char t; //用于选择菜单选项
	do
	{
		system("cls");
		HideCursor(0); //隐藏光标
		system("color 74");
		printf("             ---------------------------\n");
		printf("            |    欢迎光临河南大学餐厅    |\n");
		printf("            |    当前商家账号：%s      |\n", _userName);
		printf("             ---------------------------\n");
		printf("            |      1      菜品信息       |\n");
		printf("            |      2      查看流水       |\n");
		printf("            |      3      设置管理员     |\n");
		printf("            |      4      设置厨师       |\n");
		printf("            |      5      修改密码       |\n");
		printf("            |      6      切换账号       |\n");
		printf("            |      7      退出系统       |\n");
		printf("             ---------------------------\n");
		printf("            输入序号：");
		t = _getch(); //不回显函数，输入一个值
		switch (t)
		{
		case '1':
			Food();
			break;
		case '2':
			Check_income();
			break;
		case '3':
			Setup_administrator();
			break;
		case '4':
			Setup_chef();
			break;
		case '5':
			Amend_passWord();
			break;
		case '6':
			Change();
			break;
		case '7':
			Exit();
			break;
		default:
			break;
		}
	} while (1); //永远 为真
}

void Check_income()
{
	pPay p = readPay_infoForm();
	pPay   head;
	if (file_size("pay_info.txt") == 0)
	{
		system("cls");
		toxy(26, 8);
		printf("当前无流水(按任意键返回)");
		_getch();
		Administrator_Menu();
	}
	system("cls");

	int i = 10;//行定位
	char t;
	float totalPrice = 0;
	toxy(30, 4);
	printf("-----------");
	toxy(30, 5);
	printf("| 流水列表 |");
	toxy(30, 6);
	printf("-----------");
	toxy(5, 8);
	printf("流水号    菜品名    菜品价格    下单时间             下单账号");
	toxy(5, 9);
	printf("*************************************************************");
	while (p != NULL)
	{
		toxy(7, i);
		printf("%d", p->Pay_number);
		toxy(15, i);
		printf("%s", p->name);
		toxy(26, i);
		printf("%.2f", p->price);
		//下单时间
		toxy(37, i);
		printf(p->ordering_time);
		//下单账号
		toxy(60, i);
		printf("%s\n", p->login_account);
		i++;
		p = p->next;
	}
	toxy(5, i);
	printf("*************************************************************");
	toxy(26, i + 4);
	printf("(按任意键返回)");
	_getch();
	Administrator_Menu();

}
void Administrator_Login() //管理员登录
{
	char* q, t, c;			  // q指针用于接收 *HidePassword()函数返回的指向密码的指针，t和c均用于选择
	pUser p = readUserFile(); //从文件中读取用户信息，返回一个表头地址
	do
	{
		HideCursor(1);		//显示管光标
		system("cls");		//清屏
		system("color 73"); //设置颜色
		printf("             ---------------------------\n");
		printf("            |    欢迎光临河南大学餐厅    |\n");
		printf("            |   请输入管理员用户名密码   |\n");
		printf("             ---------------------------\n");
		printf("            |      用户名：              |\n");
		printf("            |      密  码：              |\n");
		printf("             ---------------------------\n");
		toxy(27, 4);
		scanf("%s", _userName);
		toxy(27, 5);
		q = HidePassword();										 //输入密码
		strcpy(_passWord, q);									 //将获得的密码复制给_passWord[]数组
		while (p != NULL && strcmp(p->login_account, _userName) != 0) //遍历链表，寻找当前输入的账户的账号
		{
			p = p->next;
		}
		if (p != NULL) //如果p不为空
		{
			int s1 = strcmp(p->login_pwd, _passWord);
			int s2 = strcmp(p->login_type, "商家");
			if (s1 == 0 && s2 == 0) //如果输入的密码正确
			{
				toxy(32, 16);
				printf("正在登陆....");
				Sleep(500); //暂停0.5秒
				system("cls");
				toxy(32, 10);
				printf("登陆成功！");
				break; //跳出循环

			}
			else //如果输入的密码错误
			{
				toxy(32, 16);
				printf("正在登陆....");
				Sleep(500);
				system("cls");
				toxy(30, 8);
				printf("-------------------------");
				toxy(30, 9);
				printf("|                       |");
				toxy(30, 10);
				printf("| 密码错误！是否继续？  |");
				toxy(30, 11);
				printf("|                       |");
				toxy(30, 12);
				printf("| 1.是             2.否 |");
				toxy(30, 13);
				printf("|                       |");
				toxy(30, 14);
				printf("-------------------------");
				while (1) //无限循环为防止其他按键干扰
				{
					t = _getch(); //输入t
					if (t == '1')
					{
						system("cls"); //清屏
						break;
					}
					else if (t == '2') //如果输入t为n，进入开始界面
					{
						system("cls"); //清屏
						Default();
						break;
					}
				}
			}
		}
		else //如果p为空，即输入的账号不正确
		{
			toxy(32, 16);
			printf("正在登陆....");
			Sleep(500);
			system("cls");
			toxy(30, 8);
			printf("-------------------------");
			toxy(30, 9);
			printf("|                       |");
			toxy(30, 10);
			printf("|  账号错误！是否继续？ |");
			toxy(30, 11);
			printf("|                       |");
			toxy(30, 12);
			printf("|  1.是            2.否 |");
			toxy(30, 13);
			printf("|                       |");
			toxy(30, 14);
			printf("-------------------------");
			while (1) //死循环防止其他按键干扰
			{
				c = _getch();
				if (c == '1')
				{
					system("cls");
					break;
				}
				else if (c == '2')
				{
					system("cls"); //清屏
					Default();
				}
			}
		}
	} while (1);
	Sleep(500);			  //暂停0.5秒
	Administrator_Menu(); //账号密码均正确，进入管理员主菜单
}

void Food() //菜品信息
{
	HideCursor(0);
	system("color 74");
	char t;
	do
	{
		system("cls");
		toxy(32, 6);
		printf("菜品信息");
		toxy(28, 7);
		printf("-----------------");
		toxy(30, 9);
		printf(" 1) 添加菜品");
		toxy(30, 11);
		printf(" 2) 删除菜品");
		toxy(30, 13);
		printf(" 3) 菜单列表");
		toxy(30, 15);
		printf(" 4) 返回上一级");
		t = _getch();
		switch (t)
		{
		case '1':
			Add_Food();
			break;
		case '2':
			Remove_Food();
			break;
		case '3':
			List_Food();
			break;
		case '4':
			Administrator_Menu();
			break;
		default:
			break;
		}
		if (t == '3')
			break;
	} while (1);
}


void resave_stapleFood(pStapleFood p2) //保存主食信息
{
	FILE* fp;							//文件指针
	fp = fopen("StapleFood.txt", "w"); //以重写的方式打开文件
	if (fp == NULL)
	{
		printf("cannot open file\n");
	}
	pStapleFood p = p2;
	while (p)
	{
		if (fwrite(p, N, 1, fp) != 1) //将p1所指向的1个大小为N的数据结构写入fp所指向的文件中
		{
			printf("file write error\n");
		}
		p = p->next;
	}
	fclose(fp); //关闭文件
}

void resave_OrderForm(pOrder p)
{
	FILE* fp;							//文件指针
	fp = fopen("OrderForm.txt", "w"); //以重写的方式打开文件
	if (fp == NULL)
	{
		printf("cannot open file\n");
		return;
	}
	pOrder p2 = p;
	while (p2)
	{
		if (!(p2->ordering_remarks))
		{
			strcpy(p2->ordering_remarks, "");
		}
		if (fwrite(p2, Y, 1, fp) != 1) //将p1所指向的1个大小为N的数据结构写入fp所指向的文件中
		{
			printf("file write error\n");
		}
		p2 = p2->next;
	}
	fclose(fp); //关闭文件
}

void save_OrderForm(pOrder p)
{
	FILE* fp;							//文件指针
	fp = fopen("OrderForm.txt", "a+"); //以追加的方式打开文件
	if (fp == NULL)
	{
		printf("cannot open file\n");
	}
	if (!(p->ordering_remarks))
	{
		strcpy(p->ordering_remarks, "");
	}
	if (fwrite(p, Y, 1, fp) != 1) //将p1所指向的1个大小为N的数据结构写入fp所指向的文件中
	{
		printf("file write error\n");
	}
	fclose(fp); //关闭文件
}
void save_stapleFood(pStapleFood p) //保存主食信息
{
	FILE* fp;							//文件指针
	fp = fopen("StapleFood.txt", "a+"); //以追加的方式打开文件
	if (fp == NULL)
	{
		printf("cannot open file\n");
	}
	if (fwrite(p, N, 1, fp) != 1) //将p1所指向的1个大小为N的数据结构写入fp所指向的文件中
	{
		printf("file write error\n");
	}
	fclose(fp); //关闭文件
}
pStapleFood readStapleFoodFile() //从主食文件中读取用户信息，返回一个表头地址
{
	FILE* fp;				 //文件指针
	int n = 0;				 //记录链表 的结点数
	pStapleFood head = NULL; //定义头指针
	pStapleFood p2 = NULL, p, pre = NULL;
	fp = fopen("StapleFood.txt", "a+"); //以只读的方式打开文件
	if (fp == NULL)
	{
		printf("cannot open file\n");
	}
	else
	{
		while (!feof(fp)) //判断文件位置标志是否移动到文件末尾
		{
			n++;
			p = (pStapleFood)malloc(N); //向内存申请一段空间
			fread(p, N, 1, fp);			//将fp所指向的文件中的内容赋给p
			if (n == 1)
			{
				head = p;
				p2 = p;
			}
			else //创建链表
			{
				pre = p2;
				p2->next = p;
				p2 = p;
			}
		}
		p2->next = NULL;
	}
	if (n <= 2)
		head->next = NULL;
	else
		pre->next = NULL;
	fclose(fp);	 //关闭文件
	return head; //返回头指针
}


void Add_Food() //添加菜单
{
	HideCursor(1);		//显示光标
	pStapleFood p1;		//主食
	char name[20];
	float price = 0;
	int type = 1;
	char t;
	do
	{
		system("cls");
		p1 = (pStapleFood)malloc(N);
		toxy(30, 6);
		printf("请输入菜名:");
		toxy(30, 8);
		printf("请输入价格:");
		toxy(42, 6);
		scanf("%s", name);
		toxy(42, 8);
		scanf("%f", &price);
		//add
		strcpy(p1->name, name);
		p1->price = price;
		save_stapleFood(p1);
		toxy(28, 14);
		printf("添加成功！是否继续？(1.是  2.否)");
		while (1)
		{
			t = _getch();
			if (t == '1')
				break;
			else if (t == '2')
				break;
		}
		if (t == '2')
			break;
	} while (1);
}

int resave_2_stapleFood(pStapleFood p2, char foodName[20]) //保存主食信息 如果名字为foodName 则不存（删除）找到则返回1
{
	int if_find = 0;
	FILE* fp;							//文件指针
	fp = fopen("StapleFood.txt", "w"); //以重写的方式打开文件
	if (fp == NULL)
	{
		printf("cannot open file\n");
	}
	pStapleFood p = p2;
	while (p)
	{
		if (strcmp(foodName, p->name) == 0)
		{
			if_find = 1;
		}
		else
		{
			if (fwrite(p, N, 1, fp) != 1) //将p1所指向的1个大小为N的数据结构写入fp所指向的文件中
			{
				printf("file write error\n");
			}
		}
		p = p->next;
	}
	fclose(fp); //关闭文件
	return if_find;
}

void Remove_Food()//删除菜品
{
	char t;
	do {
		system("cls");
		if (file_size("StapleFood.txt") == 0)
		{
			toxy(26, 8);
			printf("当前无菜品(按任意键返回)");
			_getch();
			Food();
			return;
		}
		int i = 8;
		toxy(30, 4);
		printf("菜单列表(按任意键继续)");
		pStapleFood p1;				  //主食
		p1 = readStapleFoodFile();	  //从主食文件中读取用户信息，返回一个表头地址
		toxy(26, 6);
		printf("菜名\t\t\t价格");
		toxy(24, 7);
		printf("-------------------------------");
		while (p1 != NULL)
		{
			toxy(26, i);
			printf("%-22s%.2f", p1->name, p1->price);
			i++;
			p1 = p1->next;
		}
		i++;
		toxy(30, i);
		printf("输入删除菜品:");
		toxy(43, i);
		char foodName[20] = "";
		scanf("%s", foodName);
		p1 = readStapleFoodFile();
		int if_find = 0;
		if_find = resave_2_stapleFood(p1, foodName);

		//resave_stapleFood(p_head);
		if (if_find == 0)
		{
			toxy(36, i + 2);
			printf("------------------------------");
			toxy(36, i + 3);
			printf("|                            |");
			toxy(36, i + 4);
			printf("|菜名错误!是否继续删除?      |");
			toxy(36, i + 5);
			printf("|                            |");
			toxy(36, i + 6);
			printf("|  1.是                2.否  |");
			toxy(36, i + 7);
			printf("|                            |");
			toxy(36, i + 8);
			printf("------------------------------");
			while (1) //无限循环为防止其他按键干扰
			{
				t = _getch(); //输入t
				if (t == '1')
				{
					system("cls"); //清屏
					Remove_Food();
					break;
				}
				else if (t == '2') //如果输入t为n，进入开始界面
				{
					system("cls"); //清屏
					Food();
					break;
				}
			}
		}
		else
		{
			toxy(30, i + 2);
			printf("删除成功！是否继续？1.是   2.否");

			//save_OrderForm(p1);
			while (1)
			{
				t = _getch();
				if (t == '1')
					break;
				else if (t == '2')
				{
					Food();
					break;
				}

			}
			if (t == '2')
				break;
		}
	} while (1);
}

void List_Food() //菜单列表
{
	system("cls");
	if (file_size("StapleFood.txt") == 0)
	{
		toxy(26, 8);
		printf("当前无菜品(按任意键返回)");
		_getch();
		Food();
		return;
	}
	int i = 8;
	toxy(30, 4);
	printf("菜单列表(按任意键继续)");
	pStapleFood p1;				  //主食
	p1 = readStapleFoodFile();	  //从主食文件中读取用户信息，返回一个表头地址
	toxy(26, 6);
	printf("菜名\t\t\t价格");
	toxy(24, 7);
	printf("-------------------------------");
	while (p1 != NULL)
	{
		toxy(26, i);
		printf("%-22s%.2f", p1->name, p1->price);
		i++;
		p1 = p1->next;
	}
	_getch();
}


void Setup_administrator() //设置管理员
{

	system("color 80");
	char t;
	char username[20];
	pUser p, head;
	head = readUserFile();
	do
	{
		system("cls");
		p = head;
		toxy(26, 8);
		printf("请输入您要设置为管理员的用户名：");
		scanf("%s", username);
		while (p != NULL && strcmp(p->login_account, username) != 0)
		{
			p = p->next;
		}
		if (p != NULL)
		{
			strcpy(p->login_type, "商家");
			Updata_User(head);
			toxy(32, 10);
			printf("设置成功!请按任意键返回");
			_getch();
			system("cls");
			Default();

			break;
		}
		else
		{
			toxy(24, 10);
			printf("您所输入的用户名不存在！是否继续？(1.是   2.否)");
			while (1)
			{
				t = _getch();
				if (t == '1')
					break;
				else if (t == '2')
					break;
			}
			if (t == '2')
				break;
		}
	} while (1);
}
void Setup_chef()
{
	system("color 80");
	char t;
	char username[20];
	pUser p, head;
	head = readUserFile();
	do
	{
		system("cls");
		p = head;
		toxy(26, 8);
		printf("请输入您要设置为厨师的用户名：");
		scanf("%s", username);
		while (p != NULL && strcmp(p->login_account, username) != 0)
		{
			p = p->next;
		}
		if (p != NULL)
		{
			strcpy(p->login_type, "厨师");
			Updata_User(head);
			toxy(32, 10);
			printf("设置成功!请按任意键返回");
			_getch();
			system("cls");
			Default();

			break;
		}
		else
		{
			toxy(24, 10);
			printf("您所输入的用户名不存在！是否继续？(1.是   2.否)");
			while (1)
			{
				t = _getch();
				if (t == '1')
					break;
				else if (t == '2')
					break;
			}
			if (t == '2')
				break;
		}
	} while (1);
}
void Cook_Menu()
{
	char t;//用于选择菜单选择
	while (1)
	{
		system("cls");
		HideCursor(0);//隐藏光标
		system("color 74");
		printf("             ---------------------------\n");
		printf("            |    欢迎光临河南大学餐厅    |\n");
		printf("            |      当前厨师：%s        |\n", _userName);
		printf("             ---------------------------\n");
		printf("            |      1      订单信息       |\n");
		printf("            |      2      上   菜        |\n");
		printf("            |      3      修改密码       |\n");
		printf("            |      4      切换账号       |\n");
		printf("            |      5      退出系统       |\n");
		printf("             ---------------------------\n");
		printf("            输入序号：                  \n ");
		t = _getch();
		switch (t)
		{
		case'1':
			Cook_list(); break;
		case'2':
			Cook_serving(); break;
		case'3':
			Amend_passWord(); break;
		case'4':
			Change(); break;
		case'5':
			Exit();
			break;
		default:
			break;
		}//永远为真
	}
}
void Cook_login()
{
	char* q, t, c;			  // q指针用于接收 *HidePassword()函数返回的指向密码的指针，t和c均用于选择
	pUser p = readUserFile(); //从文件中读取用户信息，返回一个表头地址
	do
	{
		HideCursor(1);		//显示管光标
		system("cls");		//清屏
		system("color 73"); //设置颜色
		printf("             ---------------------------\n");
		printf("            |    欢迎光临河南大学餐厅    |\n");
		printf("            |    请输入后厨用户名密码    |\n");
		printf("             ---------------------------\n");
		printf("            |      用户名：              |\n");
		printf("            |      密  码：              |\n");
		printf("             ---------------------------\n");
		toxy(27, 4);
		scanf("%s", _userName);
		toxy(27, 5);
		q = HidePassword();										 //输入密码
		strcpy(_passWord, q);									 //将获得的密码复制给_passWord[]数组
		while (p != NULL && strcmp(p->login_account, _userName) != 0) //遍历链表，寻找当前输入的账户的账号
		{
			p = p->next;
		}
		if (p != NULL) //如果p不为空
		{
			if (strcmp(p->login_account, _passWord) == 0 && strcmp(p->login_type, "厨师") == 0) //如果输入的密码正确
			{
				toxy(32, 16);
				printf("正在登陆....");
				Sleep(500); //暂停0.5秒
				system("cls");
				toxy(32, 10);
				printf("登陆成功！");
				break; //跳出循环
			}
			else //如果输入的密码错误
			{
				toxy(32, 16);
				printf("正在登陆....");
				Sleep(500);
				system("cls");
				toxy(30, 8);
				printf("-------------------------");
				toxy(30, 9);
				printf("|                       |");
				toxy(30, 10);
				printf("| 密码错误！是否继续？  |");
				toxy(30, 11);
				printf("|                       |");
				toxy(30, 12);
				printf("| 1.是             2.否 |");
				toxy(30, 13);
				printf("|                       |");
				toxy(30, 14);
				printf("-------------------------");
				while (1) //无限循环为防止其他按键干扰
				{
					t = _getch(); //输入t
					if (t == '1')
					{
						system("cls"); //清屏
						break;
					}
					else if (t == '2') //如果输入t为n，进入开始界面
					{
						system("cls"); //清屏
						Default();
						break;
					}
				}
			}
		}
		else //如果p为空，即输入的账号不正确
		{
			toxy(32, 16);
			printf("正在登陆....");
			Sleep(500);
			system("cls");
			toxy(30, 8);
			printf("-------------------------");
			toxy(30, 9);
			printf("|                       |");
			toxy(30, 10);
			printf("|  账号错误！是否继续？ |");
			toxy(30, 11);
			printf("|                       |");
			toxy(30, 12);
			printf("|  1.是            2.否 |");
			toxy(30, 13);
			printf("|                       |");
			toxy(30, 14);
			printf("-------------------------");
			while (1) //死循环防止其他按键干扰
			{
				c = _getch();
				if (c == '1')
				{
					system("cls");
					break;
				}
				else if (c == '2')
				{
					system("cls"); //清屏
					Default();
				}
			}
		}
	} while (1);
	Sleep(500);			  //暂停0.5秒
	Cook_Menu(); //账号密码均正确，进入厨师主菜单

}

int file_size(char* filename)
{
	FILE* fp = fopen(filename, "r");
	if (!fp) return -1;
	fseek(fp, 0L, SEEK_END);
	int size = ftell(fp);
	fclose(fp);

	return size;
}
void Cook_list()
{
	pOrder p = readOrederFormFile();
	pUser q, head;
	if (file_size("OrderForm.txt") == 0)
	{
		system("cls");
		toxy(26, 8);
		printf("当前无订单(按任意键返回)");
		_getch();
		Cook_Menu();
	}
	system("cls");
	int i = 10;//行定位
	char t;
	float totalPrice = 0;
	toxy(40, 4);
	printf("-----------");
	toxy(40, 5);
	printf("| 订单列表 |");
	toxy(40, 6);
	printf("-----------");
	toxy(5, 8);
	printf("订单号    菜品名    菜品价格    下单时间               下单账号    备注信息    是否已付    是否完成");
	toxy(5, 9);
	printf("****************************************************************************************************");
	while (p != NULL)
	{
		toxy(7, i);
		if (strcmp(p->if_complete, "no") == 0)//未完成的订单
		{
			printf("%d", p->Ordering_number);
			toxy(15, i);
			printf("%s", p->name);
			toxy(26, i);
			printf("%.2f", p->price);
			//下单时间
			toxy(37, i);
			printf(p->ordering_time);
			//下单账号
			toxy(60, i);
			printf(" %s", p->login_account);
			//备注信息
			toxy(71, i);
			printf(" %s", p->ordering_remarks);
			//是否已付
			toxy(85, i);
			if (strcmp(p->if_pay, "yes") == 0)
			{
				printf(" 是");
			}
			else
			{
				printf(" 否");
			}
			//是否完成
			toxy(97, i);
			if (strcmp(p->if_complete, "yes") == 0)
			{
				printf(" 是");
			}
			else
			{
				printf(" 否");
			}
			i++;
		}
		p = p->next;
	}
	toxy(5, i);
	printf("****************************************************************************************************");
	toxy(26, i + 4);
	printf("(按任意键返回)");
	_getch();
	Cook_Menu();

}
//将已经完成和支付的订单加入流水
void add_pay_info_form()
{
	pOrder p = readOrederFormFile();
	pPay delete_one = (pPay)malloc(Z);
	//delete_one->next = NULL;
	while (p)
	{
		if (strcmp(p->if_pay, "yes") == 0 && strcmp(p->if_complete, "yes") == 0)//加入流水
		{
			strcpy(delete_one->login_account, p->login_account);
			delete_one->Pay_number = 0;
			strcpy(delete_one->name, p->name);
			strcpy(delete_one->ordering_time, p->ordering_time);
			delete_one->price = p->price;
			save_pPay(delete_one);
		}
		p = p->next;
	}

}
//清除已经加入流水的订单
void check_OrderForm()
{
	pOrder p = readOrederFormFile();
	while (p && strcmp(p->if_pay, "yes") == 0 && strcmp(p->if_complete, "yes") == 0)
	{
		p = p->next;
	}
	if (!p)
	{
		resave_OrderForm(NULL);
		return;
	}
	pOrder p_head = p;
	pOrder p_before = p;
	if (!p)
	{
		;
	}
	else
	{
		p = p->next;
		while (p)
		{
			if (strcmp(p->if_pay, "yes") == 0 && strcmp(p->if_complete, "yes") == 0)
			{
				p_before->next = p->next;
				p = p->next;
				continue;
			}
			p = p->next;
			p_before = p_before->next;
		}
	}
	resave_OrderForm(p_head);
}
void Cook_serving()
{
	do
	{
		system("cls");
		pOrder p = readOrederFormFile();
		HideCursor(1);		//显示光标
		pOrder p1;
		int id;
		int i = 10;//行定位
		char t;
		/*
		显示订单
		*/
		toxy(40, 4);
		printf("-----------");
		toxy(40, 5);
		printf("| 订单列表 |");
		toxy(40, 6);
		printf("-----------");
		toxy(5, 8);
		printf("订单号    菜品名    菜品价格    下单时间               下单账号    备注信息    是否已付    是否完成");
		toxy(5, 9);
		printf("****************************************************************************************************");
		while (p != NULL)
		{
			toxy(7, i);
			//修改下输出的位置
			//订单号
			if (strcmp(p->if_complete, "no") == 0)//未完成的订单
			{
				printf("%d", p->Ordering_number);
				toxy(15, i);
				printf("%s", p->name);
				toxy(26, i);
				printf("%.2f", p->price);
				//下单时间
				toxy(37, i);
				printf(p->ordering_time);
				//下单账号
				toxy(60, i);
				printf(" %s", p->login_account);
				//备注信息
				toxy(71, i);
				printf(" %s", p->ordering_remarks);
				//是否已付
				toxy(85, i);
				if (strcmp(p->if_pay, "yes") == 0)
				{
					printf(" 是");
				}
				else
				{
					printf(" 否");
				}
				//是否完成
				toxy(97, i);
				if (strcmp(p->if_complete, "yes") == 0)
				{
					printf(" 是");
				}
				else
				{
					printf(" 否");
				}
				i++;
			}
			p = p->next;
		}
		toxy(5, i);
		printf("****************************************************************************************************");
		p1 = (pOrder)malloc(N);
		toxy(30, i + 4);
		printf("输入已完成订单号:");
		int if_do = 0;
		toxy(48, i + 4);
		scanf("%d", &id);
		p = readOrederFormFile();
		pOrder p_head = p;
		while (p)
		{
			if (p->Ordering_number == id)
			{
				strcpy(p->if_complete, "yes");
				if_do = 1;
			}
			p = p->next;
		}

		resave_OrderForm(p_head);
		add_pay_info_form();
		check_OrderForm();
		system("cls");
		if (if_do == 0)
		{
			toxy(30, 8);
			printf("--------------------------");
			toxy(30, 9);
			printf("|                        |");
			toxy(30, 10);
			printf("|未发现该订单!是否继续？ |");
			toxy(30, 11);
			printf("|                        |");
			toxy(30, 12);
			printf("|  1.是           2.否   |");
			toxy(30, 13);
			printf("|                        |");
			toxy(30, 14);
			printf("--------------------------");
		}
		else
		{
			toxy(30, 8);
			printf("--------------------------");
			toxy(30, 9);
			printf("|                        |");
			toxy(30, 10);
			printf("| 上菜成功！是否继续？   |");
			toxy(30, 11);
			printf("|                        |");
			toxy(30, 12);
			printf("|  1.是           2.否   |");
			toxy(30, 13);
			printf("|                        |");
			toxy(30, 14);
			printf("--------------------------");
		}

		while (1)
		{
			t = _getch(); //输入t
			if (t == '1')
			{
				system("cls");
				break;
			}
			else if (t == '2')
				Cook_Menu();
		}
	} while (1);

}

//调试用 查看用户库
void show_user()
{
	pUser p = readUserFile();
	while (p)
	{
		printf("1.login_account:%s\n", p->login_account);
		printf("2.login_type:%s\n", p->login_type);
		printf("3.login_pwd:%s\n", p->login_pwd);
		printf("4.pick_number:%d\n", p->pick_number);
		printf("5.if_qualified:%s\n", p->if_qualified);
		printf("\n");
		p = p->next;
	}
}

//调试用 查看订单库
void show_Order_Form()
{
	pOrder p = readOrederFormFile();
	while (p)
	{
		printf("%d\n", p->Ordering_number);
		printf("\n%-18s%.2f\n", p->name, p->price);
		//下单时间
		printf(p->ordering_time);
		//下单账号
		printf("\n%s", p->login_account);
		//备注信息
		printf("\n%s", p->ordering_remarks);
		//是否已付
		if (strcmp(p->if_pay, "yes") == 0)
		{
			printf("\n是");
		}
		else
		{
			printf("\n否");
		}
		//是否完成
		if (strcmp(p->if_complete, "yes") == 0)
		{
			printf("\n是");
		}
		else
		{
			printf("\n否");
		}
		p = p->next;
	};
}

//打印当前时间
void ct_now()
{
	time_t timep;
	struct tm* p;
	time(&timep);
	p = gmtime(&timep);
	printf("%d年%d月%d日-%d:%d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, 8 + p->tm_hour, p->tm_min);
}
int main()
{
	Default(); //进入欢迎界面
	return 0;
}
