#ifndef _BUG_H
#define _BUG_H 1
#include "sprite.h"

class bugs: public sprite
{

	protected:
	public:
		
		//default constructor for the bug sprite type
		void init();
		//function to resurect the bugs
		void resurrect();

};
#endif