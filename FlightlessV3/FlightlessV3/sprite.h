#ifndef _SPRITE_H
#define _SPRITE_H 1

#include <allegro.h>

class sprite {
private:
public:
		//class variables
		bool alive;
		
		/*
		****Available states are:
		*	state=0...walking
		*	state=1...attacking
		*	state=2...hiding
		*/
		int state;
		int objecttype;

		//sounds played by the sprites
		SAMPLE *hurtsound;
		SAMPLE *deadsound;
		SAMPLE *attacksound;

		//intigers detailing health, damage and score
		int damage;
		int health;
		int score;
		int scoremodifier;

		//is the sprite waiting to be eaten?
		bool dying;

		//current facing or direction of the sprite
		int direction;
		
		//current x and y position of the sprite
		double x,y;
		
		//height and width of the acutal sprite
		int width, height;
		
		//the x and y velocity of the sprite
		double velx,vely;
		
		//the delay of velocity/speed in the x and y direction
		int xdelay,ydelay;
		int xcount, ycount;
		double speed;
		
		//helpers for animation
		int curframe,totalframes,animdir;
		int framecount,framedelay;
		int animcolumns;
		int animstartx,animstarty;
		BITMAP *image;

	//class methods
		//constructor and destructor
		sprite();
		~sprite();
		//loader
		int load(char *filename);
		//drawer
		void draw(BITMAP *dest);
		void drawframe(BITMAP *dest);
		//mover
		virtual void updatePosition();
		//generic sprite animator function
		virtual void updateAnimation();
		//collision detector methods
		int pointInside(int px, int py);
		int collided(sprite *other,int shrink);
		int inside(int x, int y,int left,int top,int right,int bottom);
		//warp sprite from bottom to top and left to right ...or right to left
		void warpsprite ();
		//routine used to update sprites...helper function
		void updateroutine();
		//kill sprite function
		void killsprite(SAMPLE *sound,int vol);
		//start the dying process
		void isdying(SAMPLE* sound);
		//used to control attack sounds and behvaviour
		void attack(SAMPLE* sound,int vol);
		//checkcollisions is used to control some of the collision behaviour
		void checkcollisions(sprite *other);
		//used to take health from a sprite
		void takedamage(int dmg);
		//used to add health back to a sprite
		void heal(int pnts);
		//update the score the sprites 
		void updatescore(int points);
		
		//used to track sprites
		void tracksprite(sprite *pry);
		
		//used to see if the sprites can see the player
		bool isvisable(sprite *spr);

		//function used to change the binary direction
		void changedirection();
};
#endif