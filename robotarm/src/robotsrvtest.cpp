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
#include "RoboticArm.hpp"
#include "AStar.hpp"
#include "Size.hpp"
#include "Point.hpp"
// OS Specific sleep
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void mySleep(unsigned long milliseconds) {
#ifdef _WIN32
      Sleep(milliseconds); // 100 ms
#else
      usleep(milliseconds*1000); // 100 ms
#endif
}

int main(int argc, char **argv)
{
        try
        {
                ros::init(argc, argv, "RotboArmBesturing_client");
                const short hoverHeight = 80;
                const short objectHeight = 0;
                RoboticArm robotArm(70, 146, 187, 115, Servo
                { -90, 90 }, Servo
                { -30, 90 }, Servo
                { 0, 135 }, Servo
                { -90, 90 }, Servo
                { -90, 90 }, Servo
                { 0, 30 }); //TODO: hoogte a opmeten.
                robotArm.setConf(std::vector<signed short>
                { 0, 0, 0, 0, 0, 0 });

                //(1) vind blokje positie&rotatie
                const int objectX = -183; 	//object X in mm
                const int objectY = 0;	//object Y in mm
                const int objectangle = 0;	//object angle in degrees
                const int objectwidth = 15;	//object windth in mm
                const int circelX = 60;	//circel center X
                const int circelY = 250;	//circel center Y

                std::cout << "\033[1;31mBlokje oppakken: \033[0m\n" << std::endl;
                //(2) base&gripper goed roteren & gripper openen
                robotArm.setGripperValue(30);

                //(3) ga naar 2 cm boven blokje & gripper naar beneden richten
                robotArm.armGoto(objectX, objectY, hoverHeight, objectangle);

                //(4) ga naar beneden & gripper dichtknijpen
                robotArm.armGoto(objectX, objectY, objectHeight, objectangle); //TODO: hoogte van grond afstellen
                robotArm.setGripperValue(objectwidth - 5); //TODO: knijpkracht afstellen

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
