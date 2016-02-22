#ifndef _HUD_H
#define _HUD_H 1

#include <allegro.h>

class hud{
private:
public:
	//used to initalize the games heads up display
	void inithud(BITMAP *drawto);
	//used to draw and update the hud
	void udpdatehud(BITMAP *drawto);
	//used to kill the hud and free it from memory
	void destoryhud();
	//print the final message to the player
	void printendmessage(BITMAP *drawto);
};
#endif