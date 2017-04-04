/*
 * main.cpp
 *
 *  Created on: 30 mrt. 2017
 *      Author: stefan
 */
#include <iostream>
#include <cmath>
#include "RoboticArm.hpp"
#include "AStar.hpp"
#include "Size.hpp"
#include "Point.hpp"


int main(int argc, char **argv)
{
	try
	{
		const short hoverHeight = 50;
		const short objectHeight = 10;
		RoboticArm robotArm(50, 146, 187, 86, Servo
		{ -90, 90 }, Servo
		{ -30, 90 }, Servo
		{ 0, 135 }, Servo
		{ -90, 90 }, Servo
		{ -90, 90 }, Servo
		{ 0, 20 }); //TODO: hoogte a opmeten.
		robotArm.setConf(std::vector<signed short>
		{ 0, 0, 0, 0, 0, 0 });

		//(1) vind blokje positie&rotatie
		const int objectX = -150; 	//object X in mm
		const int objectY = 250;	//object Y in mm
		const int objectangle = 25;	//object angle in degrees
		const int objectwidth = 20;	//object windth in mm
		const int circelX = 50;	//circel center X
		const int circelY = 150;	//circel center Y

		std::cout << "\033[1;31mBlokje oppakken: \033[0m\n" << std::endl;
		//(2) base&gripper goed roteren & gripper openen
		robotArm.setGripperValue(30);

		//(3) ga naar 2 cm boven blokje & gripper naar beneden richten
		robotArm.armGoto(objectX, objectY, hoverHeight, objectangle);

		//(4) ga naar beneden & gripper dichtknijpen
		robotArm.armGoto(objectX, objectY, objectHeight, objectangle); //TODO: hoogte van grond afstellen
		robotArm.setGripperValue(objectwidth - 0); //TODO: knijpkracht afstellen

		//(5) ga naar 2 cm boven grond
		robotArm.armGoto(objectX, objectY, hoverHeight, 0);

		// Blokje neerleggen:
		std::cout << "\033[1;31mBlokje neerleggen: \033[0m\n" << std::endl;
		//(3) ga naar 2cm boven cirkel
		robotArm.armGoto(circelX, circelY, hoverHeight, 0);

		//(4) zakken naar grond & gripper loslaten
		robotArm.armGoto(circelX, circelY, objectHeight, 0);
		robotArm.setGripperValue(30);

		//(3) ga naar 2cm boven cirkel
		robotArm.armGoto(circelX, circelY, hoverHeight, 0);


	} catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
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
 * hoogte a opmeten in mm
 * robot pwm max en min instellen
 *
 */
