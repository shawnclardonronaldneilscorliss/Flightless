#ifndef _Flightless_globals
#define _Filghtless_globals 1

#include <allegro.h>

//define common colors
#define WHITE makecol(255,255,255)
#define BLACK mackcol(255,255,255)

//define montior values
#define MODE GFX_AUTODETECT_FULLSCREEN

#define HEIGHT 600 
#define WIDTH 800
#define COLORDEPTH 32

//number of enimies
#define BADIES 5

//define frames persecond goal
#define FPS 120

//global variables 
//value for ending the game
bool GAMEOVER=false;
//variables for the HUD
bool SHOWFPS=false;
bool SHOWHEALTH=false;

#endif