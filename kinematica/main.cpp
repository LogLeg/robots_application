/*
 * main.cpp
 *
 *  Created on: 30 mrt. 2017
 *      Author: stefan
 */
#include <iostream>
#include <cmath>
#include "RoboticArm.hpp"

struct robotInput	{
	unsigned long x;
	signed short robotAngle;
	signed short gripperAngle;
	signed short objectWidth;
};

/*
 * @brief convert vision data to x & angle
 * @param x x value from base
 * @param y y value from base
 * @param objectAngle angle of the object
 * @param objectWidth width of the object
 */
robotInput convertToRobotInput(signed long x, unsigned long y, unsigned short objectAngle, unsigned short objectWidth)	{
	robotInput robot;
	robot.x = sqrt(pow(x,2) + pow(y,2));
	robot.robotAngle = round(std::asin(double(x)/double(robot.x)) * 180.0 / M_PI);
	robot.objectWidth = objectWidth;
	robot.gripperAngle = -robot.robotAngle + objectAngle + 0; // TODO: evt. +90
	return robot;
}

int main(int argc, char **argv) {

	RoboticArm robotArm(1,146,187,86); //TODO: hoogte a opmeten.
	robotArm.setConf(Configuration{0,0,0,0,0,0});

	//(1) vind blokje positie&rotatie
	robotInput input = convertToRobotInput(-100, 200, 20, 20);
	std::cout << "x: " << input.x << std::endl;
	std::cout << "robotAngle: " << input.robotAngle << std::endl;
	std::cout << "gripperAngle: " << input.gripperAngle << std::endl;
	std::cout << "objectWidth: " << input.objectWidth << std::endl;


	//(2) base&gripper goed roteren & gripper openen
	std::cout << "pad naar 6" << std::endl;
	robotArm.printPath(robotArm.calculatePath(1, 6));

	std::cout << "pad test" << std::endl;
	robotArm.printPath(robotArm.calculatePath(Configuration{6,8,1,9,1,4}, Point(1,2)));



	return true;
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
 * (5) ga naar ready posities
 */




