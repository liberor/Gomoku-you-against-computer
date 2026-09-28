#include <windows.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define Board_size 15
#define sleep_ms 0

#include <easyx.h>
#include <conio.h>
void print_board(int b[][Board_size]);
void init_random_step(int init_i, int init_j, int board[][Board_size], int flag, int cnt);
void player_input(int b[][Board_size], int flag,ExMessage msg,int g_x,int g_y);
void determine(int b[][Board_size], int flag, const int cnt);//core
int  check_player(int b[][Board_size], int flag);//return 0:continue  1:player win
int  check_computer(int b[][Board_size], int flag);//return 0:continue  1:computer win
int check_full(int b[][Board_size]);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	srand((unsigned int)time(NULL));
	char quit = 0;
	initgraph(15 * 40, 15 * 40);
	while (!quit) {
		ExMessage msg;
		int g_x = 0;
		int g_y = 0;
		setbkcolor(BROWN);
		setlinecolor(BLACK);
		setbkmode(TRANSPARENT);
		setfillcolor(BLUE);
		settextcolor(YELLOW);
		settextstyle(30, 0, L"楷体");
		int height1 = textheight(L"先手");
		int width1 = textwidth(L"先手");

		BeginBatchDraw();
		cleardevice();
		for (int i = 0; i < 15; i++) {
			line(20 + 40 * i, 20, 20 + 40 * i, 580);
			line(20, 20 + 40 * i, 580, 20 + 40 * i);
		}
		fillroundrect(150, 250, 250, 310, 20, 20);
		outtextxy(150+50-width1/2,250+30-height1/2,L"先手");

		fillroundrect(350, 250, 450, 310, 20, 20);
		outtextxy(350+50-width1/2, 250 + 30 - height1 / 2, L"后手");

		EndBatchDraw();
		settextcolor(RED);
		settextstyle(60,0, L"楷体");
		int height2 = textheight(L"玩家胜利 五子连珠");
		int width2 = textwidth(L"玩家胜利 五子连珠");
		int text_x = 300 - width2 / 2;
		int text_y = 300 - height2 / 2;
		//printf("1: 先手  2:后手\n");
		int input2 = 0;
		//scanf("%d", &input2);
		while (input2 == 0) {
			if (peekmessage(&msg,EX_MOUSE)) {
				if (msg.message == WM_LBUTTONDOWN) {
					if (msg.x>=150 && msg.x<=250 && msg.y>=250 &&msg.y<=310) {
						input2 = 1;
						break;
					}
					else if (msg.x >= 350 && msg.x <=450 && msg.y >= 250 && msg.y <= 310) {
						input2 = 2;
						break;
					}
				}
			}
		}
		int board[Board_size][Board_size];
		memset(board, 0, sizeof(int) * Board_size * Board_size);
		//0:empty 1:black 2:white
		print_board(board);
		int init_i = 7;
		int init_j = 7;
		if (input2 == 1) {
			setfillcolor(BLACK);
			while (1) {
				if (peekmessage(&msg, EX_MOUSE)) {
					if (msg.message == WM_LBUTTONDOWN) {
						init_i = msg.y/40;
						init_j = msg.x/40;
						break;
					}
					else{
						int t_gx = g_x;
						int t_gy = g_y;
						g_x = msg.x / 40;
						g_y = msg.y / 40;
						if(g_x!=t_gx || g_y != t_gy)
						{
							BeginBatchDraw();
							print_board(board);
							solidcircle(20 + 40 * (msg.x / 40), 20 + 40 * (msg.y / 40), 3);
							EndBatchDraw();
						}
					}
				}
			}
			board[init_i][init_j] = 1;

			print_board(board);
		}
		init_random_step(init_i, init_j, board, input2, 0);
		print_board(board);

		player_input(board, input2,msg,g_x,g_y);
		//print_board(board);
		determine(board, input2, 0);
		print_board(board);

		int gameover = 0;
		int cnt = 0;
		while (!gameover) {
			player_input(board, input2,msg,g_x,g_y);
			print_board(board);
			if (check_player(board, input2)) {
				outtextxy(text_x, text_y,L"玩家胜利 五子连珠");
				gameover = 1;
				break;
			}
			if (check_full(board)) {
				outtextxy(text_x, text_y, L"双方平局 胜负难分");
				gameover = 1;
				break;
			}
			determine(board, input2, cnt);
			Sleep(sleep_ms);
			print_board(board);
			if (check_computer(board, input2)) {
				outtextxy(text_x, text_y, L"电脑胜利 五子连珠");
				gameover = 1;
				break;
			}
			if (check_full(board)) {
				outtextxy(text_x, text_y, L"双方平局 胜负难分");
				gameover = 1;
				break;
			}
			cnt++;
		}
		while (1) {
			if (peekmessage(&msg, EX_MOUSE)) {
				if (msg.message == WM_LBUTTONDOWN) {
					break;
				}
			}
		}
	}
	_getch();
	closegraph();
	return 0;
}

void print_board(int b[][Board_size]) {
	BeginBatchDraw();
	cleardevice();
	for (int i = 0; i < 15; i++) {
		line(20 + 40 * i, 20, 20 + 40 * i, 580);
		line(20, 20 + 40 * i, 580, 20 + 40 * i);
	}
	for (int i = 0; i < 15;i++) {
		for (int j = 0; j < 15;j++) {
			if (b[i][j]!=0) {
				switch (b[i][j]) {
				case 1:
					setfillcolor(BLACK);
					solidcircle(20+j*40,20+i*40,20-3);
					break;
				case 2:
					setfillcolor(WHITE);
					solidcircle(20 + j * 40, 20 + i * 40, 20 - 3);
					break;
				}
			}
		}
	}
	EndBatchDraw();
}

void init_random_step(int init_i, int init_j, int board[][Board_size], int flag, int cnt) {
	int final_i;
	int final_j;
	int base = 3;
	int bios = (int)((ceil(sqrt(cnt * 2 + 2)) > base) ? ceil(sqrt(cnt * 2 + 2)) : base);
	if ((bios & 0x1) == 0) bios += 1;
	bios = (bios > 15) ? 15 : bios;
	do {
		int random_i = rand() % bios;
		int random_j = rand() % bios;
		final_i = init_i + random_i - (bios - 1) / 2;
		final_j = init_j + random_j - (bios - 1) / 2;
	} while (!(final_i >= 0 && final_i <= 14 && final_j >= 0 && final_j <= 14 && board[final_i][final_j] == 0));
	board[final_i][final_j] = (flag - 1) ? 1 : 2;
}

void player_input(int b[][Board_size], int flag,ExMessage msg,int g_x,int g_y) {
	int i, j;
	do {
		if (peekmessage(&msg, EX_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN) {
				i = msg.y / 40;
				j = msg.x / 40;
				if (b[i][j] == 0) {
					b[i][j] = (flag - 1) ? 2 : 1;
					break;
				}
			}
			else {
				int t_gx = g_x;
				int t_gy = g_y;
				g_x = msg.x / 40;
				g_y = msg.y / 40;
				if ((g_x != t_gx || g_y != t_gy) && b[g_y][g_x]==0)
				{
					BeginBatchDraw();
					print_board(b);
					setfillcolor(BLACK);
					solidcircle(20 + 40 * (msg.x / 40), 20 + 40 * (msg.y / 40), 3);
					EndBatchDraw();
				}
				else if (g_x != t_gx || g_y != t_gy) {
					print_board(b);
				}
			}
		}
	} while (1);

}

