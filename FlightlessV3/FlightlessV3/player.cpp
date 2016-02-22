#include "player.h"
#include <allegro.h>
 /*****************************************************************************
 *Player.cpp file
 *This class (player class) inheritances from the sprite class.
 *This is needed as much of the players base functionality is that of a sprite
 *****************************************************************************/

/******************************************************************************
*player initalizeation function
*This is the player constructor that is called when a new player is created
*This function has no parameters or retrun statments
******************************************************************************/
void player::init()
{		
	//set sprite properties
        x = SCREEN_W/2;
        y = 400;
        width = 100;
        height = 100;
        velx = 2;
        vely = 2;
		animcolumns = 10;
        curframe = 0;
		totalframes = 10;
        animdir = 1;
		walkingstart=5;
		//object type of player
		objecttype=1;
		health=90;
		setScore();
		damage=1;
		key_count=0;
		key_delay=1;
		scoremodifier=0;
		attacksound= load_sample("Hammering.wav");
}

/******************************************************************************
*sethealth function
*This is an accessor (setter) method to set the players health used in 
*initalization
******************************************************************************/		
void player::sethealth(float init_health)
{
	if(init_health>90)
		health=90;
	else
		health=init_health;
}
/******************************************************************************
*player instance of updateAnimation function
*This function is a helper function for the player animation
*Inheritated from the sprite class the player version uses direction
******************************************************************************/
void player::updateAnimation(int nextdir) 
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
*player updatePosition function
*Inheritated from the sprite class the player version establishes boundries
******************************************************************************/
void player::updatePosition()
{
    //update x position
    if (++xcount > xdelay)
    {
        xcount = 0;
        x += velx * direction;
    }

    //update y position
    if (++ycount > ydelay)
    {
        ycount = 0;
        y += vely * direction;
    }
	//establish boundries for the player
	//on the y axis
	if(y<=311)
		y=311;
	if(y>=SCREEN_H-height)
		y=SCREEN_H-height;
	
	//on the x axis
	if(x<=0)
		x=0;
	if(x>=SCREEN_W-width)
		x=SCREEN_W-width;
}
/******************************************************************************
*getplayerinput function
*This function controls what events happen to the player based on input
******************************************************************************/
void player::getinput()
{
	//update positions and animations
	//dig to hide
	if(key[KEY_D])
	{
		//the player cannot move during the dig
		vely=0;
		velx=0;
		//state 2 = hide
		state=2;
		//change to use the dig animation
		animstarty=455;
		//set update animation to 0 as this will reset the walking frames
		updateAnimation(0);

		updatePosition();
		//set walking state to the last key frame in the dig animation
		walkingstart=9;
	}
	//move the player right
	if(key[KEY_RIGHT])
	{
		framedelay=10;
		velx=3;
		vely=0;
		state=0;
		animstarty=0;
		updateAnimation(1);
		updatePosition();
		walkingstart=5;

	}
	//move the player left
	if(key[KEY_LEFT])
	{
		framedelay=10;
		velx=4;
		vely=0;
		state=0;
		animstarty = 100; 
		updateAnimation(-1);
		updatePosition();
		walkingstart=5;
	}
	//move the player up on the screen
	if(key[KEY_UP])
	{
		framedelay=10;
		vely=1;
		velx=0;
		state=0;
		animstarty=0;
		updateAnimation(-1);
		updatePosition();
		walkingstart=5;
	}
	//move the player down in the screen
	if(key[KEY_DOWN])
	{
		framedelay=10;
		direction=1;
		vely=1;
		velx=0;
		state=0;
		animstarty = 100;
		updateAnimation(1);
		updatePosition();
		walkingstart=5;
	}
				//attack happend 
	if(key[KEY_SPACE])
	{
			key_count=0;
			state=1;
			//speed up the animation
			framedelay=0;
			//reduce the velocity of the player so it dosnt move
			vely=0;
			velx=0;
		//if the player is facing left
		if(direction==-1)
		{
			//map to the start position on the y axis 
			//of the first frame for the attack animation
			animstarty = 320;
			//run the animation
			updateAnimation(-1);
		}
				
		//else the player is facing right
		else
		{
			//map to the start position on the y axis 
			//of the first frame for the attack animation
			animstarty = 210;
			//runthe animation
			updateAnimation(1);
		}

	}
}
/******************************************************************************
*getScore function
*Since score is private an accessor method is needed to look/ inspect the int
*This function returns an int and has no parameters
******************************************************************************/
int player::getScore()
{
	return score;
}
/******************************************************************************
*setScore function
*this is an setter method to initalize the score of the player
******************************************************************************/
void player::setScore()
{
	score=0;
}
