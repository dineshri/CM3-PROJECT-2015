

enum game{ PLAYER, PROJECTILES, ENEMY };

struct block
{
	int x;
	int y;

};

struct Pipe
{
	int x;
	int y;

};

struct spike
{
	int x;
	int y;
	bool r = false;
	bool l = false;
	bool d = false;

};
struct enemies
{
	int ID;
	int x;
	int y;
	bool alive;
	int boundx;
	int boundy;

};

struct Projectiles
{
	int ID;
	int x;
	int y;
	bool live;
	int speed;
};


void resize(int);
void jump(int);

