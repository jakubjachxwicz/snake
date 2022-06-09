#include "header.h"


int main()
{
	std::srand(std::time(NULL));

	Win win;

	while (win.isOpen())
	{
		win.loop();
	}
}