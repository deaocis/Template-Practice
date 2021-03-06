#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <iostream>

#define MAX2(a,b)              ((a)>(b) ? (a):(b))
#define MAX3(a,b,c)            MAX2(MAX2(a,b),(c))
#define MAX4(a,b,c,d)          MAX2(MAX3(a,b,c),(d))

#define SCR_WIDTH 45   //원래 75
#define SCR_HEIGHT 20
#define MAX_BULLETS 5
#define MAX_E_BULLETS 4
#define POWER_BULLETS 2

using namespace std;

class CellData
{
public:
	double q_[4] = { 0.0, 0.0, 0.0, 0.0 };  //up, down, left, right
	double reward = 0.0;

	CellData()
	{}

	double GetMaxQ()
	{
		return MAX4(q_[0], q_[1], q_[2], q_[3]);
	}
};

class GridWorld    //To Do: make destructor -> it is necessary?
{
public:
	int i_num, j_num;
	CellData *q_arr2d = nullptr;

	GridWorld(const int& i_res, const int& j_res)
		:i_num(i_res), j_num(j_res)
	{
		q_arr2d = new CellData[i_res*j_res];
	}

	CellData& GetCellData(const int& i, const int& j)
	{
		return q_arr2d[i + i_num*j];
	}

	bool IsInsideGrid(const int& i, const int& j)
	{
		if (i >= i_num) return false;
		if (j >= j_num) return false;
		if (i <= 0) return false;
		if (j <= 0) return false;
		return true;
	}
};

class my_player
{
public:
	int width_;
	int height_;
	int life_;

	int pos_x_;
	int pos_y_;

	char* image_;     //position(st)
					//	double reward;

	my_player()
		:pos_x_(0), pos_y_(0), width_(1), height_(1)
	{ }
};

class enemy
{
public:
	int width_;
	int height_;
	int life_;

	int pos_x_;
	int pos_y_;

	char* image_;

	enemy()
		:pos_x_(0), pos_y_(0), image_("X"), width_(1), height_(1)
	{ }
};

char front_buffer[SCR_HEIGHT][SCR_WIDTH];
char back_buffer[SCR_HEIGHT][SCR_WIDTH];

void drawToBackBuffer(const int i, const int j, char *image);
void moveCusrsorTo(const int x, const int y);          //나오는 위치 (커서 위치)
void Gameover();
void render();	//바뀐부분 다시 그리기
void drawBoundary();       //경계선 긋기

