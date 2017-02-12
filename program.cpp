#include <iostream>

#include "Game\controller.h"

int main()
{

	Controller c;

	
	c.Init(); // init
	c.Run(500); // Run the games
	c.Run(1000); // Run the games
	c.Run(2000); // Run the games
	c.Run(4000); // Run the games
	c.Run(8000); // Run the games
	c.FinalScore(); // Print final scoreboard

	// Allow reading
	std::cout << "Press anykey to exit";
	getchar();
}