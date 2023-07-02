#define _CRT_SECURE_NO_WARNINGS
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#include<easyx.h>
#include<graphics.h>
#include<algorithm>
#include<windows.h>
#include<iostream>
#include<stdio.h>
#include<deque>

//游戏核心部分如下(20221104写的)
#include<vector>
#include<iomanip>
#include<ctime>
#include<fstream>
#define Jin '#'//代表未开启的区域
#define Jia '+'//用于标记有雷
using namespace std;//以下初始化二维数组的指针为全局变量
char** gra = NULL;//该指针指向的二维数组存放游戏画面
int** arr = NULL;//该指针指向的二维数组存放有雷（9）或无雷（10）两种情况
int** ar = NULL;//该指针指向的二维数组存放有雷（9）或无雷时其周围九格区域的雷数
IMAGE* nums;
//
void game_void_downbutton(int x, int y);
int kopen(int x, int y)//快捷标记打开函数
{
	void open(int x, int y);
	void open1(int x, int y);
	if (gra[x][y] == Jia)
		return 0;
	else
	{
		if (arr[x][y] == 9)
			return 1;//发现有雷
		else if (ar[x][y] == 0)//周围无雷时递归地开启周围的八格
			open(x, y);
		else
			open1(x, y);//周围有雷时只开启本格
		return 0;
	}
}
void open1(int x, int y)//遇到非0的函数实现，不允许开启的部分继续开启周围八格
{
	putimage((x - 1) * 20, (y - 1) * 20, nums + ar[x][y]);
	gra[x][y] = ar[x][y] + 48;
}
void open(int x, int y)
{
	if (ar[x][y] == 0)
		game_void_downbutton((x - 1) * 20, (y - 1) * 20);
	else
		putimage((x - 1) * 20, (y - 1) * 20, nums + ar[x][y]);
	gra[x][y] = ar[x][y] + 48;//生成数字，展示在屏幕上
	if (ar[x - 1][y] == 0 && gra[x - 1][y] == Jin)
		open(x - 1, y);
	if (ar[x][y - 1] == 0 && gra[x][y - 1] == Jin)
		open(x, y - 1);
	if (ar[x][y + 1] == 0 && gra[x][y + 1] == Jin)
		open(x, y + 1);
	if (ar[x + 1][y] == 0 && gra[x + 1][y] == Jin)
		open(x + 1, y);
	if (ar[x - 1][y - 1] == 0 && gra[x - 1][y - 1] == Jin)
		open(x - 1, y - 1);
	if (ar[x - 1][y + 1] == 0 && gra[x - 1][y + 1] == Jin)
		open(x - 1, y + 1);
	if (ar[x + 1][y - 1] == 0 && gra[x + 1][y - 1] == Jin)
		open(x + 1, y - 1);
	if (ar[x + 1][y + 1] == 0 && gra[x + 1][y + 1] == Jin)
		open(x + 1, y + 1);
	//遇到0时自动开启周围八格，并且开启的部分继续开启周围八格
	if (ar[x - 1][y] > 0 && ar[x - 1][y] < 9 && gra[x - 1][y] == Jin)
		open1(x - 1, y);
	if (ar[x][y - 1] > 0 && ar[x][y - 1] < 9 && gra[x][y - 1] == Jin)
		open1(x, y - 1);
	if (ar[x][y + 1] > 0 && ar[x][y + 1] < 9 && gra[x][y + 1] == Jin)
		open1(x, y + 1);
	if (ar[x + 1][y] > 0 && ar[x + 1][y] < 9 && gra[x + 1][y] == Jin)
		open1(x + 1, y);
	if (ar[x - 1][y - 1] > 0 && ar[x - 1][y - 1] < 9 && gra[x - 1][y - 1] == Jin)
		open1(x - 1, y - 1);
	if (ar[x - 1][y + 1] > 0 && ar[x - 1][y + 1] < 9 && gra[x - 1][y + 1] == Jin)
		open1(x - 1, y + 1);
	if (ar[x + 1][y - 1] > 0 && ar[x + 1][y - 1] < 9 && gra[x + 1][y - 1] == Jin)
		open1(x + 1, y - 1);
	if (ar[x + 1][y + 1] > 0 && ar[x + 1][y + 1] < 9 && gra[x + 1][y + 1] == Jin)
		open1(x + 1, y + 1);
	//遇到非0时自动开启周围八格，不允许开启的部分继续开启周围八格
}
void game_button(int x, int y)
{
	setcolor(0x808a87);
	setfillcolor(0xC0C0C0);
	setfillstyle(BS_SOLID);
	fillrectangle(x, y, x + 18, y + 18);
}
void game_downbutton(int x, int y)
{
	setcolor(0x808a87);
	setfillcolor(0x878787);
	setfillstyle(BS_SOLID);
	fillrectangle(x, y, x + 18, y + 18);
}
void game_void_downbutton(int x, int y)
{
	setcolor(RGB(192,192,192));
	setfillcolor(RGB(255, 255, 255));
	setfillstyle(BS_SOLID);
	fillrectangle(x, y, x + 18, y + 18);
}
void init_game_graph(int n)
{
	initgraph(20 * n, 20 * n);
	setbkcolor(WHITE);
	cleardevice();
	for (int i = 1; i <= n; ++i)
	{
		for (int j = 1; j <= n; ++j)
		{
			game_button((i - 1) * 20, (j - 1) * 20);
		}
	}
}
void fail(int n,int gametimestart)
{
	IMAGE bomb;
	loadimage(&bomb, _T("bomb.png"), 18, 18);
	for (int i = 1; i <= n; ++i)
	{
		for (int j = 0; j <= n; ++j)
		{
			if (arr[i][j] == 9)
			{
				putimage((i - 1) * 20, (j - 1) * 20, &bomb);
			}
		}
	}
	unsigned int gametimeend = (unsigned int)time(NULL);
	unsigned int gametime = gametimeend - gametimestart;//输出游戏时间
	TCHAR s[50];
	_stprintf(s, _T("游戏失败，本次用时%d秒"), gametime);
	freopen("boomdata.txt", "a", stdout);
	//cout << 2 << " " << gametime << endl;
	printf("2 %d\n", gametime);
	fclose(stdout);
	//cout << "本次游戏用时" << gametime << "秒" << endl;
	//file(1, gametime);//游戏记录写入文件
	MessageBox(NULL, s, _T("你没了"), MB_OK);
	closegraph();
	for (int i = 0; i < n + 2; ++i)
		delete[] gra[i];
	delete[] gra;
	for (int i = 0; i < n + 2; ++i)
		delete[] arr[i];
	delete[] arr;
	for (int i = 0; i < n + 2; ++i)
		delete[] ar[i];
	delete[] ar;
	delete[] nums;
	return;
}
void game(int n, int boom)
{
	srand((unsigned int)time(NULL));
	unsigned int gametimestart = (unsigned int)time(NULL);//记录游戏开始的时间
	arr = new int* [n + 2];
	for (int i = 0; i < n + 2; i++)
		arr[i] = new int[n + 2];//定义二维数组arr
	ar = new int* [n + 2];
	for (int i = 0; i < n + 2; i++)
		ar[i] = new int[n + 2];//定义二维数组ar
	for (int i = 0; i < n + 2; i++)
	{
		arr[0][i] = 10;
		arr[n + 1][i] = 10;
		arr[i][0] = 10;
		arr[i][n + 1] = 10;//初始化边缘无雷
		ar[0][i] = 10;
		ar[n + 1][i] = 10;//10表示无雷
		ar[i][0] = 10;
		ar[i][n + 1] = 10;
	}
	vector<int>v;
	v.resize(n * n, 10);
	for (vector<int>::iterator it = v.begin(); it < v.begin() + boom; it++)
		*it = 9;//9表示有雷
	random_shuffle(v.begin(), v.end());//随机生成n个位置夹杂boom个雷的排列
	for (int i = 0; i < n * n; i++)
		arr[i / n + 1][i % n + 1] = v[i];//将雷填充入二维数组arr
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)//遍历二维数组arr每个位置
		{
			if (arr[i][j] == 9)//如果是雷，遍历下一个位置
				ar[i][j] = 9;
			else
			{
				ar[i][j] = arr[i][j] + arr[i - 1][j - 1] + arr[i - 1][j] + arr[i - 1][j + 1]
					+ arr[i][j - 1] + arr[i][j + 1] + arr[i + 1][j - 1] + arr[i + 1][j]
					+ arr[i + 1][j + 1];
				ar[i][j] = 90 - ar[i][j];//如果不是雷，计算其周边八格雷的个数，并赋值给ar
			}
		}
	}
	gra = new char* [n + 2];
	for (int i = 0; i < n + 2; i++)
		gra[i] = new char[n + 2];//定义用于显示游戏页面的二维数组gra
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			gra[i][j] = Jin;//初始化用Jin填充
		}
	}
	closegraph();
	init_game_graph(n);
	//system("pause");
	int x, y, z, m1 = 0, right = 0;
	IMAGE redflag;
	TCHAR s[15];
	nums = new IMAGE[9];
	for (int i = 1; i <= 8; ++i)
	{
		_stprintf(s, _T("./num%d.png"),i);
		loadimage(nums + i, s, 18, 18);
	}
	loadimage(&redflag, _T("./flag2.png"), 18, 18);
	while (1)
	{
		if (m1 == boom && right == boom)//采用m1(标记点个数)和right(正确点个数)判断是否赢了
		{
			//printgraph(n);
			//cout << "你赢了" << endl;
			unsigned int gametimeend = (unsigned int)time(NULL);
			unsigned int gametime = gametimeend - gametimestart;//输出游戏时间
			TCHAR s[50];
			_stprintf(s, _T("你赢了，本次用时%d秒"), gametime);
			//cout << "本次游戏用时" << gametime << "秒" << endl;
			//file(1, gametime);//游戏记录写入文件
			freopen("boomdata.txt", "a", stdout);
			//cout << 1 << " " << gametime << endl;
			printf("1 %d\n", gametime);
			fclose(stdout);
			MessageBox(NULL, s,_T("你赢了"), MB_OK);
			closegraph();
			for (int i = 0; i < n + 2; ++i)
				delete[] gra[i];
			delete[] gra;
			for (int i = 0; i < n + 2; ++i)
				delete[] arr[i];
			delete[] arr;
			for (int i = 0; i < n + 2; ++i)
				delete[] ar[i];
			delete[] ar;
			delete[] nums;
			return;
		}
		ExMessage msg;
		if (peekmessage(&msg, EM_MOUSE))
		{
			if (msg.x % 20 >= 18 && msg.x % 20 <= 20 || msg.y % 20 >= 18 && msg.y % 20 <= 20)
				continue;
			switch (msg.message)
			{
			case WM_LBUTTONDOWN:
				x = msg.x / 20 + 1;
				y = msg.y / 20 + 1;
				game_downbutton((x - 1) * 20, (y - 1) * 20);
				while (1)
				{
					if (peekmessage(&msg, EM_MOUSE))
					{
						if (msg.message == WM_LBUTTONUP)//1
						{
							z = 1;
							if (ar[x][y] == 9)
							{
								fail(n, gametimestart);
								return;
							}
							else
							{
								gra[x][y] = ar[x][y] + 48;
								if (ar[x][y] == 0)
								{
									game_void_downbutton((x - 1) * 20, (y - 1) * 20);
									open(x, y);
								}
								else
								{
									putimage((x - 1) * 20, (y - 1) * 20, nums + ar[x][y]);
								}
							}
							break;
						}
						else if (msg.message == WM_RBUTTONUP || msg.message == WM_RBUTTONDOWN)
						{
							z = 4;
							goto condition4;
							//cout << 4 << endl;//4
							break;
						}
					}
				}
				//game_button((x - 1) * 20, (y - 1) * 20);
				break;
			case WM_RBUTTONDOWN:
				x = msg.x / 20 + 1;
				y = msg.y / 20 + 1;
				game_downbutton((x - 1) * 20, (y - 1) * 20);
				while (1)
				{
					if (peekmessage(&msg, EM_MOUSE))
					{
						if (msg.message == WM_RBUTTONUP)
						{
							z = 2;
							if (gra[x][y] == Jin)//插旗
							{
								gra[x][y] = Jia;
								m1++;
								right = (ar[x][y] == 9) ? (right + 1) : right;
								putimage((x - 1) * 20, (y - 1) * 20, &redflag);
							}
							else if (gra[x][y] == Jia)//取消插旗
							{
								gra[x][y] = Jin;
								m1--;
								right = (ar[x][y] == 9) ? (right - 1) : right;
								game_button((x - 1) * 20, (y - 1) * 20);
							}
							else if (ar[x][y] == 0)
								game_void_downbutton((x - 1) * 20, (y - 1) * 20);
							else
								putimage((x - 1) * 20, (y - 1) * 20, nums + ar[x][y]);
							//cout << 2 << endl;//2
							break;
						}
						else if (msg.message == WM_LBUTTONUP || msg.message == WM_LBUTTONDOWN)
						{
							z = 4;
						condition4:
							if (gra[x][y] != Jin && gra[x][y] != Jia)//快捷标记安全实现
							{
								int jnum = 0;
								jnum = (gra[x - 1][y - 1] == Jia ? jnum + 1 : jnum);
								jnum = (gra[x - 1][y] == Jia ? jnum + 1 : jnum);
								jnum = (gra[x - 1][y + 1] == Jia ? jnum + 1 : jnum);
								jnum = (gra[x][y - 1] == Jia ? jnum + 1 : jnum);
								jnum = (gra[x][y + 1] == Jia ? jnum + 1 : jnum);
								jnum = (gra[x + 1][y - 1] == Jia ? jnum + 1 : jnum);
								jnum = (gra[x + 1][y] == Jia ? jnum + 1 : jnum);
								jnum = (gra[x + 1][y + 1] == Jia ? jnum + 1 : jnum);//统计周围标记有雷点个数
								if (jnum != ar[x][y])
								{
									game_button((x - 1) * 20, (y - 1) * 20);
								}
								else
								{
									int failnum = 0;//检查是否踩雷
									failnum += kopen(x - 1, y - 1);
									failnum += kopen(x - 1, y);
									failnum += kopen(x - 1, y + 1);
									failnum += kopen(x, y - 1);
									failnum += kopen(x, y + 1);
									failnum += kopen(x + 1, y - 1);
									failnum += kopen(x + 1, y);
									failnum += kopen(x + 1, y + 1);
									if (failnum != 0)
									{
										fail(n, gametimestart);
										return;
									}
								}
							}
							if (gra[x][y] == Jin)
							{
								game_button((x - 1) * 20, (y - 1) * 20);
							}
							else if (gra[x][y] == Jia)
							{
								putimage((x - 1) * 20, (y - 1) * 20, &redflag);
							}
							else if (ar[x][y] == 0)
							{
								game_void_downbutton((x - 1) * 20, (y - 1) * 20);
							}
							else
								putimage((x - 1) * 20, (y - 1) * 20, nums + ar[x][y]);
							//cout << 4 << endl;//4
							break;
						}
					}
				}
				//game_button((x - 1) * 20, (y - 1) * 20);
				break;
			}
		}
	}
}

