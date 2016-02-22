/******************************************************************************
*spritehandler.cpp
*The spritehandler hanldes some of the behaviour and all of the interaction
*between the sprites.
******************************************************************************/
#include "spritehandler.h"
#include <vector>

/******************************************************************************
*spritehandler default constructor
*
******************************************************************************/
spritehandler::spritehandler(void)
{
	count=0;
	bugcount=0;
	snakecount=0;
	sprcount=0;
}
/******************************************************************************
*spritehandler defualt destructor
*This is an all or nothing function that deletes all sprites registerd
******************************************************************************/
spritehandler::~spritehandler(void)
{
	//delete the sprites
	for(int n=0;n<bugcount;n++)
		delete bug[count];
}
/******************************************************************************
*spritehandler addsprite function
*This function registers a sprite to the sprite handler
******************************************************************************/
void spritehandler::addsprite(sprite *spr)
{
	//sanity check to see if the sprite exists
	if(spr !=NULL)
	{
		sprites[count]=spr;
		count++;
	}
}
/******************************************************************************
*spritehandler create function
*create a sprite inside of the spritehandler
******************************************************************************/
void spritehandler::create()
{
	sprites[sprcount]= new sprite();
	sprcount++;
	count ++;
}
/******************************************************************************
*spritehandler addsprite function
*This function registers a sprite to the sprite handler
******************************************************************************/
void spritehandler::addbugs(bugs *buggy)
{
	//sanity check to see if the sprite exists
	if(bug !=NULL)
	{
		bug[bugcount]=buggy;
		count++;
		bugcount++;

	}
}
/******************************************************************************
*spritehandler addsprite function
*This function registers a sprite to the sprite handler
******************************************************************************/
void spritehandler::addboss(snake *snk)
{
	//sanity check to see if the sprite exists
	if(snk !=NULL)
	{
		snakes[snakecount]=snk;
		snakecount++;
		count++;
	}
}

/******************************************************************************
*spritehandler getsprite function
*accessor method that returns a sprite
******************************************************************************/
sprite *spritehandler::getsprite(int index)
{
	return sprites[index];
}
/******************************************************************************
*spritehandler getbugs function
*accessor method that returns a pointer to a bug at the specified index
******************************************************************************/
bugs *spritehandler::getbugs(int index)
{
	return bug[index];
}
/******************************************************************************
*spritehandler getsnakes function
*accessor method that returns a pointer to a snake at the specified index
******************************************************************************/
snake *spritehandler::getsnakes(int index)
{
	return snakes[index];
}
/******************************************************************************
*updatebugs function
*This funciton controls the states or potenital states of the bugs life
******************************************************************************/
void spritehandler::updatebugs()
{
	for(int i=0;i<bugcount;i++)
	{
		//if the bugs are alive
		if(getbugs(i)->alive==true)
		{
			getbugs(i)->updateroutine();
			
			//check the collisions for both the snake and the owl
			getbugs(i)->checkcollisions(getsprite(5));
			getbugs(i)->checkcollisions(getsnakes(0));

			//the bug is injured and scared...run
			if(getbugs(i)->health==5)
			{
				getbugs(i)->isdying(getbugs(i)->hurtsound);
				getbugs(i)->updatePosition();
			}
			//the bug is dead
			if(getbugs(i)->health<=0)
			{
				getbugs(i)->killsprite(getbugs(i)->deadsound,200);
				getsprite(5)->updatescore(1);
				getbugs(i)->alive=false;
				getsprite(5)->heal(5);
			}
		}
		//if not resurrect the bugs
		else if(getbugs(i)->alive==false)
		{
			getbugs(i)->resurrect();
		}
	}
}
/******************************************************************************
*udpatesnake function
*This function controls the logic of the snake
******************************************************************************/
void spritehandler::updatesnake()
{
	for(int i=0;i<snakecount;i++)
	{
		if(getsnakes(i)->alive==true)
		{

			//state 1: move left and right to the end of the screen
			//call the snake specific update routine
			getsnakes(i)->updateroutine();

			//adjust the keyframes used in the snakes animiation based on direction 
			if(getsnakes(i)->direction==1)
					getsnakes(i)->animstarty=100;
			if(getsnakes(i)->direction==-1)
				getsnakes(i)->animstarty=0;
			

			//state 2: track the player- if the snake has full health track the player
			if(getsnakes(i)->health>= 25 && getsnakes(i)->isvisable(getsprite(5))==true)
			{
				getsnakes(i)->tracksprite(getsprite(5));
			}

			//state 3:attack the player
			if(getsnakes(i)->isvisable(getsprite(5))==true &&
				getsnakes(i)->collided(getsprite(5),-5))
			{
				getsnakes(i)->attacksprite();
				getsprite(5)->checkcollisions(getsnakes(i));
			}

			//state 4: if the snake is being attacked...RUN!
			if(getsnakes(i)->collided(getsprite(5),-10) == 1 && 
				getsnakes(i)->isvisable(getsprite(5)) == false && getsprite(5)->state == 1)
			{
				getsnakes(i)->changedirection();
				getsnakes(i)->checkcollisions(getsprite(5));
			}

			//if the snake is dead
			if(getsnakes(i)->health<=0)
			{
				getsnakes(i)->killsprite(getsnakes(i)->deadsound,500);
				getsprite(5)->heal(50);
				getsprite(5)->scoremodifier++;
			}
		}
	}
}