void main()
{
	my_player player, reward;
	enemy enemy,enemy1,enemy2,enemy3, enemy4, enemy5, enemy6, enemy7, enemy8, enemy9;
	for (int j = 0; j < SCR_HEIGHT; j++)
		for (int i = 0; i < SCR_WIDTH; i++)
		{
			front_buffer[j][i] = '\0';
			back_buffer[j][i] = '\0';
		}

	CONSOLE_CURSOR_INFO cur_info;
	cur_info.dwSize = 1;
	cur_info.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cur_info);

	player.image_ = "O";
	player.pos_x_ = 1;
	player.pos_y_ = 1;

	reward.image_ = "!";
	reward.pos_x_ = 43;
	reward.pos_y_ = 19;

	enemy.pos_x_ = 10;
	enemy.pos_y_ = 5;

	enemy1.pos_x_ = 10;
	enemy1.pos_y_ = 15;

	enemy2.pos_x_ = 20;
	enemy2.pos_y_ = 8;

	enemy3.pos_x_ = 30;
	enemy3.pos_y_ = 11;

	enemy4.pos_x_ = 17;
	enemy4.pos_y_ = 4;

	enemy5.pos_x_ = 24;
	enemy5.pos_y_ = 10;

	enemy6.pos_x_ = 28;
	enemy6.pos_y_ = 2;

	enemy7.pos_x_ = 5;
	enemy7.pos_y_ = 16;

	enemy8.pos_x_ = 37;
	enemy8.pos_y_ = 5;

	enemy9.pos_x_ = 25;
	enemy9.pos_y_ = 12;

	const int i_res = SCR_WIDTH, j_res = SCR_HEIGHT;

	CellData data;
	GridWorld world(i_res, j_res);

	for (int j = 0; j < j_res; j++)
	{
		for (int i = 0; i < i_res; i++)
		{
			world.GetCellData(i, j).reward = -0.1;   // All cells reward is -0.1 except reward +1, reward -1
		}
	}

	world.GetCellData(43, 19).reward = 1.0;

	world.GetCellData(10, 5).reward = -1.0;
	world.GetCellData(10, 15).reward = -1.0;
	world.GetCellData(20, 8).reward = -1.0;
	world.GetCellData(30, 11).reward = -1.0;
	world.GetCellData(17, 4).reward = -1.0;
	world.GetCellData(24, 10).reward = -1.0;
	world.GetCellData(28, 2).reward = -1.0;
	world.GetCellData(5, 16).reward = -1.0;
	world.GetCellData(37, 5).reward = -1.0;
	world.GetCellData(25, 12).reward = -1.0;

	for (int tr = 0; tr < 100000; tr++)
	{
		const int action = rand() % 4;          //action=0,1,2,3
		int i = player.pos_x_;
		int j = player.pos_y_;   //i=0, my_agent.i_=0
		int i_old = i;
		int j_old = j;               //i_old=0, j_old=0

		float learning_rate = 0.01f;
		float discount_factor = 0.9f;

		switch (action)       //move player
		{
		case 0:
			j++;
			break; //up
		case 1:
			j--;
			break;  //down
		case 2:
			i--;
			break;   //left
		case 3:
			i++;
			break;   //right
		}

		if (world.IsInsideGrid(i, j) == true)
		{
			switch (action)       //move agent
			{
			case 0:
				player.pos_y_++;
				break; //up
			case 1:
				player.pos_y_--;
				break;  //down
			case 2:
				player.pos_x_--;
				break;   //left
			case 3:
				player.pos_x_++;
				break;   //right
			}

			world.GetCellData(i_old, j_old).q_[action] += learning_rate*(world.GetCellData(player.pos_x_, player.pos_y_).reward + discount_factor*MAX4(world.GetCellData(player.pos_x_, player.pos_y_).q_[0], world.GetCellData(player.pos_x_, player.pos_y_).q_[1], world.GetCellData(player.pos_x_, player.pos_y_).q_[2], world.GetCellData(player.pos_x_, player.pos_y_).q_[3]) - world.GetCellData(i_old, j_old).q_[action]);
			//update q values of previous cell (q_t)
																																																							
			if ((player.pos_x_ == 43 && player.pos_y_ == 19) || (player.pos_x_ == 10 && player.pos_y_ == 5) || (player.pos_x_ == 10 && player.pos_y_ == 15) || (player.pos_x_ == 20 && player.pos_y_ == 8) || (player.pos_x_ == 30 && player.pos_y_ == 11) || (player.pos_x_ == 17 && player.pos_y_ == 4) || (player.pos_x_ == 24 && player.pos_y_ == 10) || (player.pos_x_ == 28 && player.pos_y_ == 2) || (player.pos_x_ == 5 && player.pos_y_ == 16) || (player.pos_x_ == 37 && player.pos_y_ == 5) || (player.pos_x_ == 25 && player.pos_y_ == 12))
			{
				player.pos_x_ = 1;
				player.pos_y_ = 1;
			}
			//reset if agent is in final cells
		}
	}

	while (true)                  // main game loop
	{	
		
		if (world.IsInsideGrid(player.pos_x_, player.pos_y_) == true)
		{
			player.pos_x_ = 1; player.pos_y_ = 1;
			double result = 0.0;
			result=MAX4(world.GetCellData(player.pos_x_, player.pos_y_).q_[0], world.GetCellData(player.pos_x_, player.pos_y_).q_[1], world.GetCellData(player.pos_x_, player.pos_y_).q_[2], world.GetCellData(player.pos_x_, player.pos_y_).q_[3]);

			if (result == world.GetCellData(player.pos_x_, player.pos_y_).q_[0])
			{
				player.pos_y_++;
			}

			else if (result == world.GetCellData(player.pos_x_, player.pos_y_).q_[1])
			{
				player.pos_y_--;
			}

			else if (result == world.GetCellData(player.pos_x_, player.pos_y_).q_[2])
			{
				player.pos_x_--;
			}

			else if (result == world.GetCellData(player.pos_x_, player.pos_y_).q_[3])
			{
				player.pos_x_++;
			}

			if (player.pos_x_ == 43 && player.pos_y_ == 19)
			{
				Gameover();
			}
		}
		//1. q[0], q[1], q[2], q[3]중에 큰 쪽으로 이동   ->q[0]=up , q[1]=down,  q[2]=left, q[3]=right
		//2. (73,19)에 이동하면 
		//3. return 0;

		//draw Boundary
		drawBoundary();

		//draw player
		char *current_state_p_image = player.image_ + (player.width_ + 1);
		char *current_state_r_image = player.image_ + (player.width_ + 1);

		char *current_state_e_image = enemy.image_ + (enemy.width_ + 1);
		char *current_state_e1_image = enemy1.image_ + (enemy1.width_ + 1);
		char *current_state_e2_image = enemy2.image_ + (enemy2.width_ + 1);
		char *current_state_e3_image = enemy3.image_ + (enemy3.width_ + 1);
		char *current_state_e4_image = enemy4.image_ + (enemy4.width_ + 1);
		char *current_state_e5_image = enemy5.image_ + (enemy5.width_ + 1);
		char *current_state_e6_image = enemy6.image_ + (enemy6.width_ + 1);
		char *current_state_e7_image = enemy7.image_ + (enemy7.width_ + 1);
		char *current_state_e8_image = enemy8.image_ + (enemy8.width_ + 1);
		char *current_state_e9_image = enemy9.image_ + (enemy9.width_ + 1);

		drawToBackBuffer(player.pos_x_, player.pos_y_, player.image_);
		drawToBackBuffer(reward.pos_x_, reward.pos_y_, reward.image_);

		drawToBackBuffer(enemy.pos_x_, enemy.pos_y_, enemy.image_);
		drawToBackBuffer(enemy1.pos_x_, enemy1.pos_y_, enemy1.image_);
		drawToBackBuffer(enemy2.pos_x_, enemy2.pos_y_, enemy2.image_);
		drawToBackBuffer(enemy3.pos_x_, enemy3.pos_y_, enemy3.image_);
		drawToBackBuffer(enemy4.pos_x_, enemy4.pos_y_, enemy4.image_);
		drawToBackBuffer(enemy5.pos_x_, enemy5.pos_y_, enemy5.image_);
		drawToBackBuffer(enemy6.pos_x_, enemy6.pos_y_, enemy6.image_);
		drawToBackBuffer(enemy7.pos_x_, enemy7.pos_y_, enemy7.image_);
		drawToBackBuffer(enemy8.pos_x_, enemy8.pos_y_, enemy8.image_);
		drawToBackBuffer(enemy9.pos_x_, enemy9.pos_y_, enemy9.image_);

		Sleep(50);

		render();
	}
	//main loop END
}

