/******************************************************************************
*bug.cpp
*This class (bug class) inheritances from the sprite class.
*This is needed as much of the players base functionality is that of a sprite
******************************************************************************/
#include "bug.h"
/******************************************************************************
*bug Default initalization function
*This function is used to setup a sprite object of bug type
******************************************************************************/
void bugs::init()
{
		x = rand() % (SCREEN_W - 64);
		y =rand() %200+ 380;
		width = 40;
		height = 20;
		velx = rand()%3;
		vely = 0;
		animcolumns =5;
		curframe = 1;
		totalframes =5;
		animdir = 1;
		speed=rand()%10;
		direction=-1;
		//object type of enemy
		objecttype=0;
		hurtsound=load_sample("pop.wav");
		deadsound=load_sample("Squish.wav");
		dying=false;
		alive=true;
		health=15;
}
/******************************************************************************
*resurrect function
*This reanimates a dead bug 
******************************************************************************/
void bugs::resurrect()
{


	animstarty=0;
	dying=false;
	alive=true;
	init();
}