void determine(int b[][Board_size], int flag, int cnt) {
	int i, j;
	int computer = (flag == 2) ? 1 : 2;
	int player = (flag == 2) ? 2 : 1;

	//1 检查自己是否有机会5子连珠 
	for (i = 0; i < Board_size; i++) {
		int isOver = 0;
		for (j = 0; j < Board_size; j++)
		{
			if (b[i][j] == computer) {
				//core
				//up 
				if (i - 4 >= 0) {
					if (((b[i - 1][j] == computer) + (b[i - 2][j] == computer) + (b[i - 3][j] == computer) + (b[i - 4][j] == computer)) == 3 && (b[i - 1][j] != player) && (b[i - 2][j] != player) && (b[i - 3][j] != player) && (b[i - 4][j] != player)) {
						b[i - 1][j] = computer; b[i - 2][j] = computer; b[i - 3][j] = computer; b[i - 4][j] = computer;
						isOver = 1;
						break;
					}
				}
				//down
				if (i + 4 <= Board_size - 1) {
					if (((b[i + 1][j] == computer) + (b[i + 2][j] == computer) + (b[i + 3][j] == computer) + (b[i + 4][j] == computer)) == 3 && (b[i + 1][j] != player) && (b[i + 2][j] != player) && (b[i + 3][j] != player) && (b[i + 4][j] != player)) {
						b[i + 1][j] = computer; b[i + 2][j] = computer; b[i + 3][j] = computer; b[i + 4][j] = computer;
						isOver = 1;
						break;
					}
				}
				//left
				if (j - 4 >= 0) {
					if (((b[i][j - 1] == computer) + (b[i][j - 2] == computer) + (b[i][j - 3] == computer) + (b[i][j - 4] == computer)) == 3 && (b[i][j - 1] != player) && (b[i][j - 2] != player) && (b[i][j - 3] != player) && (b[i][j - 4] != player)) {
						b[i][j - 1] = computer; b[i][j - 2] = computer; b[i][j - 3] = computer; b[i][j - 4] = computer;
						isOver = 1;
						break;
					}
				}
				//right
				if (j + 4 <= Board_size - 1) {
					if (((b[i][j + 1] == computer) + (b[i][j + 2] == computer) + (b[i][j + 3] == computer) + (b[i][j + 4] == computer)) == 3 && (b[i][j + 1] != player) && (b[i][j + 2] != player) && (b[i][j + 3] != player) && (b[i][j + 4] != player)) {
						b[i][j + 1] = computer; b[i][j + 2] = computer; b[i][j + 3] = computer; b[i][j + 4] = computer;
						isOver = 1;
						break;
					}
				}
				//左上 
				if (i - 4 >= 0 && j - 4 >= 0) {
					if (((b[i - 1][j - 1] == computer) + (b[i - 2][j - 2] == computer) + (b[i - 3][j - 3] == computer) + (b[i - 4][j - 4] == computer)) == 3 && (b[i - 1][j - 1] != player) && (b[i - 2][j - 2] != player) && (b[i - 3][j - 3] != player) && (b[i - 4][j - 4] != player)) {
						b[i - 1][j - 1] = computer; b[i - 2][j - 2] = computer; b[i - 3][j - 3] = computer; b[i - 4][j - 4] = computer;
						isOver = 1;
						break;
					}
				}
				//左下 
				if (i + 4 <= Board_size - 1 && j - 4 >= 0) {
					if (((b[i + 1][j - 1] == computer) + (b[i + 2][j - 2] == computer) + (b[i + 3][j - 3] == computer) + (b[i + 4][j - 4] == computer)) == 3 && (b[i + 1][j - 1] != player) && (b[i + 2][j - 2] != player) && (b[i + 3][j - 3] != player) && (b[i + 4][j - 4] != player)) {
						b[i + 1][j - 1] = computer; b[i + 2][j - 2] = computer; b[i + 3][j - 3] = computer; b[i + 4][j - 4] = computer;
						isOver = 1;
						break;
					}
				}
				//右上 
				if (i - 4 >= 0 && j + 4 <= Board_size - 1) {
					if (((b[i - 1][j + 1] == computer) + (b[i - 2][j + 2] == computer) + (b[i - 3][j + 3] == computer) + (b[i - 4][j + 4] == computer)) == 3 && (b[i - 1][j + 1] != player) && (b[i - 2][j + 2] != player) && (b[i - 3][j + 3] != player) && (b[i - 4][j + 4] != player)) {
						b[i - 1][j + 1] = computer; b[i - 2][j + 2] = computer; b[i - 3][j + 3] = computer; b[i - 4][j + 4] = computer;
						isOver = 1;
						break;
					}
				}
				//右下 
				if (i + 4 <= Board_size - 1 && j + 4 <= Board_size - 1) {
					if (((b[i + 1][j + 1] == computer) + (b[i + 2][j + 2] == computer) + (b[i + 3][j + 3] == computer) + (b[i + 4][j + 4] == computer)) == 3 && (b[i + 1][j + 1] != player) && (b[i + 2][j + 2] != player) && (b[i + 3][j + 3] != player) && (b[i + 4][j + 4] != player)) {
						b[i + 1][j + 1] = computer; b[i + 2][j + 2] = computer; b[i + 3][j + 3] = computer; b[i + 4][j + 4] = computer;
						isOver = 1;
						break;
					}
				}
			}
		}
		if (isOver == 1) {
			return;
		}
	}

	//2 检查player是否有机会5子连珠
	for (i = 0; i < Board_size; i++) {
		int isOver = 0;
		for (j = 0; j < Board_size; j++)
		{
			if (b[i][j] == player) {
				//core
				//up 
				if (i - 4 >= 0) {
					if (((b[i - 1][j] == player) + (b[i - 2][j] == player) + (b[i - 3][j] == player) + (b[i - 4][j] == player)) == 3 && (b[i - 1][j] != computer) && (b[i - 2][j] != computer) && (b[i - 3][j] != computer) && (b[i - 4][j] != computer)) {
						if (b[i - 1][j] == 0) b[i - 1][j] = computer;
						else if (b[i - 2][j] == 0) b[i - 2][j] = computer;
						else if (b[i - 3][j] == 0) b[i - 3][j] = computer;
						else b[i - 4][j] = computer;
						isOver = 1;
						break;
					}
				}
				//down
				if (i + 4 <= Board_size - 1) {
					if (((b[i + 1][j] == player) + (b[i + 2][j] == player) + (b[i + 3][j] == player) + (b[i + 4][j] == player)) == 3 && (b[i + 1][j] != computer) && (b[i + 2][j] != computer) && (b[i + 3][j] != computer) && (b[i + 4][j] != computer)) {
						if (b[i + 1][j] == 0) b[i + 1][j] = computer;
						else if (b[i + 2][j] == 0) b[i + 2][j] = computer;
						else if (b[i + 3][j] == 0) b[i + 3][j] = computer;
						else b[i + 4][j] = computer;
						isOver = 1;
						break;
					}
				}
				//left
				if (j - 4 >= 0) {
					if (((b[i][j - 1] == player) + (b[i][j - 2] == player) + (b[i][j - 3] == player) + (b[i][j - 4] == player)) == 3 && (b[i][j - 1] != computer) && (b[i][j - 2] != computer) && (b[i][j - 3] != computer) && (b[i][j - 4] != computer)) {
						if (b[i][j - 1] == 0) b[i][j - 1] = computer;
						else if (b[i][j - 2] == 0) b[i][j - 2] = computer;
						else if (b[i][j - 3] == 0) b[i][j - 3] = computer;
						else b[i][j - 4] = computer;
						isOver = 1;
						break;
					}
				}
				//right
				if (j + 4 <= Board_size - 1) {
					if (((b[i][j + 1] == player) + (b[i][j + 2] == player) + (b[i][j + 3] == player) + (b[i][j + 4] == player)) == 3 && (b[i][j + 1] != computer) && (b[i][j + 2] != computer) && (b[i][j + 3] != computer) && (b[i][j + 4] != computer)) {
						if (b[i][j + 1] == 0) b[i][j + 1] = computer;
						else if (b[i][j + 2] == 0) b[i][j + 2] = computer;
						else if (b[i][j + 3] == 0) b[i][j + 3] = computer;
						else b[i][j + 4] = computer;
						isOver = 1;
						break;
					}
				}
				//左上 
				if (i - 4 >= 0 && j - 4 >= 0) {
					if (((b[i - 1][j - 1] == player) + (b[i - 2][j - 2] == player) + (b[i - 3][j - 3] == player) + (b[i - 4][j - 4] == player)) == 3 && (b[i - 1][j - 1] != computer) && (b[i - 2][j - 2] != computer) && (b[i - 3][j - 3] != computer) && (b[i - 4][j - 4] != computer)) {
						if (b[i - 1][j - 1] == 0) b[i - 1][j - 1] = computer;
						else if (b[i - 2][j - 2] == 0) b[i - 2][j - 2] = computer;
						else if (b[i - 3][j - 3] == 0) b[i - 3][j - 3] = computer;
						else b[i - 4][j - 4] = computer;
						isOver = 1;
						break;
					}
				}
				//左下 
				if (i + 4 <= Board_size - 1 && j - 4 >= 0) {
					if (((b[i + 1][j - 1] == player) + (b[i + 2][j - 2] == player) + (b[i + 3][j - 3] == player) + (b[i + 4][j - 4] == player)) == 3 && (b[i + 1][j - 1] != computer) && (b[i + 2][j - 2] != computer) && (b[i + 3][j - 3] != computer) && (b[i + 4][j - 4] != computer)) {
						if (b[i + 1][j - 1] == 0) b[i + 1][j - 1] = computer;
						else if (b[i + 2][j - 2] == 0) b[i + 2][j - 2] = computer;
						else if (b[i + 3][j - 3] == 0) b[i + 3][j - 3] = computer;
						else b[i + 4][j - 4] = computer;
						isOver = 1;
						break;
					}
				}
				//右上 
				if (i - 4 >= 0 && j + 4 <= Board_size - 1) {
					if (((b[i - 1][j + 1] == player) + (b[i - 2][j + 2] == player) + (b[i - 3][j + 3] == player) + (b[i - 4][j + 4] == player)) == 3 && (b[i - 1][j + 1] != computer) && (b[i - 2][j + 2] != computer) && (b[i - 3][j + 3] != computer) && (b[i - 4][j + 4] != computer)) {
						if (b[i - 1][j + 1] == 0) b[i - 1][j + 1] = computer;
						else if (b[i - 2][j + 2] == 0) b[i - 2][j + 2] = computer;
						else if (b[i - 3][j + 3] == 0) b[i - 3][j + 3] = computer;
						else b[i - 4][j + 4] = computer;
						isOver = 1;
						break;
					}
				}
				//右下 
				if (i + 4 <= Board_size - 1 && j + 4 <= Board_size - 1) {
					if (((b[i + 1][j + 1] == player) + (b[i + 2][j + 2] == player) + (b[i + 3][j + 3] == player) + (b[i + 4][j + 4] == player)) == 3 && (b[i + 1][j + 1] != computer) && (b[i + 2][j + 2] != computer) && (b[i + 3][j + 3] != computer) && (b[i + 4][j + 4] != computer)) {
						if (b[i + 1][j + 1] == 0) b[i + 1][j + 1] = computer;
						else if (b[i + 2][j + 2] == 0) b[i + 2][j + 2] = computer;
						else if (b[i + 3][j + 3] == 0) b[i + 3][j + 3] = computer;
						else b[i + 4][j + 4] = computer;
						isOver = 1;
						break;
					}
				}
			}
		}
		if (isOver == 1) {
			return;
		}
	}

	//3 检查自己   _OOO__    _OO_O_    _O_OO_的情况
	for (i = 0; i < Board_size; i++) {
		int isOver = 0;
		for (j = 0; j < Board_size; j++)
		{
			if (b[i][j] == 0) {
				//core
				//up 
				if (i - 5 >= 0) {
					if (((b[i - 1][j] == computer) + (b[i - 2][j] == computer) + (b[i - 3][j] == computer) + (b[i - 4][j] == computer)) == 3 && (b[i - 1][j] != player) && (b[i - 2][j] != player) && (b[i - 3][j] != player) && (b[i - 4][j] != player) && b[i - 5][j] == 0) {
						if (b[i - 1][j] == 0) b[i - 1][j] = computer;
						else if (b[i - 2][j] == 0) b[i - 2][j] = computer;
						else if (b[i - 3][j] == 0) b[i - 3][j] = computer;
						else b[i - 4][j] = computer;
						isOver = 1;
						break;
					}
				}
				//down
				if (i + 5 <= Board_size - 1) {
					if (((b[i + 1][j] == computer) + (b[i + 2][j] == computer) + (b[i + 3][j] == computer) + (b[i + 4][j] == computer)) == 3 && (b[i + 1][j] != player) && (b[i + 2][j] != player) && (b[i + 3][j] != player) && (b[i + 4][j] != player) && b[i + 5][j] == 0) {
						if (b[i + 1][j] == 0) b[i + 1][j] = computer;
						else if (b[i + 2][j] == 0) b[i + 2][j] = computer;
						else if (b[i + 3][j] == 0) b[i + 3][j] = computer;
						else b[i + 4][j] = computer;
						isOver = 1;
						break;
					}
				}
				//left
				if (j - 5 >= 0) {
					if (((b[i][j - 1] == computer) + (b[i][j - 2] == computer) + (b[i][j - 3] == computer) + (b[i][j - 4] == computer)) == 3 && (b[i][j - 1] != player) && (b[i][j - 2] != player) && (b[i][j - 3] != player) && (b[i][j - 4] != player) && b[i][j - 5] == 0) {
						if (b[i][j - 1] == 0) b[i][j - 1] = computer;
						else if (b[i][j - 2] == 0) b[i][j - 2] = computer;
						else if (b[i][j - 3] == 0) b[i][j - 3] = computer;
						else b[i][j - 4] = computer;
						isOver = 1;
						break;
					}
				}
				//right
				if (j + 5 <= Board_size - 1) {
					if (((b[i][j + 1] == computer) + (b[i][j + 2] == computer) + (b[i][j + 3] == computer) + (b[i][j + 4] == computer)) == 3 && (b[i][j + 1] != player) && (b[i][j + 2] != player) && (b[i][j + 3] != player) && (b[i][j + 4] != player) && b[i][j + 5] == 0) {
						if (b[i][j + 1] == 0) b[i][j + 1] = computer;
						else if (b[i][j + 2] == 0) b[i][j + 2] = computer;
						else if (b[i][j + 3] == 0) b[i][j + 3] = computer;
						else b[i][j + 4] = computer;
						isOver = 1;
						break;
					}
				}
				//左上 
				if (i - 5 >= 0 && j - 5 >= 0) {
					if (((b[i - 1][j - 1] == computer) + (b[i - 2][j - 2] == computer) + (b[i - 3][j - 3] == computer) + (b[i - 4][j - 4] == computer)) == 3 && (b[i - 1][j - 1] != player) && (b[i - 2][j - 2] != player) && (b[i - 3][j - 3] != player) && (b[i - 4][j - 4] != player) && b[i - 5][j - 5] == 0) {
						if (b[i - 1][j - 1] == 0) b[i - 1][j - 1] = computer;
						else if (b[i - 2][j - 2] == 0) b[i - 2][j - 2] = computer;
						else if (b[i - 3][j - 3] == 0) b[i - 3][j - 3] = computer;
						else b[i - 4][j - 4] = computer;
						isOver = 1;
						break;
					}
				}
				//左下 
				if (i + 5 <= Board_size - 1 && j - 5 >= 0) {
					if (((b[i + 1][j - 1] == computer) + (b[i + 2][j - 2] == computer) + (b[i + 3][j - 3] == computer) + (b[i + 4][j - 4] == computer)) == 3 && (b[i + 1][j - 1] != player) && (b[i + 2][j - 2] != player) && (b[i + 3][j - 3] != player) && (b[i + 4][j - 4] != player) && b[i + 5][j - 5] == 0) {
						if (b[i + 1][j - 1] == 0) b[i + 1][j - 1] = computer;
						else if (b[i + 2][j - 2] == 0) b[i + 2][j - 2] = computer;
						else if (b[i + 3][j - 3] == 0) b[i + 3][j - 3] = computer;
						else b[i + 4][j - 4] = computer;
						isOver = 1;
						break;
					}
				}
				//右上 
				if (i - 5 >= 0 && j + 5 <= Board_size - 1) {
					if (((b[i - 1][j + 1] == computer) + (b[i - 2][j + 2] == computer) + (b[i - 3][j + 3] == computer) + (b[i - 4][j + 4] == computer)) == 3 && (b[i - 1][j + 1] != player) && (b[i - 2][j + 2] != player) && (b[i - 3][j + 3] != player) && (b[i - 4][j + 4] != player) && b[i - 5][j + 5] == 0) {
						if (b[i - 1][j + 1] == 0) b[i - 1][j + 1] = computer;
						else if (b[i - 2][j + 2] == 0) b[i - 2][j + 2] = computer;
						else if (b[i - 3][j + 3] == 0) b[i - 3][j + 3] = computer;
						else b[i - 4][j + 4] = computer;
						isOver = 1;
						break;
					}
				}
				//右下 
				if (i + 5 <= Board_size - 1 && j + 5 <= Board_size - 1) {
					if (((b[i + 1][j + 1] == computer) + (b[i + 2][j + 2] == computer) + (b[i + 3][j + 3] == computer) + (b[i + 4][j + 4] == computer)) == 3 && (b[i + 1][j + 1] != player) && (b[i + 2][j + 2] != player) && (b[i + 3][j + 3] != player) && (b[i + 4][j + 4] != player) && b[i + 5][j + 5] == 0) {
						if (b[i + 1][j + 1] == 0) b[i + 1][j + 1] = computer;
						else if (b[i + 2][j + 2] == 0) b[i + 2][j + 2] = computer;
						else if (b[i + 3][j + 3] == 0) b[i + 3][j + 3] = computer;
						else b[i + 4][j + 4] = computer;
						isOver = 1;
						break;
					}
				}
			}
		}
		if (isOver == 1) {
			return;
		}
	}

	//4 检查player _OOO__    _OO_O_    _O_OO_的情况
	for (i = 0; i < Board_size; i++) {
		int isOver = 0;
		for (j = 0; j < Board_size; j++)
		{
			if (b[i][j] == 0) {
				//core
				//up 
				if (i - 5 >= 0) {
					if (((b[i - 1][j] == player) + (b[i - 2][j] == player) + (b[i - 3][j] == player) + (b[i - 4][j] == player)) == 3 && (b[i - 1][j] != computer) && (b[i - 2][j] != computer) && (b[i - 3][j] != computer) && (b[i - 4][j] != computer) && b[i - 5][j] == 0) {
						if (b[i - 1][j] == 0) b[i - 1][j] = computer;
						else if (b[i - 2][j] == 0) b[i - 2][j] = computer;
						else if (b[i - 3][j] == 0) b[i - 3][j] = computer;
						else b[i - 4][j] = computer;
						isOver = 1;
						break;
					}
				}
				//down
				if (i + 5 <= Board_size - 1) {
					if (((b[i + 1][j] == player) + (b[i + 2][j] == player) + (b[i + 3][j] == player) + (b[i + 4][j] == player)) == 3 && (b[i + 1][j] != computer) && (b[i + 2][j] != computer) && (b[i + 3][j] != computer) && (b[i + 4][j] != computer) && b[i + 5][j] == 0) {
						if (b[i + 1][j] == 0) b[i + 1][j] = computer;
						else if (b[i + 2][j] == 0) b[i + 2][j] = computer;
						else if (b[i + 3][j] == 0) b[i + 3][j] = computer;
						else b[i + 4][j] = computer;
						isOver = 1;
						break;
					}
				}
				//left
				if (j - 5 >= 0) {
					if (((b[i][j - 1] == player) + (b[i][j - 2] == player) + (b[i][j - 3] == player) + (b[i][j - 4] == player)) == 3 && (b[i][j - 1] != computer) && (b[i][j - 2] != computer) && (b[i][j - 3] != computer) && (b[i][j - 4] != computer) && b[i][j - 5] == 0) {
						if (b[i][j - 1] == 0) b[i][j - 1] = computer;
						else if (b[i][j - 2] == 0) b[i][j - 2] = computer;
						else if (b[i][j - 3] == 0) b[i][j - 3] = computer;
						else b[i][j - 4] = computer;
						isOver = 1;
						break;
					}
				}
				//right
				if (j + 5 <= Board_size - 1) {
					if (((b[i][j + 1] == player) + (b[i][j + 2] == player) + (b[i][j + 3] == player) + (b[i][j + 4] == player)) == 3 && (b[i][j + 1] != computer) && (b[i][j + 2] != computer) && (b[i][j + 3] != computer) && (b[i][j + 4] != computer) && b[i][j + 5] == 0) {
						if (b[i][j + 1] == 0) b[i][j + 1] = computer;
						else if (b[i][j + 2] == 0) b[i][j + 2] = computer;
						else if (b[i][j + 3] == 0) b[i][j + 3] = computer;
						else b[i][j + 4] = computer;
						isOver = 1;
						break;
					}
				}
				//左上 
				if (i - 5 >= 0 && j - 5 >= 0) {
					if (((b[i - 1][j - 1] == player) + (b[i - 2][j - 2] == player) + (b[i - 3][j - 3] == player) + (b[i - 4][j - 4] == player)) == 3 && (b[i - 1][j - 1] != computer) && (b[i - 2][j - 2] != computer) && (b[i - 3][j - 3] != computer) && (b[i - 4][j - 4] != computer) && b[i - 5][j - 5] == 0) {
						if (b[i - 1][j - 1] == 0) b[i - 1][j - 1] = computer;
						else if (b[i - 2][j - 2] == 0) b[i - 2][j - 2] = computer;
						else if (b[i - 3][j - 3] == 0) b[i - 3][j - 3] = computer;
						else b[i - 4][j - 4] = computer;
						isOver = 1;
						break;
					}
				}
				//左下 
				if (i + 5 <= Board_size - 1 && j - 5 >= 0) {
					if (((b[i + 1][j - 1] == player) + (b[i + 2][j - 2] == player) + (b[i + 3][j - 3] == player) + (b[i + 4][j - 4] == player)) == 3 && (b[i + 1][j - 1] != computer) && (b[i + 2][j - 2] != computer) && (b[i + 3][j - 3] != computer) && (b[i + 4][j - 4] != computer) && b[i + 5][j - 5] == 0) {
						if (b[i + 1][j - 1] == 0) b[i + 1][j - 1] = computer;
						else if (b[i + 2][j - 2] == 0) b[i + 2][j - 2] = computer;
						else if (b[i + 3][j - 3] == 0) b[i + 3][j - 3] = computer;
						else b[i + 4][j - 4] = computer;
						isOver = 1;
						break;
					}
				}
				//右上 
				if (i - 5 >= 0 && j + 5 <= Board_size - 1) {
					if (((b[i - 1][j + 1] == player) + (b[i - 2][j + 2] == player) + (b[i - 3][j + 3] == player) + (b[i - 4][j + 4] == player)) == 3 && (b[i - 1][j + 1] != computer) && (b[i - 2][j + 2] != computer) && (b[i - 3][j + 3] != computer) && (b[i - 4][j + 4] != computer) && b[i - 5][j + 5] == 0) {
						if (b[i - 1][j + 1] == 0) b[i - 1][j + 1] = computer;
						else if (b[i - 2][j + 2] == 0) b[i - 2][j + 2] = computer;
						else if (b[i - 3][j + 3] == 0) b[i - 3][j + 3] = computer;
						else b[i - 4][j + 4] = computer;
						isOver = 1;
						break;
					}
				}
				//右下 
				if (i + 5 <= Board_size - 1 && j + 5 <= Board_size - 1) {
					if (((b[i + 1][j + 1] == player) + (b[i + 2][j + 2] == player) + (b[i + 3][j + 3] == player) + (b[i + 4][j + 4] == player)) == 3 && (b[i + 1][j + 1] != computer) && (b[i + 2][j + 2] != computer) && (b[i + 3][j + 3] != computer) && (b[i + 4][j + 4] != computer) && b[i + 5][j + 5] == 0) {
						if (b[i + 1][j + 1] == 0) b[i + 1][j + 1] = computer;
						else if (b[i + 2][j + 2] == 0) b[i + 2][j + 2] = computer;
						else if (b[i + 3][j + 3] == 0) b[i + 3][j + 3] = computer;
						else b[i + 4][j + 4] = computer;
						isOver = 1;
						break;
					}
				}
			}
		}
		if (isOver == 1) {
			return;
		}
	}

	//5 检查player       _
	//                 _ ! O O _ 的情况
	//                   _
	//                   O
	//                   O
	//                   _
	for (i = 1; i < Board_size - 1; i++) {
		int count_danger = 0;
		for (j = 1; j < Board_size - 1; j++)
		{
			if (b[i][j] == 0) {
				count_danger = 0;
				//core

				//same side ********************************

				//up 
				if (i - 4 >= 0) {
					if (((b[i - 1][j] == player) + (b[i - 2][j] == player) + (b[i - 3][j] == player)) == 2 && (b[i - 1][j] != computer) && (b[i - 2][j] != computer) && (b[i - 3][j] != computer) && b[i - 4][j] == 0 && b[i + 1][j] == 0) {
						count_danger++;
					}
				}
				//down
				if (i + 4 <= Board_size - 1) {
					if (((b[i + 1][j] == player) + (b[i + 2][j] == player) + (b[i + 3][j] == player)) == 2 && (b[i + 1][j] != computer) && (b[i + 2][j] != computer) && (b[i + 3][j] != computer) && b[i + 4][j] == 0 && b[i - 1][j] == 0) {
						count_danger++;
					}
				}
				//left
				if (j - 4 >= 0) {
					if (((b[i][j - 1] == player) + (b[i][j - 2] == player) + (b[i][j - 3] == player)) == 2 && (b[i][j - 1] != computer) && (b[i][j - 2] != computer) && (b[i][j - 3] != computer) && b[i][j - 4] == 0 && b[i][j + 1] == 0) {
						count_danger++;
					}
				}
				//right
				if (j + 4 <= Board_size - 1) {
					if (((b[i][j + 1] == player) + (b[i][j + 2] == player) + (b[i][j + 3] == player)) == 2 && (b[i][j + 1] != computer) && (b[i][j + 2] != computer) && (b[i][j + 3] != computer) && b[i][j + 4] == 0 && b[i][j - 1] == 0) {
						count_danger++;
					}
				}
				//左上 
				if (i - 4 >= 0 && j - 4 >= 0) {
					if (((b[i - 1][j - 1] == player) + (b[i - 2][j - 2] == player) + (b[i - 3][j - 3] == player)) == 2 && (b[i - 1][j - 1] != computer) && (b[i - 2][j - 2] != computer) && (b[i - 3][j - 3] != computer) && b[i - 4][j - 4] == 0 && b[i + 1][j + 1] == 0) {
						count_danger++;
					}
				}
				//左下 
				if (i + 4 <= Board_size - 1 && j - 4 >= 0) {
					if (((b[i + 1][j - 1] == player) + (b[i + 2][j - 2] == player) + (b[i + 3][j - 3] == player)) == 2 && (b[i + 1][j - 1] != computer) && (b[i + 2][j - 2] != computer) && (b[i + 3][j - 3] != computer) && b[i + 4][j - 4] == 0 && b[i - 1][j + 1] == 0) {
						count_danger++;
					}
				}
				//右上 
				if (i - 4 >= 0 && j + 4 <= Board_size - 1) {
					if (((b[i - 1][j + 1] == player) + (b[i - 2][j + 2] == player) + (b[i - 3][j + 3] == player)) == 2 && (b[i - 1][j + 1] != computer) && (b[i - 2][j + 2] != computer) && (b[i - 3][j + 3] != computer) && b[i - 4][j + 4] == 0 && b[i + 1][j - 1] == 0) {
						count_danger++;
					}
				}
				//右下 
				if (i + 4 <= Board_size - 1 && j + 4 <= Board_size - 1) {
					if (((b[i + 1][j + 1] == player) + (b[i + 2][j + 2] == player) + (b[i + 3][j + 3] == player)) == 2 && (b[i + 1][j + 1] != computer) && (b[i + 2][j + 2] != computer) && (b[i + 3][j + 3] != computer) && b[i + 4][j + 4] == 0 && b[i - 1][j - 1] == 0) {
						count_danger++;
					}
				}

				//different side ********************************

				//up 
				if (i - 3 >= 0 && i + 2 <= Board_size - 1) {
					if (b[i - 1][j] == 0 && (b[i - 2][j] == player) && (b[i - 3][j] == 0) && b[i + 1][j] == player && b[i + 2][j] == 0) {
						count_danger++;
					}
				}
				//down
				if (i + 3 <= Board_size - 1 && i - 2 >= 0) {
					if ((b[i + 1][j] == 0) && (b[i + 2][j] == player) && (b[i + 3][j] == 0) && b[i - 1][j] == player && b[i - 2][j] == 0) {
						count_danger++;
					}
				}
				//left
				if (j - 3 >= 0 && j + 2 <= Board_size - 1) {
					if ((b[i][j - 1] == 0) && (b[i][j - 2] == player) && (b[i][j - 3] == 0) && b[i][j + 1] == player && b[i][j + 2] == 0) {
						count_danger++;
					}
				}
				//right
				if (j + 3 <= Board_size - 1 && j - 2 >= 0) {
					if ((b[i][j + 1] == 0) && (b[i][j + 2] == player) && (b[i][j + 3] == 0) && b[i][j - 1] == player && b[i][j - 2] == 0) {
						count_danger++;
					}
				}
				//左上 
				if (i - 3 >= 0 && j - 3 >= 0 && i + 2 <= Board_size - 1 && j + 2 <= Board_size - 1) {
					if ((b[i - 1][j - 1] == 0) && (b[i - 2][j - 2] == player) && (b[i - 3][j - 3] == 0) && b[i + 1][j + 1] == player && b[i + 2][j + 2] == 0) {
						count_danger++;
					}
				}
				//左下 
				if (i + 3 <= Board_size - 1 && j - 3 >= 0 && i - 2 >= 0 && j + 2 <= Board_size - 1) {
					if ((b[i + 1][j - 1] == 0) && (b[i + 2][j - 2] == player) && (b[i + 3][j - 3] == 0) && b[i - 1][j + 1] == player && b[i - 2][j + 2] == 0) {
						count_danger++;
					}
				}
				//右上 
				if (i - 3 >= 0 && j + 3 <= Board_size - 1 && i + 2 <= Board_size - 1 && j - 2 >= 0) {
					if ((b[i - 1][j + 1] == 0) && (b[i - 2][j + 2] == player) && (b[i - 3][j + 3] == 0 && b[i + 1][j - 1] == player && b[i + 2][j - 2] == 0)) {
						count_danger++;
					}
				}
				//右下 
				if (i + 3 <= Board_size - 1 && j + 3 <= Board_size - 1 && i - 2 >= 0 && j - 2 >= 0) {
					if ((b[i + 1][j + 1] == 0) && (b[i + 2][j + 2] == player) && (b[i + 3][j + 3] == 0) && b[i - 1][j - 1] == player && b[i - 2][j - 2] == 0) {
						count_danger++;
					}
				}

				//different side ******************************** symmetry

				if (b[i - 1][j] == player && b[i + 1][j] == player && b[i - 2][j] == 0 && b[i + 2][j] == 0) {
					count_danger++;
				}
				if (b[i][j - 1] == player && b[i][j + 1] == player && b[i][j - 2] == 0 && b[i][j + 2] == 0) {
					count_danger++;
				}
				if (b[i - 1][j - 1] == player && b[i + 1][j + 1] == player && b[i - 2][j - 2] == 0 && b[i + 2][j + 2] == 0) {
					count_danger++;
				}
				if (b[i - 1][j + 1] == player && b[i + 1][j - 1] == player && b[i - 2][j + 2] == 0 && b[i + 2][j - 2] == 0) {
					count_danger++;
				}
				if (count_danger >= 2) {
					b[i][j] = computer;
					return;
				}
			}
		}
	}

	//6 检查自己         _
	//                 _ ! O O _ 的情况
	//                   _
	//                   O
	//                   O
	//                   _
	for (i = 1; i < Board_size - 1; i++) {
		int count_danger = 0;
		for (j = 1; j < Board_size - 1; j++)
		{
			if (b[i][j] == 0) {
				count_danger = 0;
				//core

				//same side ********************************

				//up 
				if (i - 4 >= 0) {
					if (((b[i - 1][j] == computer) + (b[i - 2][j] == computer) + (b[i - 3][j] == computer)) == 2 && (b[i - 1][j] != player) && (b[i - 2][j] != player) && (b[i - 3][j] != player) && b[i - 4][j] == 0 && b[i + 1][j] == 0) {
						count_danger++;
					}
				}
				//down
				if (i + 4 <= Board_size - 1) {
					if (((b[i + 1][j] == computer) + (b[i + 2][j] == computer) + (b[i + 3][j] == computer)) == 2 && (b[i + 1][j] != player) && (b[i + 2][j] != player) && (b[i + 3][j] != player) && b[i + 4][j] == 0 && b[i - 1][j] == 0) {
						count_danger++;
					}
				}
				//left
				if (j - 4 >= 0) {
					if (((b[i][j - 1] == computer) + (b[i][j - 2] == computer) + (b[i][j - 3] == computer)) == 2 && (b[i][j - 1] != player) && (b[i][j - 2] != player) && (b[i][j - 3] != player) && b[i][j - 4] == 0 && b[i][j + 1] == 0) {
						count_danger++;
					}
				}
				//right
				if (j + 4 <= Board_size - 1) {
					if (((b[i][j + 1] == computer) + (b[i][j + 2] == computer) + (b[i][j + 3] == computer)) == 2 && (b[i][j + 1] != player) && (b[i][j + 2] != player) && (b[i][j + 3] != player) && b[i][j + 4] == 0 && b[i][j - 1] == 0) {
						count_danger++;
					}
				}
				//左上 
				if (i - 4 >= 0 && j - 4 >= 0) {
					if (((b[i - 1][j - 1] == computer) + (b[i - 2][j - 2] == computer) + (b[i - 3][j - 3] == computer)) == 2 && (b[i - 1][j - 1] != player) && (b[i - 2][j - 2] != player) && (b[i - 3][j - 3] != player) && b[i - 4][j - 4] == 0 && b[i + 1][j + 1] == 0) {
						count_danger++;
					}
				}
				//左下 
				if (i + 4 <= Board_size - 1 && j - 4 >= 0) {
					if (((b[i + 1][j - 1] == computer) + (b[i + 2][j - 2] == computer) + (b[i + 3][j - 3] == computer)) == 2 && (b[i + 1][j - 1] != player) && (b[i + 2][j - 2] != player) && (b[i + 3][j - 3] != player) && b[i + 4][j - 4] == 0 && b[i - 1][j + 1] == 0) {
						count_danger++;
					}
				}
				//右上 
				if (i - 4 >= 0 && j + 4 <= Board_size - 1) {
					if (((b[i - 1][j + 1] == computer) + (b[i - 2][j + 2] == computer) + (b[i - 3][j + 3] == computer)) == 2 && (b[i - 1][j + 1] != player) && (b[i - 2][j + 2] != player) && (b[i - 3][j + 3] != player) && b[i - 4][j + 4] == 0 && b[i + 1][j - 1] == 0) {
						count_danger++;
					}
				}
				//右下 
				if (i + 4 <= Board_size - 1 && j + 4 <= Board_size - 1) {
					if (((b[i + 1][j + 1] == computer) + (b[i + 2][j + 2] == computer) + (b[i + 3][j + 3] == computer)) == 2 && (b[i + 1][j + 1] != player) && (b[i + 2][j + 2] != player) && (b[i + 3][j + 3] != player) && b[i + 4][j + 4] == 0 && b[i - 1][j - 1] == 0) {
						count_danger++;
					}
				}

				//different side ********************************

				//up 
				if (i - 3 >= 0 && i + 2 <= Board_size - 1) {
					if (b[i - 1][j] == 0 && (b[i - 2][j] == computer) && (b[i - 3][j] == 0) && b[i + 1][j] == computer && b[i + 2][j] == 0) {
						count_danger++;
					}
				}
				//down
				if (i + 3 <= Board_size - 1 && i - 2 >= 0) {
					if ((b[i + 1][j] == 0) && (b[i + 2][j] == computer) && (b[i + 3][j] == 0) && b[i - 1][j] == computer && b[i - 2][j] == 0) {
						count_danger++;
					}
				}
				//left
				if (j - 3 >= 0 && j + 2 <= Board_size - 1) {
					if ((b[i][j - 1] == 0) && (b[i][j - 2] == computer) && (b[i][j - 3] == 0) && b[i][j + 1] == computer && b[i][j + 2] == 0) {
						count_danger++;
					}
				}
				//right
				if (j + 3 <= Board_size - 1 && j - 2 >= 0) {
					if ((b[i][j + 1] == 0) && (b[i][j + 2] == computer) && (b[i][j + 3] == 0) && b[i][j - 1] == computer && b[i][j - 2] == 0) {
						count_danger++;
					}
				}
				//左上 
				if (i - 3 >= 0 && j - 3 >= 0 && i + 2 <= Board_size - 1 && j + 2 <= Board_size - 1) {
					if ((b[i - 1][j - 1] == 0) && (b[i - 2][j - 2] == computer) && (b[i - 3][j - 3] == 0) && b[i + 1][j + 1] == computer && b[i + 2][j + 2] == 0) {
						count_danger++;
					}
				}
				//左下 
				if (i + 3 <= Board_size - 1 && j - 3 >= 0 && i - 2 >= 0 && j + 2 <= Board_size - 1) {
					if ((b[i + 1][j - 1] == 0) && (b[i + 2][j - 2] == computer) && (b[i + 3][j - 3] == 0) && b[i - 1][j + 1] == computer && b[i - 2][j + 2] == 0) {
						count_danger++;
					}
				}
				//右上 
				if (i - 3 >= 0 && j + 3 <= Board_size - 1 && i + 2 <= Board_size - 1 && j - 2 >= 0) {
					if ((b[i - 1][j + 1] == 0) && (b[i - 2][j + 2] == computer) && (b[i - 3][j + 3] == 0 && b[i + 1][j - 1] == computer && b[i + 2][j - 2] == 0)) {
						count_danger++;
					}
				}
				//右下 
				if (i + 3 <= Board_size - 1 && j + 3 <= Board_size - 1 && i - 2 >= 0 && j - 2 >= 0) {
					if ((b[i + 1][j + 1] == 0) && (b[i + 2][j + 2] == computer) && (b[i + 3][j + 3] == 0) && b[i - 1][j - 1] == computer && b[i - 2][j - 2] == 0) {
						count_danger++;
					}
				}

				//different side ******************************** symmetry

				if (b[i - 1][j] == computer && b[i + 1][j] == computer && b[i - 2][j] == 0 && b[i + 2][j] == 0) {
					count_danger++;
				}
				if (b[i][j - 1] == computer && b[i][j + 1] == computer && b[i][j - 2] == 0 && b[i][j + 2] == 0) {
					count_danger++;
				}
				if (b[i - 1][j - 1] == computer && b[i + 1][j + 1] == computer && b[i - 2][j - 2] == 0 && b[i + 2][j + 2] == 0) {
					count_danger++;
				}
				if (b[i - 1][j + 1] == computer && b[i + 1][j - 1] == computer && b[i - 2][j + 2] == 0 && b[i + 2][j - 2] == 0) {
					count_danger++;
				}

				if (count_danger >= 2) {
					b[i][j] = computer;
					return;
				}
			}
		}
	}

	//7 检查player !OO_的情况
	//             _
	//             O
	//             O

	for (i = 0; i < Board_size; i++) {
		int count_danger = 0;
		for (j = 0; j < Board_size; j++)
		{
			if (b[i][j] == 0) {
				count_danger = 0;
				//core
				//up 
				if (i - 3 >= 0) {
					if (((b[i - 1][j] == player) + (b[i - 2][j] == player) + (b[i - 3][j] == player)) == 2 && (b[i - 1][j] != computer) && (b[i - 2][j] != computer) && (b[i - 3][j] != computer)) {
						count_danger++;
					}
				}
				//down
				if (i + 3 <= Board_size - 1) {
					if (((b[i + 1][j] == player) + (b[i + 2][j] == player) + (b[i + 3][j] == player)) == 2 && (b[i + 1][j] != computer) && (b[i + 2][j] != computer) && (b[i + 3][j] != computer)) {
						count_danger++;
					}
				}
				//left
				if (j - 3 >= 0) {
					if (((b[i][j - 1] == player) + (b[i][j - 2] == player) + (b[i][j - 3] == player)) == 2 && (b[i][j - 1] != computer) && (b[i][j - 2] != computer) && (b[i][j - 3] != computer)) {
						count_danger++;
					}
				}
				//right
				if (j + 3 <= Board_size - 1) {
					if (((b[i][j + 1] == player) + (b[i][j + 2] == player) + (b[i][j + 3] == player)) == 2 && (b[i][j + 1] != computer) && (b[i][j + 2] != computer) && (b[i][j + 3] != computer)) {
						count_danger++;
					}
				}
				//左上 
				if (i - 3 >= 0 && j - 3 >= 0) {
					if (((b[i - 1][j - 1] == player) + (b[i - 2][j - 2] == player) + (b[i - 3][j - 3] == player)) == 2 && (b[i - 1][j - 1] != computer) && (b[i - 2][j - 2] != computer) && (b[i - 3][j - 3] != computer)) {
						count_danger++;
					}
				}
				//左下 
				if (i + 3 <= Board_size - 1 && j - 3 >= 0) {
					if (((b[i + 1][j - 1] == player) + (b[i + 2][j - 2] == player) + (b[i + 3][j - 3] == player)) == 2 && (b[i + 1][j - 1] != computer) && (b[i + 2][j - 2] != computer) && (b[i + 3][j - 3] != computer)) {
						count_danger++;
					}
				}
				//右上 
				if (i - 3 >= 0 && j + 3 <= Board_size - 1) {
					if (((b[i - 1][j + 1] == player) + (b[i - 2][j + 2] == player) + (b[i - 3][j + 3] == player)) == 2 && (b[i - 1][j + 1] != computer) && (b[i - 2][j + 2] != computer) && (b[i - 3][j + 3] != computer)) {
						count_danger++;
					}
				}
				//右下 
				if (i + 3 <= Board_size - 1 && j + 3 <= Board_size - 1) {
					if (((b[i + 1][j + 1] == player) + (b[i + 2][j + 2] == player) + (b[i + 3][j + 3] == player)) == 2 && (b[i + 1][j + 1] != computer) && (b[i + 2][j + 2] != computer) && (b[i + 3][j + 3] != computer)) {
						count_danger++;
					}
				}
				if (count_danger >= 2) {
					b[i][j] = computer;
					return;
				}
			}
		}
	}

	//8 检查player OOO_  OO_O  O_OO的情况
	for (i = 0; i < Board_size; i++) {
		int isOver = 0;
		for (j = 0; j < Board_size; j++)
		{
			if (b[i][j] == player) {
				//core
				//up 
				if (i - 3 >= 0) {
					if (((b[i - 1][j] == player) + (b[i - 2][j] == player) + (b[i - 3][j] == player)) == 2 && (b[i - 1][j] != computer) && (b[i - 2][j] != computer) && (b[i - 3][j] != computer)) {
						if (b[i - 1][j] == 0) b[i - 1][j] = computer;
						else if (b[i - 2][j] == 0) b[i - 2][j] = computer;
						else b[i - 3][j] = computer;
						isOver = 1;
						break;
					}
				}
				//down
				if (i + 3 <= Board_size - 1) {
					if (((b[i + 1][j] == player) + (b[i + 2][j] == player) + (b[i + 3][j] == player)) == 2 && (b[i + 1][j] != computer) && (b[i + 2][j] != computer) && (b[i + 3][j] != computer)) {
						if (b[i + 1][j] == 0) b[i + 1][j] = computer;
						else if (b[i + 2][j] == 0) b[i + 2][j] = computer;
						else b[i + 3][j] = computer;
						isOver = 1;
						break;
					}
				}
				//left
				if (j - 3 >= 0) {
					if (((b[i][j - 1] == player) + (b[i][j - 2] == player) + (b[i][j - 3] == player)) == 2 && (b[i][j - 1] != computer) && (b[i][j - 2] != computer) && (b[i][j - 3] != computer)) {
						if (b[i][j - 1] == 0) b[i][j - 1] = computer;
						else if (b[i][j - 2] == 0) b[i][j - 2] = computer;
						else b[i][j - 3] = computer;
						isOver = 1;
						break;
					}
				}
				//right
				if (j + 3 <= Board_size - 1) {
					if (((b[i][j + 1] == player) + (b[i][j + 2] == player) + (b[i][j + 3] == player)) == 2 && (b[i][j + 1] != computer) && (b[i][j + 2] != computer) && (b[i][j + 3] != computer)) {
						if (b[i][j + 1] == 0) b[i][j + 1] = computer;
						else if (b[i][j + 2] == 0) b[i][j + 2] = computer;
						else b[i][j + 3] = computer;
						isOver = 1;
						break;
					}
				}
				//左上 
				if (i - 3 >= 0 && j - 3 >= 0) {
					if (((b[i - 1][j - 1] == player) + (b[i - 2][j - 2] == player) + (b[i - 3][j - 3] == player)) == 2 && (b[i - 1][j - 1] != computer) && (b[i - 2][j - 2] != computer) && (b[i - 3][j - 3] != computer)) {
						if (b[i - 1][j - 1] == 0) b[i - 1][j - 1] = computer;
						else if (b[i - 2][j - 2] == 0) b[i - 2][j - 2] = computer;
						else b[i - 3][j - 3] = computer;
						isOver = 1;
						break;
					}
				}
				//左下 
				if (i + 3 <= Board_size - 1 && j - 3 >= 0) {
					if (((b[i + 1][j - 1] == player) + (b[i + 2][j - 2] == player) + (b[i + 3][j - 3] == player)) == 2 && (b[i + 1][j - 1] != computer) && (b[i + 2][j - 2] != computer) && (b[i + 3][j - 3] != computer)) {
						if (b[i + 1][j - 1] == 0) b[i + 1][j - 1] = computer;
						else if (b[i + 2][j - 2] == 0) b[i + 2][j - 2] = computer;
						else b[i + 3][j - 3] = computer;
						isOver = 1;
						break;
					}
				}
				//右上 
				if (i - 3 >= 0 && j + 3 <= Board_size - 1) {
					if (((b[i - 1][j + 1] == player) + (b[i - 2][j + 2] == player) + (b[i - 3][j + 3] == player)) == 2 && (b[i - 1][j + 1] != computer) && (b[i - 2][j + 2] != computer) && (b[i - 3][j + 3] != computer)) {
						if (b[i - 1][j + 1] == 0) b[i - 1][j + 1] = computer;
						else if (b[i - 2][j + 2] == 0) b[i - 2][j + 2] = computer;
						else b[i - 3][j + 3] = computer;
						isOver = 1;
						break;
					}
				}
				//右下 
				if (i + 3 <= Board_size - 1 && j + 3 <= Board_size - 1) {
					if (((b[i + 1][j + 1] == player) + (b[i + 2][j + 2] == player) + (b[i + 3][j + 3] == player)) == 2 && (b[i + 1][j + 1] != computer) && (b[i + 2][j + 2] != computer) && (b[i + 3][j + 3] != computer)) {
						if (b[i + 1][j + 1] == 0) b[i + 1][j + 1] = computer;
						else if (b[i + 2][j + 2] == 0) b[i + 2][j + 2] = computer;
						else b[i + 3][j + 3] = computer;
						isOver = 1;
						break;
					}
				}
			}
		}
		if (isOver == 1) {
			return;
		}
	}

	//9 检查自己 !OO_的情况
	//           _
	//           O
	//           O

	for (i = 0; i < Board_size; i++) {
		int count_danger = 0;
		for (j = 0; j < Board_size; j++)
		{
			if (b[i][j] == 0) {
				count_danger = 0;
				//core
				//up 
				if (i - 3 >= 0) {
					if (((b[i - 1][j] == computer) + (b[i - 2][j] == computer) + (b[i - 3][j] == computer)) == 2 && (b[i - 1][j] != player) && (b[i - 2][j] != player) && (b[i - 3][j] != player)) {
						count_danger++;
					}
				}
				//down
				if (i + 3 <= Board_size - 1) {
					if (((b[i + 1][j] == computer) + (b[i + 2][j] == computer) + (b[i + 3][j] == computer)) == 2 && (b[i + 1][j] != player) && (b[i + 2][j] != player) && (b[i + 3][j] != player)) {
						count_danger++;
					}
				}
				//left
				if (j - 3 >= 0) {
					if (((b[i][j - 1] == computer) + (b[i][j - 2] == computer) + (b[i][j - 3] == computer)) == 2 && (b[i][j - 1] != player) && (b[i][j - 2] != player) && (b[i][j - 3] != player)) {
						count_danger++;
					}
				}
				//right
				if (j + 3 <= Board_size - 1) {
					if (((b[i][j + 1] == computer) + (b[i][j + 2] == computer) + (b[i][j + 3] == computer)) == 2 && (b[i][j + 1] != player) && (b[i][j + 2] != player) && (b[i][j + 3] != player)) {
						count_danger++;
					}
				}
				//左上 
				if (i - 3 >= 0 && j - 3 >= 0) {
					if (((b[i - 1][j - 1] == computer) + (b[i - 2][j - 2] == computer) + (b[i - 3][j - 3] == computer)) == 2 && (b[i - 1][j - 1] != player) && (b[i - 2][j - 2] != player) && (b[i - 3][j - 3] != player)) {
						count_danger++;
					}
				}
				//左下 
				if (i + 3 <= Board_size - 1 && j - 3 >= 0) {
					if (((b[i + 1][j - 1] == computer) + (b[i + 2][j - 2] == computer) + (b[i + 3][j - 3] == computer)) == 2 && (b[i + 1][j - 1] != player) && (b[i + 2][j - 2] != player) && (b[i + 3][j - 3] != player)) {
						count_danger++;
					}
				}
				//右上 
				if (i - 3 >= 0 && j + 3 <= Board_size - 1) {
					if (((b[i - 1][j + 1] == computer) + (b[i - 2][j + 2] == computer) + (b[i - 3][j + 3] == computer)) == 2 && (b[i - 1][j + 1] != player) && (b[i - 2][j + 2] != player) && (b[i - 3][j + 3] != player)) {
						count_danger++;
					}
				}
				//右下 
				if (i + 3 <= Board_size - 1 && j + 3 <= Board_size - 1) {
					if (((b[i + 1][j + 1] == computer) + (b[i + 2][j + 2] == computer) + (b[i + 3][j + 3] == computer)) == 2 && (b[i + 1][j + 1] != player) && (b[i + 2][j + 2] != player) && (b[i + 3][j + 3] != player)) {
						count_danger++;
					}
				}
				if (count_danger >= 2) {
					b[i][j] = computer;
					return;
				}
			}

		}
	}

	//10 检查自己 OOO_  OO_O  O_OO 的情况
	for (i = 0; i < Board_size; i++) {
		int isOver = 0;
		for (j = 0; j < Board_size; j++)
		{
			if (b[i][j] == computer) {
				//core
				//up 
				if (i - 3 >= 0) {
					if (((b[i - 1][j] == computer) + (b[i - 2][j] == computer) + (b[i - 3][j] == computer)) == 2 && (b[i - 1][j] != player) && (b[i - 2][j] != player) && (b[i - 3][j] != player)) {
						if (b[i - 1][j] == 0) b[i - 1][j] = computer;
						else if (b[i - 2][j] == 0) b[i - 2][j] = computer;
						else b[i - 3][j] = computer;
						isOver = 1;
						break;
					}
				}
				//down
				if (i + 3 <= Board_size - 1) {
					if (((b[i + 1][j] == computer) + (b[i + 2][j] == computer) + (b[i + 3][j] == computer)) == 2 && (b[i + 1][j] != player) && (b[i + 2][j] != player) && (b[i + 3][j] != player)) {
						if (b[i + 1][j] == 0) b[i + 1][j] = computer;
						else if (b[i + 2][j] == 0) b[i + 2][j] = computer;
						else b[i + 3][j] = computer;
						isOver = 1;
						break;
					}
				}
				//left
				if (j - 3 >= 0) {
					if (((b[i][j - 1] == computer) + (b[i][j - 2] == computer) + (b[i][j - 3] == computer)) == 2 && (b[i][j - 1] != player) && (b[i][j - 2] != player) && (b[i][j - 3] != player)) {
						if (b[i][j - 1] == 0) b[i][j - 1] = computer;
						else if (b[i][j - 2] == 0) b[i][j - 2] = computer;
						else b[i][j - 3] = computer;
						isOver = 1;
						break;
					}
				}
				//right
				if (j + 3 <= Board_size - 1) {
					if (((b[i][j + 1] == computer) + (b[i][j + 2] == computer) + (b[i][j + 3] == computer)) == 2 && (b[i][j + 1] != player) && (b[i][j + 2] != player) && (b[i][j + 3] != player)) {
						if (b[i][j + 1] == 0) b[i][j + 1] = computer;
						else if (b[i][j + 2] == 0) b[i][j + 2] = computer;
						else b[i][j + 3] = computer;
						isOver = 1;
						break;
					}
				}
				//左上 
				if (i - 3 >= 0 && j - 3 >= 0) {
					if (((b[i - 1][j - 1] == computer) + (b[i - 2][j - 2] == computer) + (b[i - 3][j - 3] == computer)) == 2 && (b[i - 1][j - 1] != player) && (b[i - 2][j - 2] != player) && (b[i - 3][j - 3] != player)) {
						if (b[i - 1][j - 1] == 0) b[i - 1][j - 1] = computer;
						else if (b[i - 2][j - 2] == 0) b[i - 2][j - 2] = computer;
						else b[i - 3][j - 3] = computer;
						isOver = 1;
						break;
					}
				}
				//左下 
				if (i + 3 <= Board_size - 1 && j - 3 >= 0) {
					if (((b[i + 1][j - 1] == computer) + (b[i + 2][j - 2] == computer) + (b[i + 3][j - 3] == computer)) == 2 && (b[i + 1][j - 1] != player) && (b[i + 2][j - 2] != player) && (b[i + 3][j - 3] != player)) {
						if (b[i + 1][j - 1] == 0) b[i + 1][j - 1] = computer;
						else if (b[i + 2][j - 2] == 0) b[i + 2][j - 2] = computer;
						else b[i + 3][j - 3] = computer;
						isOver = 1;
						break;
					}
				}
				//右上 
				if (i - 3 >= 0 && j + 3 <= Board_size - 1) {
					if (((b[i - 1][j + 1] == computer) + (b[i - 2][j + 2] == computer) + (b[i - 3][j + 3] == computer)) == 2 && (b[i - 1][j + 1] != player) && (b[i - 2][j + 2] != player) && (b[i - 3][j + 3] != player)) {
						if (b[i - 1][j + 1] == 0) b[i - 1][j + 1] = computer;
						else if (b[i - 2][j + 2] == 0) b[i - 2][j + 2] = computer;
						else b[i - 3][j + 3] = computer;
						isOver = 1;
						break;
					}
				}
				//右下 
				if (i + 3 <= Board_size - 1 && j + 3 <= Board_size - 1) {
					if (((b[i + 1][j + 1] == computer) + (b[i + 2][j + 2] == computer) + (b[i + 3][j + 3] == computer)) == 2 && (b[i + 1][j + 1] != player) && (b[i + 2][j + 2] != player) && (b[i + 3][j + 3] != player)) {
						if (b[i + 1][j + 1] == 0) b[i + 1][j + 1] = computer;
						else if (b[i + 2][j + 2] == 0) b[i + 2][j + 2] = computer;
						else b[i + 3][j + 3] = computer;
						isOver = 1;
						break;
					}
				}
			}
		}
		if (isOver == 1) {
			return;
		}
	}

	//11 检查自己 _OO__   _O_O_   __OO_ 的情况
	for (i = 0; i < Board_size; i++) {
		int isOver = 0;
		for (j = 0; j < Board_size; j++)
		{
			if (b[i][j] == 0) {
				//core
				//up 
				if (i - 4 >= 0) {
					if (((b[i - 1][j] == computer) + (b[i - 2][j] == computer) + (b[i - 3][j] == computer)) == 2 && (b[i - 1][j] != player) && (b[i - 2][j] != player) && (b[i - 3][j] != player) && b[i - 4][j] == 0) {
						if (b[i - 1][j] == 0) b[i - 1][j] = computer;
						else if (b[i - 2][j] == 0) b[i - 2][j] = computer;
						else b[i - 3][j] = computer;
						isOver = 1;
						break;
					}
				}
				//down
				if (i + 4 <= Board_size - 1) {
					if (((b[i + 1][j] == computer) + (b[i + 2][j] == computer) + (b[i + 3][j] == computer)) == 2 && (b[i + 1][j] != player) && (b[i + 2][j] != player) && (b[i + 3][j] != player) && b[i + 4][j] == 0) {
						if (b[i + 1][j] == 0) b[i + 1][j] = computer;
						else if (b[i + 2][j] == 0) b[i + 2][j] = computer;
						else b[i + 3][j] = computer;
						isOver = 1;
						break;
					}
				}
				//left
				if (j - 4 >= 0) {
					if (((b[i][j - 1] == computer) + (b[i][j - 2] == computer) + (b[i][j - 3] == computer)) == 2 && (b[i][j - 1] != player) && (b[i][j - 2] != player) && (b[i][j - 3] != player) && b[i][j - 4] == 0) {
						if (b[i][j - 1] == 0) b[i][j - 1] = computer;
						else if (b[i][j - 2] == 0) b[i][j - 2] = computer;
						else b[i][j - 3] = computer;
						isOver = 1;
						break;
					}
				}
				//right
				if (j + 4 <= Board_size - 1) {
					if (((b[i][j + 1] == computer) + (b[i][j + 2] == computer) + (b[i][j + 3] == computer)) == 2 && (b[i][j + 1] != player) && (b[i][j + 2] != player) && (b[i][j + 3] != player) && b[i][j + 4] == 0) {
						if (b[i][j + 1] == 0) b[i][j + 1] = computer;
						else if (b[i][j + 2] == 0) b[i][j + 2] = computer;
						else b[i][j + 3] = computer;
						isOver = 1;
						break;
					}
				}
				//左上 
				if (i - 4 >= 0 && j - 4 >= 0) {
					if (((b[i - 1][j - 1] == computer) + (b[i - 2][j - 2] == computer) + (b[i - 3][j - 3] == computer)) == 2 && (b[i - 1][j - 1] != player) && (b[i - 2][j - 2] != player) && (b[i - 3][j - 3] != player) && b[i - 4][j - 4] == 0) {
						if (b[i - 1][j - 1] == 0) b[i - 1][j - 1] = computer;
						else if (b[i - 2][j - 2] == 0) b[i - 2][j - 2] = computer;
						else b[i - 3][j - 3] = computer;
						isOver = 1;
						break;
					}
				}
				//左下 
				if (i + 4 <= Board_size - 1 && j - 4 >= 0) {
					if (((b[i + 1][j - 1] == computer) + (b[i + 2][j - 2] == computer) + (b[i + 3][j - 3] == computer)) == 2 && (b[i + 1][j - 1] != player) && (b[i + 2][j - 2] != player) && (b[i + 3][j - 3] != player) && b[i + 4][j - 4] == 0) {
						if (b[i + 1][j - 1] == 0) b[i + 1][j - 1] = computer;
						else if (b[i + 2][j - 2] == 0) b[i + 2][j - 2] = computer;
						else b[i + 3][j - 3] = computer;
						isOver = 1;
						break;
					}
				}
				//右上 
				if (i - 4 >= 0 && j + 4 <= Board_size - 1) {
					if (((b[i - 1][j + 1] == computer) + (b[i - 2][j + 2] == computer) + (b[i - 3][j + 3] == computer)) == 2 && (b[i - 1][j + 1] != player) && (b[i - 2][j + 2] != player) && (b[i - 3][j + 3] != player) && b[i - 4][j + 4] == 0) {
						if (b[i - 1][j + 1] == 0) b[i - 1][j + 1] = computer;
						else if (b[i - 2][j + 2] == 0) b[i - 2][j + 2] = computer;
						else b[i - 3][j + 3] = computer;
						isOver = 1;
						break;
					}
				}
				//右下 
				if (i + 4 <= Board_size - 1 && j + 4 <= Board_size - 1) {
					if (((b[i + 1][j + 1] == computer) + (b[i + 2][j + 2] == computer) + (b[i + 3][j + 3] == computer)) == 2 && (b[i + 1][j + 1] != player) && (b[i + 2][j + 2] != player) && (b[i + 3][j + 3] != player) && b[i + 4][j + 4] == 0) {
						if (b[i + 1][j + 1] == 0) b[i + 1][j + 1] = computer;
						else if (b[i + 2][j + 2] == 0) b[i + 2][j + 2] = computer;
						else b[i + 3][j + 3] = computer;
						isOver = 1;
						break;
					}
				}
			}
		}
		if (isOver == 1) {
			return;
		}
	}

	//12 检查player  _OO__   _O_O_   __OO_ 的情况
	for (i = 0; i < Board_size; i++) {
		int isOver = 0;
		for (j = 0; j < Board_size; j++)
		{
			if (b[i][j] == 0) {
				//core
				//up 
				if (i - 4 >= 0) {
					if (((b[i - 1][j] == player) + (b[i - 2][j] == player) + (b[i - 3][j] == player)) == 2 && (b[i - 1][j] != computer) && (b[i - 2][j] != computer) && (b[i - 3][j] != computer) && b[i - 4][j] == 0) {
						if (b[i - 1][j] == 0) b[i - 1][j] = computer;
						else if (b[i - 2][j] == 0) b[i - 2][j] = computer;
						else b[i - 3][j] = computer;
						isOver = 1;
						break;
					}
				}
				//down
				if (i + 4 <= Board_size - 1) {
					if (((b[i + 1][j] == player) + (b[i + 2][j] == player) + (b[i + 3][j] == player)) == 2 && (b[i + 1][j] != computer) && (b[i + 2][j] != computer) && (b[i + 3][j] != computer) && b[i + 4][j] == 0) {
						if (b[i + 1][j] == 0) b[i + 1][j] = computer;
						else if (b[i + 2][j] == 0) b[i + 2][j] = computer;
						else b[i + 3][j] = computer;
						isOver = 1;
						break;
					}
				}
				//left
				if (j - 4 >= 0) {
					if (((b[i][j - 1] == player) + (b[i][j - 2] == player) + (b[i][j - 3] == player)) == 2 && (b[i][j - 1] != computer) && (b[i][j - 2] != computer) && (b[i][j - 3] != computer) && b[i][j - 4] == 0) {
						if (b[i][j - 1] == 0) b[i][j - 1] = computer;
						else if (b[i][j - 2] == 0) b[i][j - 2] = computer;
						else b[i][j - 3] = computer;
						isOver = 1;
						break;
					}
				}
				//right
				if (j + 4 <= Board_size - 1) {
					if (((b[i][j + 1] == player) + (b[i][j + 2] == player) + (b[i][j + 3] == player)) == 2 && (b[i][j + 1] != computer) && (b[i][j + 2] != computer) && (b[i][j + 3] != computer) && b[i][j + 4] == 0) {
						if (b[i][j + 1] == 0) b[i][j + 1] = computer;
						else if (b[i][j + 2] == 0) b[i][j + 2] = computer;
						else b[i][j + 3] = computer;
						isOver = 1;
						break;
					}
				}
				//左上 
				if (i - 4 >= 0 && j - 4 >= 0) {
					if (((b[i - 1][j - 1] == player) + (b[i - 2][j - 2] == player) + (b[i - 3][j - 3] == player)) == 2 && (b[i - 1][j - 1] != computer) && (b[i - 2][j - 2] != computer) && (b[i - 3][j - 3] != computer) && b[i - 4][j - 4] == 0) {
						if (b[i - 1][j - 1] == 0) b[i - 1][j - 1] = computer;
						else if (b[i - 2][j - 2] == 0) b[i - 2][j - 2] = computer;
						else b[i - 3][j - 3] = computer;
						isOver = 1;
						break;
					}
				}
				//左下 
				if (i + 4 <= Board_size - 1 && j - 4 >= 0) {
					if (((b[i + 1][j - 1] == player) + (b[i + 2][j - 2] == player) + (b[i + 3][j - 3] == player)) == 2 && (b[i + 1][j - 1] != computer) && (b[i + 2][j - 2] != computer) && (b[i + 3][j - 3] != computer) && b[i + 4][j - 4] == 0) {
						if (b[i + 1][j - 1] == 0) b[i + 1][j - 1] = computer;
						else if (b[i + 2][j - 2] == 0) b[i + 2][j - 2] = computer;
						else b[i + 3][j - 3] = computer;
						isOver = 1;
						break;
					}
				}
				//右上 
				if (i - 4 >= 0 && j + 4 <= Board_size - 1) {
					if (((b[i - 1][j + 1] == player) + (b[i - 2][j + 2] == player) + (b[i - 3][j + 3] == player)) == 2 && (b[i - 1][j + 1] != computer) && (b[i - 2][j + 2] != computer) && (b[i - 3][j + 3] != computer) && b[i - 4][j + 4] == 0) {
						if (b[i - 1][j + 1] == 0) b[i - 1][j + 1] = computer;
						else if (b[i - 2][j + 2] == 0) b[i - 2][j + 2] = computer;
						else b[i - 3][j + 3] = computer;
						isOver = 1;
						break;
					}
				}
				//右下 
				if (i + 4 <= Board_size - 1 && j + 4 <= Board_size - 1) {
					if (((b[i + 1][j + 1] == player) + (b[i + 2][j + 2] == player) + (b[i + 3][j + 3] == player)) == 2 && (b[i + 1][j + 1] != computer) && (b[i + 2][j + 2] != computer) && (b[i + 3][j + 3] != computer) && b[i + 4][j + 4] == 0) {
						if (b[i + 1][j + 1] == 0) b[i + 1][j + 1] = computer;
						else if (b[i + 2][j + 2] == 0) b[i + 2][j + 2] = computer;
						else b[i + 3][j + 3] = computer;
						isOver = 1;
						break;
					}
				}
			}
		}
		if (isOver == 1) {
			return;
		}
	}

	//13 检查自己 !O_的情况
	//           _
	//           O
	for (i = 0; i < Board_size; i++) {
		int count_danger = 0;
		for (j = 0; j < Board_size; j++)
		{
			if (b[i][j] == 0) {
				count_danger = 0;
				//core
				//up 
				if (i - 2 >= 0) {
					if (((b[i - 1][j] == computer) + (b[i - 2][j] == computer)) == 1 && (b[i - 1][j] != player) && (b[i - 2][j] != player)) {
						count_danger++;
					}
				}
				//down
				if (i + 2 <= Board_size - 1) {
					if (((b[i + 1][j] == computer) + (b[i + 2][j] == computer)) == 1 && (b[i + 1][j] != player) && (b[i + 2][j] != player)) {
						count_danger++;
					}
				}
				//left
				if (j - 2 >= 0) {
					if (((b[i][j - 1] == computer) + (b[i][j - 2] == computer)) == 1 && (b[i][j - 1] != player) && (b[i][j - 2] != player)) {
						count_danger++;
					}
				}
				//right
				if (j + 2 <= Board_size - 1) {
					if (((b[i][j + 1] == computer) + (b[i][j + 2] == computer)) == 1 && (b[i][j + 1] != player) && (b[i][j + 2] != player)) {
						count_danger++;
					}
				}
				//左上 
				if (i - 2 >= 0 && j - 2 >= 0) {
					if (((b[i - 1][j - 1] == computer) + (b[i - 2][j - 2] == computer)) == 1 && (b[i - 1][j - 1] != player) && (b[i - 2][j - 2] != player)) {
						count_danger++;
					}
				}
				//左下 
				if (i + 2 <= Board_size - 1 && j - 2 >= 0) {
					if (((b[i + 1][j - 1] == computer) + (b[i + 2][j - 2] == computer)) == 1 && (b[i + 1][j - 1] != player) && (b[i + 2][j - 2] != player)) {
						count_danger++;
					}
				}
				//右上 
				if (i - 2 >= 0 && j + 2 <= Board_size - 1) {
					if (((b[i - 1][j + 1] == computer) + (b[i - 2][j + 2] == computer)) == 1 && (b[i - 1][j + 1] != player) && (b[i - 2][j + 2] != player)) {
						count_danger++;
					}
				}
				//右下 
				if (i + 2 <= Board_size - 1 && j + 2 <= Board_size - 1) {
					if (((b[i + 1][j + 1] == computer) + (b[i + 2][j + 2] == computer)) == 1 && (b[i + 1][j + 1] != player) && (b[i + 2][j + 2] != player)) {
						count_danger++;
					}
				}
				if (count_danger >= 2) {
					b[i][j] = computer;
					return;
				}
			}

		}
	}

	//14 检查player !O_的情况
	//           _
	//           O
	for (i = 0; i < Board_size; i++) {
		int count_danger = 0;
		for (j = 0; j < Board_size; j++)
		{
			if (b[i][j] == 0) {
				count_danger = 0;
				//core
				//up 
				if (i - 2 >= 0) {
					if (((b[i - 1][j] == player) + (b[i - 2][j] == player)) == 1 && (b[i - 1][j] != computer) && (b[i - 2][j] != computer)) {
						count_danger++;
					}
				}
				//down
				if (i + 2 <= Board_size - 1) {
					if (((b[i + 1][j] == player) + (b[i + 2][j] == player)) == 1 && (b[i + 1][j] != computer) && (b[i + 2][j] != computer)) {
						count_danger++;
					}
				}
				//left
				if (j - 2 >= 0) {
					if (((b[i][j - 1] == player) + (b[i][j - 2] == player)) == 1 && (b[i][j - 1] != computer) && (b[i][j - 2] != computer)) {
						count_danger++;
					}
				}
				//right
				if (j + 2 <= Board_size - 1) {
					if (((b[i][j + 1] == player) + (b[i][j + 2] == player)) == 1 && (b[i][j + 1] != computer) && (b[i][j + 2] != computer)) {
						count_danger++;
					}
				}
				//左上 
				if (i - 2 >= 0 && j - 2 >= 0) {
					if (((b[i - 1][j - 1] == player) + (b[i - 2][j - 2] == player)) == 1 && (b[i - 1][j - 1] != computer) && (b[i - 2][j - 2] != computer)) {
						count_danger++;
					}
				}
				//左下 
				if (i + 2 <= Board_size - 1 && j - 2 >= 0) {
					if (((b[i + 1][j - 1] == player) + (b[i + 2][j - 2] == player)) == 1 && (b[i + 1][j - 1] != computer) && (b[i + 2][j - 2] != computer)) {
						count_danger++;
					}
				}
				//右上 
				if (i - 2 >= 0 && j + 2 <= Board_size - 1) {
					if (((b[i - 1][j + 1] == player) + (b[i - 2][j + 2] == player)) == 1 && (b[i - 1][j + 1] != computer) && (b[i - 2][j + 2] != computer)) {
						count_danger++;
					}
				}
				//右下 
				if (i + 2 <= Board_size - 1 && j + 2 <= Board_size - 1) {
					if (((b[i + 1][j + 1] == player) + (b[i + 2][j + 2] == player)) == 1 && (b[i + 1][j + 1] != computer) && (b[i + 2][j + 2] != computer)) {
						count_danger++;
					}
				}
				if (count_danger >= 2) {
					b[i][j] = computer;
					return;
				}
			}

		}
	}

	//15 random
	init_random_step((Board_size - 1) / 2, (Board_size - 1) / 2, b, flag, cnt);
}

