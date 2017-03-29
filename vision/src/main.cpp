/*
 * main.cpp
 *
 *  Created on: Mar 27, 2017
 *      Author: sidney
 */

#include "vision.hpp"
#include "interface.hpp"

Vision vision = Vision();
Interface interface = Interface();

bool running = true;

int main(int argc,char* argv[])
{
	vision.initialize("robots_application", 0, true);

	/*while(true)
	{
		vision.show_image();
	}*/

	while(running)
	{
		if(interface.await_input(true) == 1)
		{
			running = false;
		}
		else
		{
			//vision.take_frame(true);
			vision.show_image();

			/*
			if(vision.detect(interface.get_specification(), false))
			{
				cout << "detected the shape!" << endl;
			}*/
		}
	}

	return 0;
}


