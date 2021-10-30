#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <string.h>

//全局变量区


//——————————————————————————————————————函数声明区—————————————————————————————————————————————————————————
//页面
void Welcome();        //欢迎页面
void Wel_ope(char ch); //欢迎页面操作
void Customer();       //顾客页面
void Cus_ope(char ch); //顾客页面操作
void Chef();           //厨师页面
void Che_ope(char ch); //厨师页面操作
void Owner();          //商家页面
void Own_ope(char ch); //商家页面操作

//欢迎页面功能实现
void CusLogin(); //顾客登录
void CheLogin(); //厨师登录
void OwnLogin(); //商家登录

//顾客页面功能实现
void Cus_order();  //顾客点餐
void Cus_change(); //顾客修改密码
void Register();   //顾客注册

//厨师页面功能实现
void Che_check();    //查看未完成订单
void Che_complete(); //完成订单

//商家页面功能实现
void Own_check();  //商家查看流水
void Own_chmenu(); //商家修改菜单

//通用功能
void toxy(int x, int y); //将光标移动到x，y坐标处
char *HidePassword();    //隐藏密码

//——————————————————————————————————————————————————————————————————————————————————————————————————





//———————————————————————————————函数实现区————————————————————————————————————————————————————————————————
//页面
void Welcome() //欢迎页面
{
    system("color 74"); //设置颜色
    char ch;
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
    while (1)
    {
        ch = getch();
        Wel_ope(ch);
    }
}
void Wel_ope(char ch)
{
    switch (ch)
    {
    case '1':
        system("cls");
        CusLogin();
        break;
    case '2':
        system("cls");
        CheLogin();
        break;
    case '3':
        system("cls");
        OwnLogin();
        break;
    case '4':
        system("cls");
        Register();
        break;
    default:
        system("cls");
        toxy(25, 10);
        printf("输入错误请重新输入！");
        Sleep(1000);
        system("cls");
        Welcome();
        break;
    }
}
void Customer() //顾客页面
{
    char ch;
    printf("             ---------------------------\n");
    printf("            |    欢迎光临河南大学餐厅    |\n");
    printf("            |       请选择执行操作       |\n");
    printf("             ---------------------------\n");
    printf("            |      1      立即点餐       |\n");
    printf("            |      2      修改密码       |\n");
    printf("            |      3      退出登录       |\n");
    printf("             ---------------------------\n");
    printf("            输入序号：");
    while (1)
    {
        ch = getch();
        Cus_ope(ch);
    }
}
void Cus_ope(char ch)
{
    switch (ch)
    {
    case '1':
        system("cls");
        Cus_order();
        break;
    case '2':
        system("cls");
        Cus_change();
        break;
    case '3':
        system("cls");
        Welcome();
        break;
    default:
        system("cls");
        toxy(25, 10);
        printf("输入错误请重新输入！");
        Sleep(1000);
        system("cls");
        Welcome();
        break;
    }
}
void Chef() //厨师页面
{
    char ch;
    printf("             ---------------------------\n");
    printf("            |    欢迎光临河南大学餐厅    |\n");
    printf("            |       请选择执行操作       |\n");
    printf("             ---------------------------\n");
    printf("            |      1      查看订单       |\n");
    printf("            |      2      完成订单       |\n");
    printf("            |      3      退出登录       |\n");
    printf("             ---------------------------\n");
    printf("            输入序号：");
    while (1)
    {
        ch = getch();
        Che_ope(ch);
    }
}
void Che_ope(char ch)
{
    switch (ch)
    {
    case '1':
        system("cls");
        Che_check();
        break;
    case '2':
        system("cls");
        Che_complete();
        break;
    case '3':
        system("cls");
        Welcome();
        break;
    default:
        system("cls");
        toxy(25, 10);
        printf("输入错误请重新输入！");
        Sleep(1000);
        system("cls");
        Welcome();
        break;
    }
}
void Owner() //商家页面
{
    char ch;
    printf("             ---------------------------\n");
    printf("            |    欢迎光临河南大学餐厅    |\n");
    printf("            |       请选择执行操作       |\n");
    printf("             ---------------------------\n");
    printf("            |      1      查看流水       |\n");
    printf("            |      2      修改菜单       |\n");
    printf("            |      3      退出登录       |\n");
    printf("             ---------------------------\n");
    printf("            输入序号：");
    while (1)
    {
        ch = getch();
        Own_ope(ch);
    }
}
void Own_ope(char ch)
{
    switch (ch)
    {
    case '1':
        system("cls");
        Own_check();
        break;
    case '2':
        system("cls");
        Own_chmenu();
        break;
    case '3':
        system("cls");
        Welcome();
        break;
    default:
        system("cls");
        toxy(25, 10);
        printf("输入错误请重新输入！");
        Sleep(1000);
        system("cls");
        Welcome();
        break;
    }
}

