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


int main(int argc, char **argv) {

	RoboticArm robotArm(1,146,187,86, Servo{-90,90}, Servo{-30,90}, Servo{0,135}, Servo{-90,90}, Servo{-90,90}, Servo{0,20}); //TODO: hoogte a opmeten.
	robotArm.setConf(std::vector<signed short>{0,0,0,0,0,0});

	//(1) vind blokje positie&rotatie
	const int objectX = -100; 	//object X in mm
	const int objectY = 250;	//object Y in mm
	const int objectangle = 25;	//object angle in degrees
	const int objectwidth = 20;	//object windth in mm
	const int circelX = 100;	//circel center X
	const int circelY = 150;	//circel center Y

	std::cout << "Blokje oppakken: " << std::endl;
	//(2) base&gripper goed roteren & gripper openen
	robotArm.setGripperValue(30);


	//(3) ga naar 2 cm boven blokje & gripper naar beneden richten
	robotArm.armGoto(objectX, objectY, 50, objectangle);

	//(4) ga naar beneden & gripper dichtknijpen
	robotArm.armGoto(objectX, objectY, 10, objectangle); //TODO: hoogte van grond afstellen
	robotArm.setGripperValue(objectwidth - 0); //TODO: knijpkracht afstellen

	//(5) ga naar 2 cm boven grond
	robotArm.armGoto(objectX, objectY, 50, 0);



	// Blokje neerleggen:
	std::cout << "Blokje neerleggen: " << std::endl;
	//(3) ga naar 2cm boven cirkel
	robotArm.armGoto(circelX, circelY, 50, 0);

	//(4) zakken naar grond & gripper loslaten
	robotArm.armGoto(circelX, circelY, 10, 0);
	robotArm.setGripperValue(30);

	//(3) ga naar 2cm boven cirkel
	robotArm.armGoto(circelX, circelY, 50, 0);









//	std::cout << "pad naar 6" << std::endl;
//	robotArm.printPath(robotArm.calculatePath(0, 6));

//	std::cout << "pad test" << std::endl;
//	std::vector<std::vector<signed short>> pad = robotArm.calculatePath(robotArm.getConf(), Point(320,30));
//	robotArm.printPath(pad);
//	robotArm.followPath(pad);
//	std::cout << "0: " << robotArm.getConf().at(0) << std::endl;
//	std::cout << "1: " << robotArm.getConf().at(1) << std::endl;
//	std::cout << "2: " << robotArm.getConf().at(2) << std::endl;
//	std::cout << "3: " << robotArm.getConf().at(3) << std::endl;
//	std::cout << "4: " << robotArm.getConf().at(4) << std::endl;
//	std::cout << "5: " << robotArm.getConf().at(5) << std::endl;


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
 * (5) ga naar ready positie
 */


/*
 * hoogte a opmeten in mm
 * robot pwm max en min instellen
 *
 */
