#pragma once 
#include "sprite.h"
#include "bug.h"
#include "snake.h"

class spritehandler
{
private:
		//class variables
		//total number of sprites
		int count;
		//count for each sprite type 
		int sprcount,bugcount,snakecount;

		bool isplayer;
		sprite *sprites[100];
		bugs *bug[5];
		snake *snakes[1];
public:
		//public functions
		
		//constructor
		spritehandler(void);

		//destructor
		~spritehandler(void);

		//add sprite function
		void addsprite(sprite *spr);
		void addbugs(bugs *bug);
		void addboss(snake *snakey);

		//create function
		void create();

		//accessor method that returns a sprite
		sprite *getsprite(int index);
		bugs *getbugs(int index);
		snake *getsnakes(int index);

		//inspector method that reterives the size of the sprite
		int size(){return count;}
		int bugsize(){return bugcount;}
		
		//function called to update bugs and sprites
		void updatebugs();
		void updatesnake();
};