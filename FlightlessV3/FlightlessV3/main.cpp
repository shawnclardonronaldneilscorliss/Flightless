/******************************************************************************
* Main.cpp file
* This file contains the main entrace to the applaction as well as containing
* main itself
******************************************************************************/

#include <allegro.h>
#include <stdio.h>
#include "Flightless_globals.h"
#include "sprite.h"
#include "player.h"
#include "spritehandler.h"
#include "mappyal.h"
#include "bug.h"
#include <time.h>
#include <pthread.h>
#include "snake.h"
	
//global variable declerations used in main function

//time tracking variables
int startTime, endTime, totalTime;
	
//create a new thread mutex to protect variables
pthread_mutex_t threadsafe = PTHREAD_MUTEX_INITIALIZER;

player *owl;
BITMAP *buffer;
spritehandler *sprhandler;
int numberofbaides=0;
	SAMPLE *pop=NULL;
	SAMPLE *squish=NULL;
bugs *bug[BADIES];
int old_ticks;
//deinfe a volatile integer named ticks which is used in the ticker function
volatile int ticks =0;
/******************************************************************************
*ticker function
*This function is to be called by the interrupt handler to increment ticks
******************************************************************************/
void ticker()
{
	ticks++;
}
END_OF_FUNCTION(ticker)

