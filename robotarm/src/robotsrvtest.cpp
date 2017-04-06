/*
 * main.cpp
 *
 *  Created on: 30 mrt. 2017
 *      Author: stefan
 */
#include "ros/ros.h"
#include "robotarm/Robot_GoTo.h"
#include "robotarm/Robot_Set_Ready.h"
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <thread>
#include <boost/thread/thread.hpp>

#include "RoboticArm.hpp"
#include "AStar.hpp"
#include "Size.hpp"
#include "Point.hpp"
#include "vision.hpp"
#include "interface.hpp"

// OS Specific sleep
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

Vision vision = Vision();
Interface interface = Interface();
RoboticArm robotArm(70, 146, 187, 115, Servo
		{ -90, 90 }, Servo
		{ -30, 90 }, Servo
		{ 0, 135 }, Servo
		{ -90, 90 }, Servo
		{ -90, 90 }, Servo
		{ 0, 30 });

bool running = true;

pair<Properties, Properties> get_coordinates();
void fn();


int main(int argc, char **argv)
{
	try
	{
		ros::init(argc, argv, "RotboArmBesturing_client");

		robotArm.setConf(std::vector<signed short>
		{ 0, -30, 110, 90, 0, 5 }); //{ 0, -30, 110, 90, 0, 5 }); //{ 0, 0, 0, 0, 0, 0 });
		vision.initialize("something", 1, false);
		boost::thread thread_b(fn);
		

		while(running)
		{
			vision.show_image();
			
			
		}

		thread_b.join();


	} catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}

void move(pair<Properties, Properties> robotworld)
{
	if (robotArm.moveObject(static_cast<signed long>(robotworld.first.center.x), static_cast<unsigned long>(robotworld.first.center.y),
			static_cast<signed short>(robotworld.first.angle), static_cast<unsigned short>(5), static_cast<signed long>(robotworld.second.center.x), static_cast<unsigned long>(robotworld.second.center.y)))
	{
	}else{
		std::cerr << "\033[1;31mMove object failed\033[0m\n" << std::endl;
	}
	robotArm.gotoPark();
}

pair<Properties, Properties> get_coordinates()
{
	Properties object_properties;
	Properties target_properties;
	uint8_t counter = 0;

	object_properties.height = 0;
	target_properties.height = 0;

	if(interface.await_input(true) == 1)
	{
		running = false;
	}
	else
	{
		vision.take_frame(true);
		uint8_t size = vision.number_selection(interface.get_specification());

		while(size == 0 && counter < 10)
		{
			vision.take_frame(true);
			size = vision.number_selection(interface.get_specification());
			counter++;
		}

		counter = 0;

		if(size > 0)
		{
			if(interface.await_input(false) == 1)
			{
				running = false;
			}
			else
			{
				if(interface.get_specification() < size)
				{
					object_properties = vision.shape2grab(interface.get_specification());

					cout << "height of selected object = " << vision.shape2grab(interface.get_specification()).height << endl;

					vector<pair<vector<Point>, vector<vector<Point>>>> circles =  vision.detect_shape(vision.filter_colour(WHITE), CIRCLE);

					while(circles.size() == 0 && counter < 10)
					{
						vision.take_frame(true);
						circles = vision.detect_shape(vision.filter_colour(WHITE), CIRCLE);
						counter++;
					}

					if(circles.size() != 0)
					{
						target_properties = vision.get_properties(circles.at(0).first);
						cout << "third after specification" << endl;

						vision.transform_properties(&object_properties);
						vision.transform_properties(&target_properties);

						cout << "object coordinates are: x: " << object_properties.center.x << " - y: " << object_properties.center.y << endl;
						cout << "object orientation = " << object_properties.angle << endl;
						cout << "target coordinates are: x: " << target_properties.center.x << " - y: " << target_properties.center.y << endl;
					}
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
	pair<Properties, Properties> objects;

	while(running)
	{
		objects = get_coordinates();
		if(objects.first.height != 0 || objects.second.height != 0)
		{
			move(objects);
		}
	}
}
