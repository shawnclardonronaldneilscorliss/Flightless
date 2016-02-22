#ifndef _PLAYER_H
#define _PLAYER_H 1
#include "sprite.h"

class player: public sprite
{
	protected:

	public:
		//player variables
		
		//used to keep the player in the walking animation loop
		int walkingstart;

		//input delays to stop the player from spamming the spacebar
		int key_count;
		int key_delay;

		//default constructor for the player
		void init();

		//health accessor methods
		void sethealth(float init_health);
		
		//polymorphic inhertance and player specific update animation function
		void updateAnimation(int nextdir);
		//polymorphic inhertance and player specific movement function
		void updatePosition();
		//tests the players input for contolling the player sprite
		void getinput();
		
		//score accessor method
		//updates the score of the player
		void updateScore();
		//retrive the score
		int getScore();
		//sets starting score
		void setScore();
};
#endif
