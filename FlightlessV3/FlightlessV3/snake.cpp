/******************************************************************************
*bug.cpp
*This class (bug class) inheritances from the sprite class.
*This is needed as much of the players base functionality is that of a sprite
******************************************************************************/
#include "snake.h"
/******************************************************************************
*bug Default initalization function
*This function is used to setup a sprite object of bug type
******************************************************************************/
void snake::init()
{
		x = 1500;
		y =rand() %200+ 300;
		width = 200;
		height = 100;
		velx = 2;
		vely = 0;
		animcolumns =3;
		curframe = 1;
		totalframes =3;
		animdir = 1;
		speed=2;
		direction=1;
		alive=true;
		dying=false;
		//object type of enemy
		objecttype=4;
		framedelay=20;
		health=50;
		damage=1;
		attacksound = load_sample("whip.wav");
		deadsound = load_sample("Redneck Rolls Dice-SoundBible.com-1100715950.wav");
		walkingstart=0;
}

/******************************************************************************
*isdying function
*This function controls the behaviour of the dying snake
******************************************************************************/
void snake::isdying(SAMPLE *sound)
{
	if(dying ==false)
	{
	//set dying to true
	dying=true;

	//set velocity of the dying bug
	vely=0;
	velx=-1;
	
	//bug should move in the opposite direction and cannot move up
	direction=-1;

	//play the sound when the bug is injured
	play_sample(sound,255,125,500,false);
	}
}
/******************************************************************************
*killbugfunction
*This function controls the behaviour when a bug dies
******************************************************************************/
void snake::killbug(SAMPLE *sound)
{
	//set dying to true
	alive=false;

	//play the sound for when you kill the bug
	play_sample(sound,500,125,500,false);
}
/******************************************************************************
*resurrect function
*This reanimates a dead bug 
******************************************************************************/
void snake::resurrect()
{
	animstarty=0;
	dying=false;
	alive=true;
	init();
}
/******************************************************************************
*snake instance of updateAnimation function
*This function is a helper function for the snake animation
*Inheritated from the sprite class the player version uses direction
******************************************************************************/
void snake::updateAnimation(int nextdir) 
{
	//test to see if the player is changing directions if so reset frames
	if(nextdir !=direction)
	{
		//rest the animation process 
		curframe=0;

		//set the new direction
		direction= nextdir;

		//update frame based on animdir
		//can we run the next animation
		if (++framecount > framedelay)
		{
			framecount = 0;
			curframe += animdir;

			//use the next frame in the sprite sheet
			if (curframe < 0) {
				curframe = totalframes-1;
			}
			//have we exausted all of the frames in the sprite sheet?
			if (curframe > totalframes-1) {

				//restart the process
				curframe = 0;
			} 
		}
	}
	//else we are still moving in the same direction
	else 
	{
		//update frame based on animdir
		if (++framecount > framedelay)
		{
			framecount = 0;
			curframe += animdir;

			if (curframe < 0) {
				curframe = totalframes-1;
			}
			//if we have reached the last frame
			if (curframe > totalframes-1) {
				//then reset the current frame to the walking start frame
				curframe = walkingstart;
			}
		}
	}


}
/******************************************************************************
*warpsprite function
*This funciton is used to control the warping behaviour of the snake
******************************************************************************/
void snake::warpsprite()
{
			//if the sprite is dying let it get carried away by the moving world
			if(!dying)
			{
				//clip and adjust for the x postion of the sprite
				//on the left
				if(x<0)
				{
					x=0;
					direction=1;

					
				}
				//on the right
				else if(x>SCREEN_W)
				{
					x=SCREEN_W;
					direction=-1;
				}
	
				//clip and adjust for the y postion of the sprite
				//on the top
				if(y<0)
				{
					y=SCREEN_H;
				}
				//on the bottom
				else if(y>SCREEN_H -height)
				{
					y=0;
				}
			}
			else //the sprites dying
			{
				//if so when they leave the screen they will die if injured...
				//clip and adjust for the x postion of the sprite
				//on the left
				if(x<0-width)
				{
					alive=false;
				}
				//on the right
				else if(x>SCREEN_W)
				{
					alive=false;
				}
			}
}
/******************************************************************************
*udpateroutine function
*This is the snake specific updateroutine 
******************************************************************************/
void snake::updateroutine()
{
	updatePosition();
	warpsprite();
	updateAnimation(direction);
}
/******************************************************************************
*attack function
*This function controls the actions of the snake when attacking 
******************************************************************************/
void snake::attacksprite()
{
	//set the snake to attack state
	state=1;
	
	//change the animcolumns to 5 
	animcolumns=5;
	
	attack(attacksound,225);
	//adjust the y position of the keyframe to compensate for the direction
	if(direction==1)
		animstarty=300;
	if(direction==-1)
		animstarty=200;
}