//游戏设置函数如下
//50 X 30
void draw_set_button(int x,int y)//50 X 30
{
	setcolor(RGB(34,89,168));
	setfillcolor(RGB(52,135,255));
	setfillstyle(BS_SOLID);
	fillrectangle(x, y, x + 50, y + 30);
}
void draw_set_clicked_button(int x, int y)//50 X 30
{
	setcolor(RGB(15,38,71));
	setfillcolor(RGB(34, 89, 168));
	setfillstyle(BS_SOLID);
	fillrectangle(x, y, x + 50, y + 30);
}
void draw_home_button(int x, int y);
void draw_clicked_button(int x, int y);
void draw_game_setting_bk(int n,int boom)
{
	//cleardevice();
	TCHAR s[30];
	//标题：游戏设置
	settextcolor(BLUE);
	settextstyle(50, 0, L"黑体");
	setbkmode(TRANSPARENT);
	_stprintf(s, _T("游戏设置"));
	outtextxy(200, 10, s);
	//雷区大小
	//方形
	setcolor(0x050B0D);
	setfillcolor(0x74959C);
	setfillstyle(BS_SOLID);
	fillrectangle(10, 160, 10 + 200, 160 + 50);
	//字体
	settextcolor(BLACK);
	settextstyle(30, 0, L"黑体");
	setbkmode(TRANSPARENT);
	_stprintf(s, _T("雷区大小：%d"),n);
	outtextxy(15, 165, s);
	//按钮
	draw_set_button(300, 170);
	draw_set_button(370, 170);
	draw_set_button(440, 170);
	draw_set_button(510, 170);
	//按钮字体
	settextcolor(BLACK);
	settextstyle(25, 0, L"黑体");
	setbkmode(TRANSPARENT);
	_stprintf(s, _T("+1"));
	outtextxy(310, 170, s);
	_stprintf(s, _T("-1"));
	outtextxy(380, 170, s); 
	_stprintf(s, _T("+10"));
	outtextxy(450, 170, s);
	_stprintf(s, _T("-10"));
	outtextxy(510, 170, s);

	//地雷个数
	//方形
	setcolor(0x050B0D);
	setfillcolor(0x74959C);
	setfillstyle(BS_SOLID);
	fillrectangle(10, 380, 10 + 200, 380 + 50);
	//字体
	settextcolor(BLACK);
	settextstyle(30, 0, L"黑体");
	setbkmode(TRANSPARENT);
	_stprintf(s, _T("地雷个数：%d"),boom);
	outtextxy(15, 385, s);
	//按钮
	draw_set_button(300, 390);
	draw_set_button(370, 390);
	draw_set_button(440, 390);
	draw_set_button(510, 390);
	//按钮字体
	settextcolor(BLACK);
	settextstyle(25, 0, L"黑体");
	setbkmode(TRANSPARENT);
	_stprintf(s, _T("+1"));
	outtextxy(310, 390, s);
	_stprintf(s, _T("-1"));
	outtextxy(380, 390, s);
	_stprintf(s, _T("+10"));
	outtextxy(450, 390, s);
	_stprintf(s, _T("-10"));
	outtextxy(510, 390, s);
	//开始游戏
	draw_home_button(180, 510);
	settextcolor(RED);
	settextstyle(35, 0, L"黑体");
	setbkmode(TRANSPARENT);
	_stprintf(s, _T("开始游戏"));
	outtextxy(250,515, s);
}
void game_settings()
{
	int n = 10,boom=1;
	cleardevice();
	draw_game_setting_bk(n,boom);
	int change_flag = 0;
	while (1)
	{
		if (change_flag)
		{
			draw_game_setting_bk(n,boom);
			change_flag = 0;
		}
		ExMessage msg;
		if (peekmessage(&msg, EM_MOUSE))
		{
			switch (msg.message)
			{
			case WM_LBUTTONDOWN:
				if (msg.x >= 300 && msg.x <= 300+50 && msg.y >= 170 && msg.y <= 170+30)//按键
				{
					draw_set_clicked_button(300, 170);
					change_flag = 1;
					while (1)
					{
						if (peekmessage(&msg, EM_MOUSE))
						{
							if (msg.message == WM_LBUTTONUP)
							{
								++n;
								n = min(n, 50);
								break;
							}
						}
					}
				}
				
				else if (msg.x >= 370 && msg.x <= 370 + 50 && msg.y >= 170 && msg.y <= 170 + 30)//按键
				{
					draw_set_clicked_button(370, 170);
					change_flag = 1;
					while (1)
					{
						if (peekmessage(&msg, EM_MOUSE))
						{
							if (msg.message == WM_LBUTTONUP)
							{
								--n;
								n = max(n, 10);
								boom = min(boom, n * n);
								break;
							}
						}
					}
				}
				
				else if (msg.x >= 440 && msg.x <= 440 + 50 && msg.y >= 170 && msg.y <= 170 + 30)//按键
				{
					draw_set_clicked_button(440, 170);
					change_flag = 1;
					while (1)
					{
						if (peekmessage(&msg, EM_MOUSE))
						{
							if (msg.message == WM_LBUTTONUP)
							{
								n+=10;
								n = min(n, 50);
								break;
							}
						}
					}
				}
				
				else if (msg.x >= 510 && msg.x <= 510 + 50 && msg.y >= 170 && msg.y <= 170 + 30)//按键
				{
					draw_set_clicked_button(510, 170);
					change_flag = 1;
					while (1)
					{
						if (peekmessage(&msg, EM_MOUSE))
						{
							if (msg.message == WM_LBUTTONUP)
							{
								n-=10;
								n = max(n,10);
								boom = min(boom, n * n);
								break;
							}
						}
					}
				}
				else if (msg.x >= 300 && msg.x <= 300 + 50 && msg.y >= 390 && msg.y <= 390 + 30)//按键
				{
					draw_set_clicked_button(300,390);
					change_flag = 1;
					while (1)
					{
						if (peekmessage(&msg, EM_MOUSE))
						{
							if (msg.message == WM_LBUTTONUP)
							{
								++boom;
								boom = min(boom, n * n);
								break;
							}
						}
					}
				}
				else if (msg.x >= 370 && msg.x <= 370 + 50 && msg.y >= 390 && msg.y <= 390 + 30)//按键
				{
					draw_set_clicked_button(370, 390);
					change_flag = 1;
					while (1)
					{
						if (peekmessage(&msg, EM_MOUSE))
						{
							if (msg.message == WM_LBUTTONUP)
							{
								--boom;
								boom = max(boom,1);
								break;
							}
						}
					}
				}
				else if (msg.x >= 440 && msg.x <= 440 + 50 && msg.y >= 390 && msg.y <= 390 + 30)//按键
				{
					draw_set_clicked_button(440, 390);
					change_flag = 1;
					while (1)
					{
						if (peekmessage(&msg, EM_MOUSE))
						{
							if (msg.message == WM_LBUTTONUP)
							{
								boom+=10;
								boom = min(boom, n * n);
								break;
							}
						}
					}
				}
				else if (msg.x >= 510 && msg.x <= 510 + 50 && msg.y >= 390 && msg.y <= 390 + 30)//按键
				{
					draw_set_clicked_button(510, 390);
					change_flag = 1;
					while (1)
					{
						if (peekmessage(&msg, EM_MOUSE))
						{
							if (msg.message == WM_LBUTTONUP)
							{
								boom-=10;
								boom = max(boom,1);
								break;
							}
						}
					}
					}
				else if (msg.x >= 180 && msg.x <= 180+ 300 && msg.y >= 510&& msg.y <= 510 + 50)//开始游戏
				{
					draw_clicked_button(180, 510);
					change_flag = 1;
					while (1)
					{
						if (peekmessage(&msg, EM_MOUSE))
						{
							if (msg.message == WM_LBUTTONUP)
							{
								game(n, boom);
								return;
							}
						}
					}
					}
				break;
			}
		}
	}


}

