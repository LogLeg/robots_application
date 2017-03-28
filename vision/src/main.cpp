/*
 * main.cpp
 *
 *  Created on: Mar 27, 2017
 *      Author: sidney
 */

#include "vision.hpp"

Vision vision = Vision();

int main(int argc,char* argv[])
{
	vision.initialize("robots_application", 0, true);

	while(true)
	{
		vision.show_image();
	}
	return 0;
}


