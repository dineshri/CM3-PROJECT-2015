#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>						//font header files
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_primitives.h>	//primitive header file
#include "Objects.h"
enum Keys{ A, S, D, W, SPACE };
int res_x = 1280;
int res_y = 820;
int pos_x = 0;
int checkpoint = 0;
int blockNum = 0;
const int num_proj = 5;
using namespace std;


struct person{
	int r = 20;
	int feet = res_y - 55;
	int x = 200;
	int y = feet - (3 * r);
	int j_height = res_y - 400;
	int ms = 5;
	int jumpspeed = 7;
	int lives = 3;//********************************
	int score = 0;//********************************


}player;
int c = 0;
int box_width = player.r * 2 + 15;
int box_bounds = box_width;
int pipe_width = 120;
int pipe_bounds = pipe_width;
int spike_height = 50;
int spike_width = 20;



void initproj(Projectiles proj[], int size);
void drawproj(Projectiles proj[], int size);
void shoot(Projectiles proj[], int size);
void update(Projectiles proj[], int size);
void SetEnemies(enemies guys[], int size);
void StartEnemies(enemies guys[], int size);
void CreateEnemies(enemies guys[], int size, int counter);
void DrawEnemies(enemies guys[], int size, int counter);
void MoveEnemy(bool check);
void Collision(Projectiles proj[], int sizeP, enemies guys[], int sizeE, int counter);
bool CollideEnemy(enemies guys[], int size, int counter);
void CreateLuigi();

int const num_enemies = 6;
bool shot = false;

enemies guys[num_enemies];











void initSpike(spike spikes[], int size);
void createSpike(spike spikes[], int size);
bool spikeCollide(spike spikes[], int size);



void initPipe(Pipe pipes[], int size);
void createPI(Pipe pipes[], int size);
bool checkpipe(Pipe pipes[], int size);
bool pipe_right(Pipe pipes[], int size);
bool pipe_left(Pipe pipes[], int size);



void initblock(block blocks[], int size);
void create(block blocks[], int size);
bool checkblock(block blocks[], int size);
bool block_limit(block blocks[], int size);
bool noblock(block blocks[], int size);//****************************************************************
bool box_right(block blocks[], int size); //****************************************************************
bool box_left(block blocks[], int size);//******************************************************************
int main(void)
{
	int temp = 0;
	bool fallOfPipe = false;

	bool fall = false;
	bool inJmpHeight = false;//*******************
	bool fallOfBlock = false;//*******************
	int delay = 0;
	bool jump_h = false;
	bool feet_check = true;
	int jump_cnt = 0;
	bool keys[4] = { false, false, false, false };
	int const FPS = 60;
	bool Gamerunning = true;


	int const num_blocks = 50;
	block blocks[num_blocks];



	int const num_pipes = 11;
	

	int const num_spikes = 5;
	



	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;

	if (!al_init())										//initialize Allegro
		return -1;

	display = al_create_display(res_x, res_y);			//create our display object

	if (!display)										//test display object

		return -1;

	al_init_font_addon();
	al_init_ttf_addon();								//allows for text
	al_init_primitives_addon();
	al_install_keyboard();							// allows to work with keyboard keys.
	al_install_mouse();
	al_hide_mouse_cursor(display);
	timer = al_create_timer(1.0 / FPS);

	event_queue = al_create_event_queue();
	ALLEGRO_FONT *font12 = al_load_font("arial.ttf", 18, 0);		//allows for fonts...
	al_register_event_source(event_queue, al_get_keyboard_event_source());	//register keyboard to events.
	al_register_event_source(event_queue, al_get_display_event_source(display)); // Register events from display.
	al_register_event_source(event_queue, al_get_mouse_event_source());//register mouse input to events
	al_register_event_source(event_queue, al_get_timer_event_source(timer));//registers timer 

	al_start_timer(timer);


	while (Gamerunning)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);


		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_W:
				fall = false;
				if (feet_check){
					feet_check = false;
				}

				break;

			case ALLEGRO_KEY_D:
				keys[D] = true;
				break;
			case ALLEGRO_KEY_A:
				keys[A] = true;
				break;
			}

		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				Gamerunning = false;
				break;
			case ALLEGRO_KEY_L:
				resize(50);
				break;

			case ALLEGRO_KEY_W:
				if (!jump_h&&!feet_check)
					fall = true;
				break;

			case ALLEGRO_KEY_D:
				keys[D] = false;
				break;
			case ALLEGRO_KEY_A:
				keys[A] = false;
				break;

			}

		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			Gamerunning = false;
		}


		else if (ev.type == ALLEGRO_EVENT_TIMER){


			if (pos_x > temp)
			{
				player.score++;
				temp = pos_x;
			}

			// JUMPING....
			if (!feet_check && !jump_h&&!block_limit(blocks, num_blocks)){

				jump(player.jumpspeed);
				jump_cnt += player.jumpspeed;
				if (jump_cnt >= player.j_height || block_limit(blocks, num_blocks))
				{
					jump_h = true;

				}

				if (fall&& jump_cnt > 200)
					jump_h = true;

			}
			if (jump_h&&!feet_check)
			{
				delay++;
				if (delay >= 10)
				{
					jump(-(player.jumpspeed));
					jump_cnt -= player.jumpspeed;
					if (jump_cnt <= 0)
					{
						player.j_height = res_y - 400;
						jump_h = false;
						feet_check = true;

						delay = 0;
					}
					if (checkblock(blocks, num_blocks))
					{

						jump_h = false;
						feet_check = true;
						player.j_height = res_y - blocks[blockNum].y + 250;//something like this
						delay = 0;
						fallOfBlock = true;

					}




				}

			}



		}
		if (feet_check&&fallOfBlock&&!checkblock(blocks, num_blocks))
		{
			delay = 5;
			jump_h = true;
			feet_check = false;
			fallOfBlock = false;

		}

		//END JUMPING

		pos_x += keys[D] * player.ms;

		pos_x -= keys[A] * player.ms;							// MOVES CHARACTER




		if (dead)
			pos_x = 0;



		initblock(blocks, num_blocks);
		create(blocks, num_blocks);






		al_draw_filled_circle(player.x, player.y, player.r, al_map_rgb(0, 128, 0));														//player
		al_draw_filled_rectangle(player.x - player.r, player.y + player.r, player.x + player.r, player.feet, al_map_rgb(0, 128, 0));
		al_draw_filled_rectangle(0, res_y - 50, res_x, res_y, al_map_rgb(139, 69, 19));//Ground		


		al_draw_textf(font12, al_map_rgb(200, 0, 0), 30, 20, 0, "LIVES:       %d", player.lives);
		al_draw_textf(font12, al_map_rgb(210, 0, 0), 29, 40, 0, "SCORE:    %d", player.score);









		al_flip_display();
		al_clear_to_color(al_map_rgb(50, 100, 200));

	}

	al_destroy_display(display);
}