void drawToBackBuffer(const int i, const int j, char *image)
{
	int ix = 0;
	while (1)
	{
		if (image[ix] == '\0') break;

		back_buffer[j][i + ix] = image[ix];

		ix++;
	}
}

void moveCusrsorTo(const int x, const int y)            //나오는 위치 (커서 위치)
{
	const COORD pos = { x,SCR_HEIGHT - y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void render()
{
	//바뀐부분 다시 그리기
	for (int j = 0; j < SCR_HEIGHT; j++)
		for (int i = 0; i < SCR_WIDTH; i++)
		{
			if (back_buffer[j][i] != front_buffer[j][i])
			{
				moveCusrsorTo(i, j);

				if (back_buffer[j][i] == '\0')
					printf("%c", ' ');
				else
					printf("%c", back_buffer[j][i]);
			}
		}

	for (int j = 0; j < SCR_HEIGHT; j++)
		for (int i = 0; i < SCR_WIDTH; i++)
		{
			front_buffer[j][i] = back_buffer[j][i];

			back_buffer[j][i] = '\0';
		}
}

void drawBoundary()       //경계선 긋기
{
	int i = 0;
	for (int j = 0; j < SCR_HEIGHT; j++)
	{
		drawToBackBuffer(i, j, "|");
	}

	i = SCR_WIDTH - 1;
	for (int j = 0; j < SCR_HEIGHT; j++)
	{
		drawToBackBuffer(i, j, "|");
	}
}

void Gameover()
{
	CONSOLE_CURSOR_INFO cur_info;
	cur_info.dwSize = 1;
	cur_info.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cur_info);

	moveCusrsorTo(35, 10);
	printf("The End");
}