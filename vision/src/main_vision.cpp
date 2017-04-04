/*
 * main.cpp
 *
 *  Created on: Mar 27, 2017
 *      Author: sidney
 */

#include <thread>

#include "vision.hpp"
#include "interface.hpp"

Vision vision = Vision();
Interface interface = Interface();

bool running = true;

void fn();

int main(int argc,char* argv[])
{

	vision.initialize("robots_application", 0, true);

	thread live_feed(fn);

	while(running)
	{
		get_coordinates();

	}

	live_feed.join();

	return 0;
}

pair<Properties, Properties> get_coordinates()
{
	Properties object_properties;
	Properties target_properties;

	if(interface.await_input(true) == 1)
	{
		running = false;
	}
	else
	{
		vision.take_frame(true);
		uint8_t size = vision.number_selection(interface.get_specification());

		if(size != 0)
		{
			if(interface.await_input(false) == 1)
			{
				running = false;
			}
			else
			{
				if(interface.get_specification() <= size)
				{
					object_properties = vision.shape2grab(interface.get_specification());

					cout << "height of selected object = " << vision.shape2grab(interface.get_specification()).height << endl;

					target_properties = vision.get_properties(vision.detect_shape(vision.filter_colour(WHITE), CIRCLE).at(0).first);

					vision.transform_properties(&object_properties);
					vision.transform_properties(&target_properties);

					cout << "object coordinates are: x: " << object_properties.center.x << " - y: " << object_properties.center.y << endl;
					cout << "object orientation = " << object_properties.angle << endl;
					cout << "target coordinates are: x: " << target_properties.center.x << " - y: " << target_properties.center.y << endl;
				}
			}
		}
		else
		{
			cout << "did not find any grabbable shapes in that colour, please try again" << endl << endl;
		}
	}
	return make_pair(object_properties, target_properties);
}

void fn()
{
	while(running)
	{
		vision.show_image();
	}
}