void resize(int r1){
	player.r = r1;
	player.y = player.feet - (3 * r1);
	box_bounds = player.r * 2 + 10;;
}
void jump(int y){

	player.feet -= y;
	player.y = player.feet - (3 * player.r);
}




//***************************************************************BLOCKS*******************************************************************************
void initblock(block B[], int size)
{

	B[0].x = 258;
	B[1].x = 1000;
	for (int i = 1; i < 5; i++)
		B[i + 1].x = B[i].x + box_width + 5;


	B[6].x = 1600;
	for (int i = 6; i < 10; i++)
		B[i + 1].x = B[i].x + box_width + 5;





	B[11].x = 2000;
	B[12].x = B[11].x + box_width + 5;

	for (int i = 0; i <4; i++)
	{
		B[i].y = 600;
	}


	for (int i = 5; i <9; i++)
	{
		B[i].y = 390;
	}

	B[10].y = 500;
	B[11].y = B[10].y;
	B[12].y = B[10].y;



}
void create(block B[], int size)

{

	for (int i = 0; i < size; i++)
		al_draw_filled_rectangle(B[i].x - pos_x, B[i].y, B[i].x + box_width - pos_x, B[i].y - box_width, al_map_rgb(139, 69, 19));

}
bool block_limit(block B[], int size)
{
	bool check = false;
	for (int i = 0; i < size; i++)
	{
		for (int j = -player.r; j < box_bounds * 2 - player.r + 8; j++)
		{
			if (B[i].x == player.x - j + pos_x)
			{
				for (int k = 0; k < player.jumpspeed; k++)
				{
					if ((B[i].y + box_width) == (player.y + player.r) + k + 5)
					{
						check = true;
					}
				}
			}
		}
	}
	return check;

}
bool checkblock(block B[], int size)
{
	bool check = false;
	for (int i = 0; i < size; i++)
	{
		for (int j = -player.r; j < box_bounds * 2 - player.r + 3; j++)
		{
			if (B[i].x == player.x - j + pos_x)
			{
				for (int k = 0; k < player.jumpspeed; k++)
				if (player.feet == B[i].y + k - (player.jumpspeed * 9))
				{
					check = true;
					blockNum = i;
				}
			}

		}
	}
	return check;
}
bool box_right(block B[], int size)
{

	bool check = true;
	for (int i = 0; i < size; i++)
	{
		for (int k = 0; k < 10; k++)
		if (B[i].x == player.x + player.r + pos_x - k)
		{
			for (int j = 0; j<box_width; j++)
			if (player.feet>B[i].y - j && (player.y + player.r) < B[i].y)
				check = false;

		}
	}
	return check;
}
bool box_left(block B[], int size)
{

	bool check = true;
	for (int i = 0; i < size; i++)
	{
		for (int k = 0; k < 10; k++)
		if (B[i].x + box_width == player.x - player.r + pos_x + k)
		{
			for (int j = 0; j<box_width; j++)
			if (player.feet>B[i].y - j && (player.y + player.r) < B[i].y)
				check = false;

		}
	}
	return check;
}
bool noblock(block B[], int size)
{
	bool check = false;
	for (int i = 0; i < size; i++)
	{
		for (int j = -player.r; j < box_bounds + 10 - player.r; j++)
		{
			if (B[i].x == player.x - j + pos_x)
			{
				check = true;
			}
		}
	}
	return check;
}
//*************************************************************************************************************************************************