int check_player(int b[][Board_size], int flag) {
	int i, j;
	int isOver = 0;
	int player = (flag == 1) ? 1 : 2;
	for (i = 0; i < Board_size; i++) {
		for (j = 0; j < Board_size; j++)
		{
			if (b[i][j] == player) {
				//core
				//up 
				if (i - 4 >= 0) {
					if (b[i - 1][j] == player && b[i - 2][j] == player && b[i - 3][j] == player && b[i - 4][j] == player) {
						isOver = 1;
						break;
					}
				}
				//down
				if (i + 4 <= Board_size - 1) {
					if (b[i + 1][j] == player && b[i + 2][j] == player && b[i + 3][j] == player && b[i + 4][j] == player) {
						isOver = 1;
						break;
					}
				}
				//left
				if (j - 4 >= 0) {
					if (b[i][j - 1] == player && b[i][j - 2] == player && b[i][j - 3] == player && b[i][j - 4] == player) {
						isOver = 1;
						break;
					}
				}
				//right
				if (j + 4 <= Board_size - 1) {
					if (b[i][j + 1] == player && b[i][j + 2] == player && b[i][j + 3] == player && b[i][j + 4] == player) {
						isOver = 1;
						break;
					}
				}
				//左上 
				if (i - 4 >= 0 && j - 4 >= 0) {
					if (b[i - 1][j - 1] == player && b[i - 2][j - 2] == player && b[i - 3][j - 3] == player && b[i - 4][j - 4] == player) {
						isOver = 1;
						break;
					}
				}
				//左下 
				if (i + 4 <= Board_size - 1 && j - 4 >= 0) {
					if (b[i + 1][j - 1] == player && b[i + 2][j - 2] == player && b[i + 3][j - 3] == player && b[i + 4][j - 4] == player) {
						isOver = 1;
						break;
					}
				}
				//右上 
				if (i - 4 >= 0 && j + 4 <= Board_size - 1) {
					if (b[i - 1][j + 1] == player && b[i - 2][j + 2] == player && b[i - 3][j + 3] == player && b[i - 4][j + 4] == player) {
						isOver = 1;
						break;
					}
				}
				//右下 
				if (i + 4 <= Board_size - 1 && j + 4 <= Board_size - 1) {
					if (b[i + 1][j + 1] == player && b[i + 2][j + 2] == player && b[i + 3][j + 3] == player && b[i + 4][j + 4] == player) {
						isOver = 1;
						break;
					}
				}
			}
		}
		if (isOver == 1) {
			return 1;
		}
	}
	return 0;
}

