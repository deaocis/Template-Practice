#include <windows.h>
#include <stdio.h>
#include <stdbool.h>

#include <iostream>

#define MAX2(a,b)              ((a)>(b) ? (a):(b))
#define MAX3(a,b,c)            MAX2(MAX2(a,b),(c))
#define MAX4(a,b,c,d)          MAX2(MAX3(a,b,c),(d))

#define SCR_WIDTH 10   
#define SCR_HEIGHT 10
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
		if (i < 0) return false;
		if (j < 0) return false;
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

int main()
{
	my_player player, reward;
	enemy enemy, enemy1, enemy2, enemy3;
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
	player.pos_y_ = 9;

	reward.image_ = "!";
	reward.pos_x_ = 6;
	reward.pos_y_ = 5;

	enemy.pos_x_ = 6;
	enemy.pos_y_ = 6;

	enemy1.pos_x_ = 7;
	enemy1.pos_y_ = 5;

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

	world.GetCellData(6, 5).reward = 1.0;

	world.GetCellData(6, 6).reward = -1.0;
	world.GetCellData(7, 5).reward = -1.0;

	for (int tr = 0; tr < 100000; tr++)
	{
		const int action = rand() % 4;          //action=0,1,2,3
		int i = player.pos_x_;
		int j = player.pos_y_;   //i=0, my_agent.i_=0
		int i_old = i;
		int j_old = j;               //i_old=0, j_old=0

		double learning_rate = 0.1f;
		double discount_factor = 0.9f;

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

			if ((player.pos_x_ == 6 && player.pos_y_ == 5) || (player.pos_x_ == 6 && player.pos_y_ == 6) || (player.pos_x_ == 7 && player.pos_y_ == 5))
			{
				player.pos_x_ = 1;
				player.pos_y_ = 1;
			}
			//reset if agent is in final cells
		}
	}

	player.pos_x_ = 1; player.pos_y_ = 8;
	while (true)                  // main game loop
	{
		double result = 0.0;
		if (world.IsInsideGrid(player.pos_x_, player.pos_y_) == true)
		{
			result = MAX4(world.GetCellData(player.pos_x_, player.pos_y_).q_[0], world.GetCellData(player.pos_x_, player.pos_y_).q_[1], world.GetCellData(player.pos_x_, player.pos_y_).q_[2], world.GetCellData(player.pos_x_, player.pos_y_).q_[3]);
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

			if ((player.pos_x_ == 6 && player.pos_y_ == 5))
			{
				Gameover();
				return 0;
			}
		}

		else
			return 0;

		//draw Boundary
		//drawBoundary();

		//draw player,enemy
		char *current_state_p_image = player.image_ + (player.width_ + 1);
		char *current_state_r_image = player.image_ + (player.width_ + 1);

		char *current_state_e_image = enemy.image_ + (enemy.width_ + 1);
		char *current_state_e1_image = enemy1.image_ + (enemy1.width_ + 1);

		//player
		drawToBackBuffer(player.pos_x_, player.pos_y_, player.image_);
		drawToBackBuffer(reward.pos_x_, reward.pos_y_, reward.image_);

		//enemy
		drawToBackBuffer(enemy.pos_x_, enemy.pos_y_, enemy.image_);
		drawToBackBuffer(enemy1.pos_x_, enemy1.pos_y_, enemy1.image_);

		Sleep(50);

		render();
	}
	//main loop END

	return 0;
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