//deinfe a volatile integer named ticks which is used in the game time function
volatile int gametime =0;
/******************************************************************************
*game time ticker function
*This function is to be called by the interrupt handler to increment game time
******************************************************************************/
void game_time_ticker()
{
	gametime++;
}
END_OF_FUNCTION(game_time_ticker)
/******************************************************************************
*create_close function
*This is a final thankyou to the user for playing the game
******************************************************************************/
void create_close(BITMAP *buffer,FONT *myfont, player *me)
{
	//bitmap to load image
	BITMAP *finalimage;

	finalimage= load_bitmap("Flightlessfinish screen.tga",NULL);
	if(finalimage)
	{
			blit(finalimage,screen,0,0,0,0,900,600);
	}

	//print the players score
	textprintf_ex(screen,myfont,50,200,makecol(255,0,0),-1,"Your Survival Score is:");
	textprintf_ex(screen,myfont,350,200,makecol(255,0,0),-1," %d",me->getScore());

	textprintf_ex(screen,myfont,50,250,makecol(255,0,0),-1,"Your Kill Score is:");
	textprintf_ex(screen,myfont,280,250,makecol(255,0,0),-1," %d",me->getScore()*me->scoremodifier);

	//print the time in game
	textprintf_ex(screen,myfont,50,300,makecol(255,0,0),-1,"Total time in game is:");
	textprintf_ex(screen,myfont,50,330,makecol(255,0,0),-1,"%d mins, %d seconds",(((endTime-startTime)/60)%60),(endTime-startTime)%60);

	//print the bugs that got away

	//give the user time to look at the image
	rest(4000);

}
//this thread updates sprite 0
void* playerdraw_thread(void* data)
{
    //get this thread id
    int my_thread_id = *((int*)data);

    //thread's main loop
    while(!GAMEOVER)
    {
        //lock the mutex to protect variables
        if (pthread_mutex_lock(&threadsafe))
            textout_ex(buffer,font,"ERROR: thread mutex was locked",0,0,WHITE,0);
		
		if(SHOWHEALTH==true)
		{		
			textprintf_ex(buffer,font,owl->x,
				owl->y-20,makecol(255,0,0),-1,"%d",owl->health);
		}
			//call the owl draw function
			owl->drawframe(buffer);

        //unlock the mutex
        if (pthread_mutex_unlock(&threadsafe))
            textout_ex(buffer,font,"ERROR: thread mutex unlock error",0,0,WHITE,0);

    }

    // terminate the thread
    pthread_exit(NULL);

    return NULL;
}
//this thread updates sprite 0
void* wormdraw_thread(void* data)
{
    //get this thread id
    int my_thread_id = *((int*)data);

    //thread's main loop
    while(!GAMEOVER)
    {
		if(old_ticks>ticks)
		{
        //lock the mutex to protect variables
        if (pthread_mutex_lock(&threadsafe))
            textout_ex(buffer,font,"ERROR: thread mutex was locked",0,0,WHITE,0);
		
		if(sprhandler->getbugs(my_thread_id)->alive==true)
		{
			//draw the sprites
			sprhandler->getbugs(my_thread_id)->drawframe(buffer);
			if(SHOWHEALTH==true)
			{
				textprintf_ex(buffer,font,sprhandler->getbugs(my_thread_id)->x,
					sprhandler->getbugs(my_thread_id)->y-5,
					makecol(255,0,0),-1,"%d",sprhandler->getbugs(my_thread_id)->health);
			}
		}

        //unlock the mutex
        if (pthread_mutex_unlock(&threadsafe))
            textout_ex(buffer,font,"ERROR: thread mutex unlock error",0,0,WHITE,0);
		}
    }

    // terminate the thread
    pthread_exit(NULL);


    return NULL;
}
//this thread updates the snakes
void* snakedraw_thread(void* data)
{
    //get this thread id
    int my_thread_id = *((int*)data);

    //thread's main loop
    while(!GAMEOVER)
    {
		if(old_ticks>ticks)
		{
        //lock the mutex to protect variables
        if (pthread_mutex_lock(&threadsafe))
            textout_ex(buffer,font,"ERROR: thread mutex was locked",0,0,WHITE,0);

		//only draw the snake if it is alive
		if(sprhandler->getsnakes(0)->alive==true)
		{
			if(SHOWHEALTH==true)
			{
				textprintf_ex(buffer,font,sprhandler->getsnakes(0)->x,
					sprhandler->getsnakes(0)->y-5,
					makecol(255,0,0),-1,"%d",sprhandler->getsnakes(0)->health);
			}
			//draw the sprites
			sprhandler->getsnakes(0)->drawframe(buffer);
		}
				

        //unlock the mutex
        if (pthread_mutex_unlock(&threadsafe))
            textout_ex(buffer,font,"ERROR: thread mutex unlock error",0,0,WHITE,0);
		}
    }

    // terminate the thread
    pthread_exit(NULL);


    return NULL;
}
int main(int argc, char *argv[])
{//start of main function

	//set gameover = true to stop the game from loading before the player plays
	GAMEOVER=true;

	//variable declerations
	int id;

	//variables used for the threads
	pthread_t pthread0;
    pthread_t pthread1;
    int threadid0 = 0;
	int threadid1 = 1;
	int threadid2 = 2;
	int threadid3 = 3;
	int threadid4 = 4;
	int threadid5 = 5;
	int threadid6 = 6;


	//set is intro to true to play the intro
	bool inIntro =true; 

	//variable used to display the players message
	char *playermessage;

	//map variables 
	int mapxoff = 0;
	int mapyoff = 0;

	//game speed variables declerations and initalization
	int frames_per_sec=0;
	int frames_done=0;
	int old_time =0;
	int totaltime=0;

	//declare allegro variables
	//BITMAP *buffer;
	BITMAP *intro;

	//initalize the classes
	//player *owl;
	sprite *hearts[3];
	sprhandler = new spritehandler();
	snake *snakeboss;

	//initalize allegro
	allegro_init();
	install_keyboard();
	install_timer();
	set_keyboard_rate(1000,1000);
	install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL);

	//initalize the screen
	set_color_depth(COLORDEPTH);
	set_gfx_mode(MODE,WIDTH,HEIGHT,0,0);

	//lock both the varibale and the funciton to be used by the interrupt handler
	LOCK_VARIABLE(ticks);
	LOCK_FUNCTION(ticker);
	//install the interrupt handler which calls the ticker 60 times a second
	install_int_ex(ticker,BPS_TO_TIMER(FPS));
	
	//lock both the varibale and the funciton to be used by the interrupt handler
	//these 
	LOCK_VARIABLE(gametime);
	LOCK_FUNCTION(game_time_ticker);
	
	//install the interrupt handler which calls the game time ticker
	//game time ticker is in 10ths of seconds
	install_int_ex(game_time_ticker,BPS_TO_TIMER(10));

	//create back buffer
    buffer = create_bitmap(SCREEN_W, SCREEN_H);

	//create the intro screen by loading the bitmap into the intro buffer
	intro = load_bitmap("flightless title.bmp",NULL);
	if(!intro){
		allegro_message("Error loading intro image");
		return 1;
	}

	//seed the random function
	srand(time(NULL));
	
	//load fonts
	FONT *dicot_22_font;
	PALETTE palette;
	
	//load the the font for the header of the application
	dicot_22_font = load_font("dicot_22.pcx", palette, NULL);
	
	//load and display the intro screen
	while(inIntro)
	{
		//display the intro screen for the game
		blit(intro,screen,0,0,0,0,SCREEN_W,SCREEN_H);
		
		//wait for the player to press enter to start the game
		if(key[KEY_ENTER]){
			inIntro=false;
			GAMEOVER=false;
		}
		
		//if the player presses esc then exit the game and the intro
		if(key[KEY_ESC]){
			inIntro=false;
			GAMEOVER=true;
		}
	}
	//for as many badies make sprites to hold the bugs
	for(int i=0;i<BADIES;i++)
	{
			//initalize the bug sprite
			 bug[i] = new bugs();

			//load the bug sprite sheet
			if(!bug[i]->load("bug.bmp")){
			allegro_message("Error loading bug sprite");
			return 1;
			}

			//initalize the bug propertys
			bug[i]->init();
			
			//add sprites to the sprite handler
			sprhandler->addbugs(bug[i]);
	}
	//initalize the player
	owl = new player();

	//load the players sprite sheet
	if(!owl->load("owl sprite_new.bmp")){
		allegro_message("Error loading owl sprite");
		return 1;
	}
	
	//initalize the sprite properties
	owl->init();

	//add the player object to the sprite handler
	sprhandler->addsprite(owl);

	//create the new snake object
	snakeboss = new snake();

	//load the snake
	if(!snakeboss->load("snake_Tiles.bmp")){
		allegro_message("Error loading snake sprite");
		return 1;
	}
	//initalize the snake
	snakeboss->init();
	//snakeboss->alive=false;

	//add the snake to the sprite handler
	sprhandler->addboss(snakeboss);

	//create for loop to load the players health indicator
	for(int i=1;i<4;i++)
	{
		//create new sprite objects to hold the images
		hearts[i-1] = new sprite();

		//load the players health image
		if(!hearts[i-1]->load("FullHealth.bmp")){
			allegro_message("Error loading player Health");
			return 1;
		}
			//set sprite properties
			hearts[i-1]->x = SCREEN_W-35*i;
			hearts[i-1]->y = 10;
			hearts[i-1]->width = 30;
			hearts[i-1]->height = 30;
			hearts[i-1]->velx = 0;
			hearts[i-1]->vely = 0;
			hearts[i-1]->animcolumns = 0;
			hearts[i-1]->curframe = 0;
			hearts[i-1]->totalframes =1;
			hearts[i-1]->animdir = 0;
			//object type of non moving sprite
			hearts[i-1]->objecttype=2;
			hearts[i-1]->alive=0;

		//add the players health to the sprite handler
		sprhandler->addsprite(hearts[i-1]);
	}
	

	//create the music samples
	SAMPLE *ingamesong;

	//load the wav file into the samples
	//main game soundtrack
	ingamesong=load_sample("melo-24_Clip.wav");
	if(ingamesong==NULL){
			allegro_message("Error loading game soundtrack");
			return 1;
	}

	//use mappy.h global function MapLoad to load the .FMP file
	MapLoad("flightless.FMP");
	
	//play the sample music
	int playing = play_sample(ingamesong,150,125,1000,true);

	//start the clock to time the player
	startTime = time(NULL);

	//create the thread for player
    id = pthread_create(&pthread0, NULL, playerdraw_thread, (void*)&threadid5);

	//create the thread for the bugs
	id = pthread_create(&pthread1, NULL, wormdraw_thread, (void*)&threadid0);
	id = pthread_create(&pthread1, NULL, wormdraw_thread, (void*)&threadid1);
	id = pthread_create(&pthread1, NULL, wormdraw_thread, (void*)&threadid2);
	id = pthread_create(&pthread1, NULL, wormdraw_thread, (void*)&threadid3);
	id = pthread_create(&pthread1, NULL, wormdraw_thread, (void*)&threadid4);

	//create the thread to draw the snake
	id = pthread_create(&pthread1, NULL, snakedraw_thread, (void*)&threadid0);

	//main game loop
	while (!GAMEOVER)
	{		

		while (ticks==0)
		{
			rest(1);
		}
		while(ticks>0)
		{
			if(key[KEY_ESC])
			{
				playermessage="You quit :( Please come back again.";
				GAMEOVER=true;
			}
			
			//get the previous value of ticks and store them
			old_ticks=ticks;

			//each time we hit this loop we increment the mapxoffset +1 			
			if(mapxoff<mapwidth*mapblockwidth)
			{
				//if the player is not hiding update the map
				if(owl->state!=2)
				{
					mapxoff++;
				}
			}
			//draw the background tiles via the mappy global funciton MapDrawBG
			MapDrawBG(buffer,mapxoff,0,0,0,WIDTH-1,HEIGHT-1);
	
			//draw the players health indicator based on the owls health/30
			for (int i=0;i<((owl->health)/30);i++)
			{
				hearts[i]->draw(buffer);
			}
			
			//lock the varibales to protect them from the unsynced threads
			pthread_mutex_lock(&threadsafe);

			//get the player input
			owl->getinput();	

			sprhandler->updatebugs();
			sprhandler->updatesnake();

			//decrement the ticks
			ticks--;
			
			//unlock the mutex
			pthread_mutex_unlock(&threadsafe);
			//if the logic is taking too long abort and draw the screen
			if(old_ticks<=ticks)
				break;
		}
		//for every second that passess we need to call this function
		if(gametime - old_time >= 10)
		{
			//frames_per_sec holds the number of frames drawn in the last sec
			frames_per_sec = frames_done;

			//reset these variables for the next second
			frames_done =0;
			old_time=gametime;
		}
		//Decrement the players health
		if(gametime%120==0)
		{
 			owl->takedamage(2);
		}
		if(sprhandler->getsnakes(0)->alive==false && gametime%400==0)
		{
			sprhandler->getsnakes(0)->resurrect();
		}
		//check players health
		if(owl->health<=0)
		{
			playermessage="You have died...please remeber to eat next time.";
			GAMEOVER=true;
		}
		//stop the window from scrolling
		if(GAMEOVER==true ||mapxoff>=mapwidth*mapblockwidth-WIDTH)
		{
			if(mapxoff>=mapwidth*mapblockwidth-WIDTH)
				playermessage="Great job player you passed the game! Way to go.";
			//stop the map from moving
			mapxoff=mapwidth*mapblockwidth-WIDTH;
			
			//stop the music from playing
			stop_sample(ingamesong);

			//create the music samples
			SAMPLE *cheer=NULL;
	
			//load the wav file into the sample
			cheer=load_sample("app.wav");
			if(cheer==NULL){
					allegro_message("Error loading game soundtrack");
					return 1;
				}

			//play the sample music
			play_sample(cheer,255,125,1000,true);

			//message the player...even if they are a quiter!
			textprintf_ex(screen,dicot_22_font,200,SCREEN_H/2,makecol(255,0,0),-1,playermessage);

			rest(2000);
 	  	 	GAMEOVER=true;
		}
		//show frames fer second on the screen by pressing "cntrl f"
		if(key[KEY_F])
		{
			if(key_shifts & KB_CTRL_FLAG)
			{
				//switch the showFPS flag to display the FPS
				SHOWFPS=!SHOWFPS;
			}
			//SHOWFPS=SHOWFPS;
		}
		if(SHOWFPS==true)
			textprintf_ex(buffer,font,20,20,WHITE,0,"frames= :%d",frames_per_sec);

		if(key[KEY_H])
		{
			if(key_shifts & KB_CTRL_FLAG)
			{
				//switch the SHOWHEALTH flag to display the FPS
				SHOWHEALTH=!SHOWHEALTH;
			}
			//SHOWHEALTH=SHOWHEALTH;
		}
		
      
		//update the screen using a double buffering technique
		acquire_screen();

		blit(buffer,screen,0,0,0,0,WIDTH,HEIGHT);
		release_screen();
		
		//everytime we bilt from buffer to screen increase frames_done
		frames_done++;
    }
	
	//find the time that the game ended
	endTime = time(NULL);

		//kill the mutex (thread protection)
    pthread_mutex_destroy(&threadsafe);

	//call to create the final image to present to the user
 	create_close(buffer,dicot_22_font,owl);
	
	//Garbage Collection clean up all of the objects that are being used in memory
	remove_keyboard();
	remove_timer();
	
	//destory the bitmaps that we are using
	destroy_bitmap(buffer);
	destroy_bitmap(intro);

	//destroy the map
	MapFreeMem();

	//delete the sprites
	delete sprhandler;

	//call to allegro exit
	allegro_exit();
	return 0;

}//end of main
END_OF_MAIN()