//顾客功能实现
void CusLogin() //顾客登录
{
    bool check = false; //登录是否成功
    char cus_name[10];  //顾客用户名
    char cus_pwd[18];   //顾客密码
    char *pwd;
    printf("             ---------------------------\n");
    printf("            |    欢迎光临河南大学餐厅    |\n");
    printf("            |     请输入用户名密码       |\n");
    printf("             ---------------------------\n");
    printf("            |      用户名：              |\n");
    printf("            |      密  码：              |\n");
    printf("             ---------------------------\n");
    toxy(27, 4);
    scanf("%s", cus_name);
    toxy(27, 5);
    pwd = HidePassword();
    strcpy(cus_pwd, pwd);
    toxy(19, 8);
    printf("正在登陆.");
    Sleep(500);
    printf(".");
    Sleep(500);
    printf(".");
    Sleep(500);
    if (check)
    {
        system("cls");
        toxy(20, 4);
        printf("登陆成功！");
        Sleep(1000);
        system("cls");
        Customer();
    }
    else
    {
        system("cls");
        toxy(20, 4);
        printf("登陆失败！");
        Sleep(1000);
        system("cls");
        Welcome();
    }
}
void Cus_order() //顾客点餐
{
}
void Cus_change() //顾客修改密码
{
}
void Register() //顾客注册
{
    bool check=false;
    char cus_name[10]; //顾客用户名
    char cus_pwd[18];  //顾客密码
    char cus_rep[18];  //确认密码
    char *pwd;
    printf("             ---------------------------\n");
    printf("            |    欢迎光临河南大学餐厅    |\n");
    printf("            |     请输入用户名密码       |\n");
    printf("             ---------------------------\n");
    printf("            |      用 户 名：            |\n");
    printf("            |      密    码：            |\n");
    printf("            |      确认密码：            |\n");
    printf("             ---------------------------\n");
    toxy(29, 4);
    scanf("%s", cus_name);
    toxy(29, 5);
    pwd = HidePassword();
    strcpy(cus_pwd, pwd);
    toxy(29, 6);
    pwd = HidePassword();
    strcpy(cus_rep, pwd);
    if(check)
    {
        system("cls");
        toxy(20, 4);
        printf("注册成功！");
        Sleep(1000);
        system("cls");
        Welcome();
    }
    else
    {
        system("cls");
        toxy(20, 4);
        printf("用户名已存在！请重新注册！");
        Sleep(1000);
        system("cls");
        Welcome();
    }
}

//厨师功能实现
void CheLogin() //厨师登录
{
    bool check=false;
    char che_name[10]; //顾客用户名
    char che_pwd[18];  //顾客密码
    char *pwd;
    printf("             ---------------------------\n");
    printf("            |    欢迎光临河南大学餐厅    |\n");
    printf("            |     请输入用户名密码       |\n");
    printf("             ---------------------------\n");
    printf("            |      用户名：              |\n");
    printf("            |      密  码：              |\n");
    printf("             ---------------------------\n");
    toxy(27, 4);
    scanf("%s", che_name);
    toxy(27, 5);
    pwd = HidePassword();
    strcpy(che_pwd, pwd);
    toxy(19, 8);
    printf("正在登陆.");
    Sleep(500);
    printf(".");
    Sleep(500);
    printf(".");
    Sleep(500);
    if (check)
    {
        system("cls");
        toxy(20, 4);
        printf("登陆成功！");
        Sleep(1000);
        system("cls");
        Chef();
    }
    else
    {
        system("cls");
        toxy(20, 4);
        printf("登陆失败！");
        Sleep(1000);
        system("cls");
        Welcome();
    }
}
void Che_check() //查看未完成订单
{
}
void Che_complete() //完成订单
{
}

//商家功能实现
void OwnLogin() //商家登录
{
    bool check=true;
    char own_name[10]; //顾客用户名
    char own_pwd[18];  //顾客密码
    char *pwd;
    printf("             ---------------------------\n");
    printf("            |    欢迎光临河南大学餐厅    |\n");
    printf("            |     请输入用户名密码       |\n");
    printf("             ---------------------------\n");
    printf("            |      用户名：              |\n");
    printf("            |      密  码：              |\n");
    printf("             ---------------------------\n");
    toxy(27, 4);
    scanf("%s", own_name);
    toxy(27, 5);
    pwd = HidePassword();
    strcpy(own_pwd, pwd);
     toxy(19, 8);
    printf("正在登陆.");
    Sleep(500);
    printf(".");
    Sleep(500);
    printf(".");
    Sleep(500);
    if (check)
    {
        system("cls");
        toxy(20, 4);
        printf("登陆成功！");
        Sleep(1000);
        system("cls");
        Owner();
    }
    else
    {
        system("cls");
        toxy(20, 4);
        printf("登陆失败！");
        Sleep(1000);
        system("cls");
        Welcome();
    }
}
void Own_check() //商家查看流水
{
}
void Own_chmenu() //商家修改菜单
{
}

//通用功能
void toxy(int x, int y) //将光标移动到x，y坐标处
{
    COORD pos = {x, y};
    HANDLE Out = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(Out, pos);
}
char *HidePassword() //隐藏密码
{
    char password[18]; //密码
    char *p;           //指向密码的指针
    int i = 0;         //记录密码位数
    char t;            //输入密码
    for (;;)           //此处为输入密码不回显操作
    {
        t = getch();   //输入k
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
            printf("*");     //输出*号，保护用户隐私
            i++;             //密码位数加1
        }
    }
    password[i] = '\0'; //在数组末端加一个结束符
    p = password;       //让p指向该数组
    return p;           //返回一个指向密码的指针
}

int main()
{
    Welcome();
    return 0;
}
