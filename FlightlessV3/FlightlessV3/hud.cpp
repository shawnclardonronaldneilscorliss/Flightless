/******************************************************************************
*hud.cpp
*This is the Heads UP Display(HUD) class
*This class controls the creation, updating  and destruction of the HUDs
******************************************************************************/
#include "hud.h"
#include <allegro.h>
/******************************************************************************
*init hud function
*used to initalize the games heads up display
******************************************************************************/
void hud::inithud(BITMAP *drawto)
{

}
/******************************************************************************
*updatehud function
*used to draw and update the hud
******************************************************************************/
void hud::udpdatehud(BITMAP *drawto)
{
	
}

/******************************************************************************
*destroyhud function
*This function is used to kill the hud and free the memory used
******************************************************************************/
void hud::destoryhud()
{

}
void printendmessage(BITMAP *drawto,FONT *myfont,char *message)
{
	//message the player...even if they are a quiter!
	textprintf_ex(drawto,myfont,200,SCREEN_H/2,makecol(255,0,0),-1,message);
}