//主页面绘制函数如下
void draw_home_button(int x,int y)//300 X 50
{
	setcolor(0x050B0D);
	setfillcolor(0x74959C);
	setfillstyle(BS_SOLID);
	fillrectangle(x, y, x + 300, y + 50);
}
void draw_clicked_button(int x, int y)
{
	setcolor(0x2D6775);
	setfillcolor(0xA3D2DB);
	setfillstyle(BS_SOLID);
	fillrectangle(x, y, x + 300, y + 50);
}
void draw_all_button()//home页面按钮
{
	TCHAR s[30];
	//按钮1开始游戏
	draw_home_button(150, 280);
	settextcolor(RED);
	settextstyle(35, 0, L"黑体");
	setbkmode(TRANSPARENT);
	_stprintf(s, _T("开始游戏"));
	outtextxy(230, 285, s);
	//按钮2游戏记录
	draw_home_button(150, 350);
	settextcolor(RED);
	settextstyle(35, 0, L"黑体");
	setbkmode(TRANSPARENT);
	_stprintf(s, _T("游戏记录"));
	outtextxy(230, 355, s);
	//按钮3开通会员
	draw_home_button(150, 420);
	settextcolor(RED);
	settextstyle(35, 0, L"黑体");
	setbkmode(TRANSPARENT);
	_stprintf(s, _T("游戏会员"));
	outtextxy(230, 425, s);
}
void inithome()//创建主页面
{
	//初始化窗口
	setbkcolor(WHITE);
	cleardevice();
	//加载图像
	IMAGE img;
	loadimage(&img, _T("./logo.jpg"), 250, 250);
	putimage(10, 10, &img);
	//写标题（扫雷游戏）
	settextcolor(RED);
	settextstyle(60, 0, L"楷体");
	TCHAR s[30];
	_stprintf(s, _T("扫雷游戏"));
	outtextxy(330, 50, s);
	//作者：CJL_sysu
	settextcolor(BLACK);
	settextstyle(20, 0, L"黑体");
	_stprintf(s, _T("作者：CJL_sysu"));
	outtextxy(360, 130, s);
	//版本号
	settextcolor(BLACK);
	settextstyle(20, 0, L"黑体");
	_stprintf(s, _T("版本号4.0.0"));
	outtextxy(370, 170, s);
	draw_all_button();
}
void return_button(int x,int y)
{
	//方框
	setcolor(RGB(124, 147, 204));
	setfillcolor(RGB(124, 147, 204));
	setfillstyle(BS_SOLID);
	fillrectangle(x, y, x + 50, y + 30);
}
void return_button_press(int x, int y)
{
	//方框
	setcolor(RGB(124, 147, 204));
	setfillcolor(RGB(85,101,140));
	setfillstyle(BS_SOLID);
	fillrectangle(x, y, x + 50, y + 30);
}
void make_VIP()
{
	cleardevice();
	IMAGE VIP;
	loadimage(&VIP, _T("VIP.jpg"), 403, 550);
	putimage(129, 50, &VIP);
	//标题
	settextcolor(BLACK);
	settextstyle(25, 0, L"黑体");
	outtextxy(129, 10, _T("Please V CJL 50 during crazy Thursday!"));
	//返回按钮
	return_button(8, 8);
	//文字
	settextcolor(BLACK);
	settextstyle(20, 0, L"黑体");
	outtextxy(12, 12, _T("返回"));
	while (1)
	{
		ExMessage msg;
		if (peekmessage(&msg, EM_MOUSE))
		{
			if (msg.message == WM_LBUTTONDOWN&& msg.x >= 8 && msg.x <= 58 && msg.y >= 8 && msg.y <= 38)
			{
				while (1)
				{
					return_button_press(8, 8);
					if (peekmessage(&msg, EM_MOUSE))
					{
						if (msg.message == WM_LBUTTONUP)
							return;
					}
				}
			}
		}
	}

}

