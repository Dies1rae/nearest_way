#include "game.h"

int main() {
	Screen scr;
	system("clear");
	Game g_main(scr);


	/*
	  TEST PATTERN
	     ###
	      @#
		 ###
	 */
	g_main.putStart({10, 45});
	g_main.putEnd({17, 77});

	g_main.putWall({9, 45});
	g_main.putWall({11, 45});
	
	g_main.putWall({9, 46});
	g_main.putWall({11, 46});
	g_main.putWall({10, 46});
	
	g_main.putWall({9, 44});
	g_main.putWall({11, 44});
	g_main.run();
	
	return 0;
}
