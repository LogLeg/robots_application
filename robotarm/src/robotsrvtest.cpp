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

void mySleep(unsigned long milliseconds)
{
#ifdef _WIN32
	Sleep(milliseconds); // 100 ms
#else
	usleep(milliseconds * 1000); // 100 ms
#endif
}

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




		//(1) vind blokje positie&rotatie
//		const int objectX = -83; 	//object X in mm
//		const int objectY = 300;	//object Y in mm
//		const int objectangle = 0;	//object angle in degrees
//		const int objectwidth = 15;	//object windth in mm
//		const int circelX = 160;	//circel center X
//		const int circelY = 250;	//circel center Y



	} catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}

void move(pair<Properties, Properties> robotworld)
{
	if (robotArm.moveObject(robotworld.first.center.x, robotworld.first.center.y,
		robotworld.first.angle, robotworld.first.width, robotworld.second.center.x, robotworld.second.center.y))
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

	object_properties.height = 999;
	target_properties.height = 999;

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
			vision.number_selection(interface.get_specification());
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
					cout << "first after specification" << endl;
					object_properties = vision.shape2grab(interface.get_specification());
					cout << "second after specification" << endl;

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
						//vision.transform_properties(&vision.calibration_square_properties);
						vision.transform_properties(&target_properties);

						//object_properties = vision.calibration_square_properties;

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
	//cout << "fn start" << endl;
	//while(true)
	//{
	//	vision.show_image();
		//cout << "dit zou vaak voorbij moeten komen" << endl;
	//}
	//for(int i = 0; i < 100; i++)
	//{
	//	cout << i << endl;
	//}
	pair<Properties, Properties> objects;

	while(running)
	{
		objects = get_coordinates();
		if(objects.first.height != 999 || objects.second.height != 999)
		{
			move(objects);
		}
	}
	
	//cout << "fn end" << endl;
}

/*
 * blokje opppakken:
 * (1) vind blokje positie&rotatie
 * (2) base&gripper goed roteren & gripper openen
 * (3) ga naar 2 cm boven blokje & gripper naar beneden richten
 * (4) ga naar beneden & gripper dichtknijpen
 * (5) ga naar 2 cm boven grond
 */

/*
 * Blokje neerleggen:
 * (1) vind de cirkel
 * (2) base roteren
 * (3) ga naar 2cm boven cirkel
 * (4) zakken naar grond & gripper loslaten
 * (5) ga naar ready positie
 */

/*
 * paden achter elkaar plakken voor concurrency
 * unit-testen
 * controleren of gripper bij blokje kan komen
 */
