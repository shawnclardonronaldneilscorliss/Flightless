#ifndef _SNAKE_H
#define _SNAKE_H 1
#include "sprite.h"

class snake: public sprite
{

	protected:
	public:
		int walkingstart;
		//default constructor for the snake sprite type
		void init();
		//override the warpsprite function
		void warpsprite();
		
		//function used to control dying behvaviour
		void isdying(SAMPLE *sound);
		void killbug(SAMPLE *sound);
		//function to resurect the snake
		void resurrect();
		//this function is an overloaded sprite version that uses direction
		void updateAnimation(int direction);
		//routine that the snake uses to update itself
		void updateroutine();
		//snake specific attack annimations 
		void attacksprite();
};
#endif