void history()
{
	cleardevice();
	settextcolor(BLACK);
	settextstyle(25, 0, L"黑体");
	outtextxy(220, 10, _T("历史记录(最近十条)"));
	vector<pair<int, int>>vec;
	FILE* f=freopen("boomdata.txt", "r", stdin);
	int win, time;
	while (scanf("%d %d",&win,&time)!=-1)
	{
		vec.emplace_back(make_pair(win, time));
	}
	fclose(stdin);
	if (vec.size() <= 10)
	{
		for (int i = 0; i < 10&&i<vec.size(); ++i)
		{
			TCHAR str[50];
			settextcolor(BLACK);
			settextstyle(25, 0, L"黑体");
			if(vec[i].first==1)
				_stprintf(str, _T("%d、游戏成功，用时%d秒"),i+1,vec[i].second);
			else
				_stprintf(str, _T("%d、游戏失败，用时%d秒"), i + 1, vec[i].second);
			outtextxy(20, 50+i*55, str);

		}
	}
	else
	{
		freopen("boomdata.txt", "w", stdout);
		vector<pair<int, int>>vec10(10);
		copy(vec.begin() + ((int)vec.size() - 10), vec.end(), vec10.begin());
		for (int i = 0; i < 10; ++i)
		{
			//cout << vec[i].first << " " << vec[i].second << endl;
			printf("%d %d\n", vec10[i].first, vec10[i].second);
			TCHAR str[50];
			settextcolor(BLACK);
			settextstyle(25, 0, L"黑体");
			if (vec10[i].first == 1)
				_stprintf(str, _T("%d、游戏成功，用时%d秒"), i + 1, vec10[i].second);
			else
				_stprintf(str, _T("%d、游戏失败，用时%d秒"), i + 1, vec10[i].second);
			outtextxy(20, 50 + i * 55, str);
		}
		fclose(stdout);
	}
	//返回按钮
	return_button(8, 8);
	//文字
	settextcolor(BLACK);
	settextstyle(20, 0, L"黑体");
	outtextxy(12, 12, _T("返回"));
	while (1)
	{
		ExMessage msg;
		if (peekmessage(&msg, EM_MOUSE))
		{
			if (msg.message == WM_LBUTTONDOWN && msg.x >= 8 && msg.x <= 58 && msg.y >= 8 && msg.y <= 38)
			{
				while (1)
				{
					return_button_press(8, 8);
					if (peekmessage(&msg, EM_MOUSE))
					{
						if (msg.message == WM_LBUTTONUP)
							return;
					}
				}
			}
		}
	}
}
void mouse_control()
{
	int change_flag = 0;
	while (1)
	{
		if (change_flag)
		{
			draw_all_button();
			change_flag = 0;
		}
		ExMessage msg;
		if (peekmessage(&msg, EM_MOUSE))
		{
			switch (msg.message)
			{
			case WM_LBUTTONDOWN:
				if (msg.x >= 150 && msg.x <= 450 && msg.y >= 280 && msg.y <= 330)//按键1
				{
					draw_clicked_button(150, 280);
					change_flag = 1;
					while (1)
					{
						if (peekmessage(&msg, EM_MOUSE))
						{
							if (msg.message == WM_LBUTTONUP)
							{
								game_settings();
								initgraph(640, 600);
								inithome();
								break;
							}
						}
					}
				}
				if (msg.x >= 150 && msg.x <= 450 && msg.y >= 350 && msg.y <= 400)//按键2
				{
					draw_clicked_button(150, 350);
					change_flag = 1;
					while (1)
					{
						if (peekmessage(&msg, EM_MOUSE))
						{
							if (msg.message == WM_LBUTTONUP)
							{
								history();
								inithome();
								break;
							}
						}
					}
				}
				if (msg.x >= 150 && msg.x <= 450 && msg.y >= 420 && msg.y <= 470)//按键3
				{
					draw_clicked_button(150, 420);
					change_flag = 1;
					while (1)
					{
						if (peekmessage(&msg, EM_MOUSE))
						{
							if (msg.message == WM_LBUTTONUP)
							{
								make_VIP();
								inithome();
								break;
							}
						}
					}
				}
				break;
			}
		}
	}
}
int main()
{
	initgraph(640, 600);
	inithome();
	mouse_control();
	closegraph();
	return 0;
}