int  check_computer(int b[][Board_size], int flag) {
	int i, j;
	int isOver = 0;
	int computer = (flag == 2) ? 1 : 2;
	for (i = 0; i < Board_size; i++) {
		for (j = 0; j < Board_size; j++)
		{
			if (b[i][j] == computer) {
				//core
				//up 
				if (i - 4 >= 0) {
					if (b[i - 1][j] == computer && b[i - 2][j] == computer && b[i - 3][j] == computer && b[i - 4][j] == computer) {
						isOver = 1;
						break;
					}
				}
				//down
				if (i + 4 <= Board_size - 1) {
					if (b[i + 1][j] == computer && b[i + 2][j] == computer && b[i + 3][j] == computer && b[i + 4][j] == computer) {
						isOver = 1;
						break;
					}
				}
				//left
				if (j - 4 >= 0) {
					if (b[i][j - 1] == computer && b[i][j - 2] == computer && b[i][j - 3] == computer && b[i][j - 4] == computer) {
						isOver = 1;
						break;
					}
				}
				//right
				if (j + 4 <= Board_size - 1) {
					if (b[i][j + 1] == computer && b[i][j + 2] == computer && b[i][j + 3] == computer && b[i][j + 4] == computer) {
						isOver = 1;
						break;
					}
				}
				//左上 
				if (i - 4 >= 0 && j - 4 >= 0) {
					if (b[i - 1][j - 1] == computer && b[i - 2][j - 2] == computer && b[i - 3][j - 3] == computer && b[i - 4][j - 4] == computer) {
						isOver = 1;
						break;
					}
				}
				//左下 
				if (i + 4 <= Board_size - 1 && j - 4 >= 0) {
					if (b[i + 1][j - 1] == computer && b[i + 2][j - 2] == computer && b[i + 3][j - 3] == computer && b[i + 4][j - 4] == computer) {
						isOver = 1;
						break;
					}
				}
				//右上 
				if (i - 4 >= 0 && j + 4 <= Board_size - 1) {
					if (b[i - 1][j + 1] == computer && b[i - 2][j + 2] == computer && b[i - 3][j + 3] == computer && b[i - 4][j + 4] == computer) {
						isOver = 1;
						break;
					}
				}
				//右下 
				if (i + 4 <= Board_size - 1 && j + 4 <= Board_size - 1) {
					if (b[i + 1][j + 1] == computer && b[i + 2][j + 2] == computer && b[i + 3][j + 3] == computer && b[i + 4][j + 4] == computer) {
						isOver = 1;
						break;
					}
				}
			}
		}
		if (isOver == 1) {
			return 1;
		}
	}
	return 0;
}

int check_full(int b[][Board_size]) {
	int i, j;
	for (i = 0; i < Board_size; i++) {
		for (j = 0; j < Board_size; j++) {
			if (b[i][j] == 0) return 0;
		}
	}
	return 1;
}