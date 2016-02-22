#include <allegro.h>
#include "sprite.h"

/******************************************************************************
*sprite constructor
*Establishes the default settings for a sprite 
******************************************************************************/
sprite::sprite() {
    image = NULL;
    alive = 1;		//sprite is alive when created by default
	dying=false; 
    direction = 0; //used to determine what direction the sprite is facing 
    animcolumns = 0;
    animstartx = 0; 
    animstarty = 0;
    x = 0.0f; 
    y = 0.0f;
    width = 0; 
    height = 0;
    xdelay = 0; 
    ydelay = 0;
    xcount = 0; 
    ycount = 0;
    velx = 0.0; 
    vely = 0.0;
    speed = 0.0;
    curframe = 0; 
    totalframes = 1;
    framecount = 0; 
    framedelay = 10;
    animdir = 1;	//establish animation to have positive animation by default
	hurtsound=NULL;
	deadsound=NULL;
	attacksound=NULL;
}
/******************************************************************************
*sprite destructor
*called when the sprite is destroyed this deletes the sprites bitmap image(GC)
******************************************************************************/
sprite::~sprite() {
    //remove bitmap from memory
    if (image != NULL)
        destroy_bitmap(image);
}
/******************************************************************************
*sprite load function
*uesd to load an image to make a sprite. Return 1 = working, 0 not working
******************************************************************************/
int sprite::load(char *filename) 
{
	//this function assumes that the image is located with the exe
	image = load_bitmap(filename, NULL);

	//sanity check of the image if the image is null return 0
	if (image == NULL) 
		return 0;
	
	//set the default height/width of the sprite to the height/width of the image
	width = image->w;
	height = image->h;

	//success
    return 1;
}
/******************************************************************************
*sprite draw function
*this function draws the sprite to the destination bitmap 
******************************************************************************/
void sprite::draw(BITMAP *dest) 
{
	//worker function draw_sprite being called from the allegro libary
	draw_sprite(dest, image, (int)x, (int)y);
}
/******************************************************************************
*sprite drawframe function
*this function uses the mask_blit function to draw the sprite to the 
*destination bitmap 
******************************************************************************/
void sprite::drawframe(BITMAP *dest)
{

	int fx = animstartx + (curframe % animcolumns) * width;
    int fy = animstarty + (curframe / animcolumns) * height;

    masked_blit(image,dest,fx,fy,(int)x,(int)y,width,height);

}
/******************************************************************************
*sprite updatePosition function
*This function is used to move the sprite on the x and y axis 
******************************************************************************/
void sprite::updatePosition()
{
	int oldxpos,oldypos;
	oldxpos=x;
	oldypos=y;
	
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

}
/******************************************************************************
*sprite updateAnimation function
*This function is a helper function for animation
******************************************************************************/
void sprite::updateAnimation() 
{
    //update frame based on animdir
    if (++framecount > framedelay)
    {
        framecount = 0;
		curframe += animdir;

		if (curframe < 0) {
            curframe = totalframes-1;
		}
		if (curframe > totalframes-1) {
            curframe = 0;
        }
    }
}
/******************************************************************************
*sprite inside function
*This function is used to test if a sprite is inside of another sprite
******************************************************************************/
int sprite::inside(int x,int y,int left,int top,int right,int bottom)
{
    if (x > left && x < right && y > top && y < bottom)
        return 1;
    else
        return 0;
}
/******************************************************************************
*sprite collided function
*this function establishes two points in space to perform intersection testing
******************************************************************************/
int sprite::collided(sprite *other, int shrink)
{
	
    //object width and height named a
	int wa = (int)x + width;
    int ha = (int)y + height;

	//object width and height named b
    int wb = (int)other->x + other->width;
    int hb = (int)other->y + other->height;

    if (inside((int)x, (int)y, (int)other->x+shrink, (int)other->y+shrink, wb-shrink, hb-shrink) ||
        inside((int)x, ha, (int)other->x+shrink, (int)other->y+shrink, wb-shrink, hb-shrink) ||
        inside(wa, (int)y, (int)other->x+shrink, (int)other->y+shrink, wb-shrink, hb-shrink) ||
        inside(wa, ha, (int)other->x+shrink, (int)other->y+shrink, wb-shrink, hb-shrink))
        return 1; //found that the two objects are intersecting
    else
        return 0; //no collision or intersection
}
/******************************************************************************
*warpsprite function
*This function takes a sprite, who is not dead or dying and warps the sprite 
******************************************************************************/
void sprite::warpsprite()
{
	if(objecttype==1)
	{
		//clip and adjust for the x postion of the sprite
		//on the left
		if(x<0 -width)
		{
			x=SCREEN_W;
		}
		//on the right
		else if(x>SCREEN_W)
		{
			x=0-width;
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
	if(objecttype==0)
		{
			//if the sprite is dying let it get carried away by the moving world
			if(!dying)
			{
				//clip and adjust for the x postion of the sprite
				//on the left
				if(x<0-width)
				{
					x=SCREEN_W;
				}
				//on the right
				else if(x>SCREEN_W)
				{
					x=0-width;
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
	}
/******************************************************************************
*update routine function
*This function is the main helper function that calls the sprites actions
******************************************************************************/
void sprite::updateroutine()
{

	updatePosition();
	warpsprite();
	updateAnimation();
}
/******************************************************************************
*isdying function
*This function takes a sound sample, sets actions and starts killing the sprite
******************************************************************************/
void sprite::isdying(SAMPLE *sound)
{
	//only call this function if the sprite is not already dying
	if(dying ==false)
	{
		//set dying to true
		dying=true;
		if(objecttype==0)
		{
			//move the next animation in the sprite sheet
			animstarty=20;
		}
		//set velocity of the dying bug
	
		//bug should move in the opposite direction and cannot move up
		direction=-1;

		//play the sound when the sprite is injured
		if(sound!=NULL)
		{
			play_sample(sound,255,125,500,false);
		}
	}
}
/******************************************************************************
*killbugfunction
*This function controls the behaviour when a bug dies
******************************************************************************/
void sprite::killsprite(SAMPLE *sound,int vol)
{
	//set dying to true
	alive=false;

	if(vol==NULL||vol==0)
		vol=255;
	//play the sound for when you kill the sprite
	if(sound!=NULL)
	{
		play_sample(sound,vol,125,1000,false);
	}
}
/******************************************************************************
*checkcollisions function
*This function is a helper function that sets behaviour for each collision
*There are 3 states, collision, being attacked and being killed.
******************************************************************************/
void sprite::checkcollisions(sprite* other)
{
	if(alive==1)
				{
					//check collisions if the owl is attacking hurt the sprite
					if(collided(other,0)&&other->state==1)
					{
						attack(other->attacksound,20);
						takedamage(other->damage);
						other->state=0; 
					}

					//if the sprite is dying and the player attacks = dead
					if(collided(other,0)&&other->state==1
						&& dying==true && curframe==0)
					{
						attack(other->attacksound,20);
						takedamage(other->damage);
						other->state=0; 
						
					}

					//check collisions to make the enemy run away 1st time
					if(collided(other,0))
					{
						changedirection();
					}
				
				}
}
/******************************************************************************
*changedirection function
*This is function is used to change the binary direction of the sprite
******************************************************************************/	
void sprite::changedirection()
{
	if(direction==1)
		direction=-1;
	else if(direction==-1)
		direction=1;

}
/******************************************************************************
*takedamage function
*This is function is used to deal damage to the player
******************************************************************************/		
void sprite::takedamage(int dmg)
{
	health = health-dmg;
}
/******************************************************************************
*takedamage function
*This is function is used to deal damage to the player
******************************************************************************/		
void sprite::heal(int pnts)
{
	if((health+pnts)>90)
		health=90;
	else
		health=health+pnts;
}
/******************************************************************************
*udpateScore function
*this is an setter method to update the player score
******************************************************************************/
void sprite::updatescore(int points)
{
	score=score+points;
}
/******************************************************************************
*track sprite function
*This function allows one sprite to follow another
*The hunter is assumed to be the caller of the function
******************************************************************************/
void sprite::tracksprite(sprite *pry)
{
	//move the hunter on the x axis
	if(pry->x >x)
		x++;
	else x--;
	
	//move the hunter on the y axis
	if(pry->y >y)
		y++;
	else y--;
}
/******************************************************************************
*isvisable function
*This function is used to test if sprites can see each other
******************************************************************************/
bool sprite::isvisable(sprite *spr)
{
	//return value for function indacating if the player is within view
	bool cansee=false;

	//if the player is not hiding
	if(spr->state!=2)
	{
		if(direction ==-1 &&spr->x<=x)
		{
			cansee=true;
		}

		if(direction==1 &&spr->x>=x)
		{
			cansee=true;
		}
	}
	return cansee;
}
/******************************************************************************
*attack function
*This function dictates the attack sound
******************************************************************************/
void sprite::attack(SAMPLE* sound,int vol)
{
	if(vol==NULL)
		vol=255;
	//play the sound for when you kill the sprite
	if(sound!=NULL)
	{
		play_sample(sound,vol,125,2000,false);
	}
}