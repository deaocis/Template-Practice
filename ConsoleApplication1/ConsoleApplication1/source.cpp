#include <stdio.h>
#include <Windows.h>
#include <conio.h>

#define SCR_WIDTH 40
#define SCR_HEIGHT 20
#define MAX_BULLETS 20

typedef struct
{
	int state;
	int width;
	int height;

	int pos_x;
	int pos_y;

	char *image_;
} Object;

Object player;
Object enemy;

Object *p_bullet_array[MAX_BULLETS];
//Object *p_bullet = NULL;	

int score = 0;
//score always important

char front_buffer[SCR_HEIGHT][SCR_WIDTH];
char back_buffer[SCR_HEIGHT][SCR_WIDTH];

void moveCursorTo(const short x, const short y);
void drawToBackBuffer(const int i, const int j, char* image);
void render();
void shootBullet();
void drawBoundary();
void drawAll();

int main()
{
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

	//initialize bullet
	for (int b = 0; b < MAX_BULLETS; b++)
	{
		p_bullet_array[b] = NULL;
	}
	player.pos_x = 10;
	player.pos_y = 3;
	player.image_ = "<-O->";
	player.width = 5;
	player.height = 1;

	enemy.state = 0;
	enemy.pos_x = 15;
	enemy.pos_y = SCR_HEIGHT-3;
	enemy.image_ = "(O_O)\0>XoX<\0>XoX<\0>XoX<\0>XoX<\0>XoX<\0>XoX<\0>XoX<\0>XoX<\0>XoX<\0>XoX<\0>XoX<\0>XoX<\0>XoX<\0>XoX<\0>XoX<";
	enemy.width = 5;
	enemy.height = 1;

	while (1)    //main game loop
	{
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			if (player.pos_x > 1)
				player.pos_x--;
			else if (player.pos_x <= 1)
				player.pos_x = SCR_WIDTH;
		}

		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			if (player.pos_x < SCR_WIDTH - player.width - 1)
				player.pos_x++;
			else if (player.pos_x >= SCR_WIDTH - player.width - 1)
				player.pos_x = 1;
		}

		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			if (player.pos_y > 0)
				player.pos_y--;
		}

		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			if (player.pos_y < SCR_HEIGHT - 1)
				player.pos_y++;
		}

		static bool roaded = true;

		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			if (roaded == true)
			{
				shootBullet();
				roaded = false;
			}
		}

		else
		{
			roaded = true;
		}

		for (int b = 0; b < MAX_BULLETS; b++)
		{
			if (p_bullet_array[b] != NULL)
			{
				if (p_bullet_array[b]->pos_y > SCR_HEIGHT - 1)
				{
					free(p_bullet_array[b]);
					p_bullet_array[b] = NULL;
				}
				else
					p_bullet_array[b]->pos_y++;
			}
		}

		const int dice = rand() % 7; //0:stay, 1:right, 2:left
		switch (dice)
		{
		case 1:  //right
			if (enemy.pos_x < SCR_WIDTH - enemy.width - 1)
				enemy.pos_x++;
			else if (enemy.pos_x >= SCR_WIDTH - enemy.width - 1)
				enemy.pos_x = 1;
			break;

		case 2:  //left
			if (enemy.pos_x > 1)
				enemy.pos_x--;
			else if (enemy.pos_x <= 1)
				enemy.pos_x = SCR_WIDTH;
			break;
		}

		if (enemy.state > 0)
			enemy.state--;

		for (int b = 0; b < MAX_BULLETS; b++)
		{
			if (p_bullet_array[b] != NULL)
			{
				//check collision
				if (p_bullet_array[b]->pos_y == enemy.pos_y)
					if (p_bullet_array[b]->pos_x >= enemy.pos_x&&
						p_bullet_array[b]->pos_x <= enemy.pos_x + enemy.width - 1)
					{
						score++;
						enemy.state = 15;
					}
			}
		}

		drawAll();

		render();

		Sleep(30);
	}

	for (int b = 0; b < MAX_BULLETS; b++)
	{
		if(p_bullet_array[b]!=NULL)
			free(p_bullet_array[b]);
	}
}






void moveCursorTo(const short x, const short y)
{
	const COORD pos = { x,SCR_HEIGHT - y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void drawToBackBuffer(const int i, const int j, char* image)
{
	int ix = 0;
	while (1)
	{
		if (image[ix] == '\0') break;

		back_buffer[j][i + ix] = image[ix];

		ix++;
	}
}

void render()
{
	//re-draw changed parts

	for (int j = 0; j < SCR_HEIGHT; j++)
		for (int i = 0; i < SCR_WIDTH; i++)
		{
			if (back_buffer[j][i] != front_buffer[j][i])
			{
				moveCursorTo(i, j);

				if (back_buffer[j][i] == '\0')
					printf("%c", ' ');
				else
					printf("%c", back_buffer[j][i]);
			}
		}

	//update frame buffer
	for (int j = 0; j < SCR_HEIGHT; j++)
		for (int i = 0; i < SCR_WIDTH; i++)
		{
			front_buffer[j][i] = back_buffer[j][i];
			back_buffer[j][i] = '\0';
		}
}

void shootBullet()
{
	int b_ix = -1;
	for (int b = 0; b < MAX_BULLETS; b++)
	{
		if (p_bullet_array[b] == NULL)
		{
			b_ix = b;
			break;
		}
	}

	if (b_ix == -1) return;

	p_bullet_array[b_ix] = (Object*)malloc(sizeof(Object));

	p_bullet_array[b_ix]->state = 0;
	p_bullet_array[b_ix]->width = 1;
	p_bullet_array[b_ix]->height = 1;
	p_bullet_array[b_ix]->pos_x = player.pos_x + 2;
	p_bullet_array[b_ix]->pos_y = player.pos_y;
	p_bullet_array[b_ix]->image_ = "!\0!\0^\0^\0";

}

void drawBoundary()
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

void drawAll()
{

	//draw Boundary
	drawBoundary();

	//draw Character

	drawToBackBuffer(player.pos_x, player.pos_y, player.image_);

	//draw Enemy
	char *current_state_image = enemy.image_ + (enemy.width + 1)*enemy.state;  //+1 is for null character

	drawToBackBuffer(enemy.pos_x, enemy.pos_y, current_state_image);

	//draw Bullet
	for (int b = 0; b < MAX_BULLETS; b++)
	{
		if (p_bullet_array[b] != NULL)
		{
			char *current_bullet_image = p_bullet_array[b]->image_ +
				(p_bullet_array[b]->width + 1)*p_bullet_array[b]->state;

			drawToBackBuffer(p_bullet_array[b]->pos_x, p_bullet_array[b]->pos_y, current_bullet_image);

			p_bullet_array[b]->state = (p_bullet_array[b]->state + 1) % 4;
		}
	}

	//draw Score	
	char scoretext[15];
	sprintf(scoretext, "Score:%d", score);
	drawToBackBuffer(15, SCR_HEIGHT - 1, scoretext);

}