/*
 * Robot-Serial.h
 *
 *  Created on: Mar 9, 2017
 *      Author: ronald
 */

#include <string>
#include <iostream>
#include "Robot-Serial.hpp"
#include "Servo.hpp"
#include "ros/ros.h"
#include "robotarm/Robot_GoTo.h"
#include "robotarm/Robot_Status.h"
#include "robotarm/Robot_Stop.h"

#ifndef ROBOTARM_H_
#define ROBOTARM_H_
class RobotArm {
public:
	RobotArm(std::string port, unsigned long baud);
	~RobotArm();
        bool goTo(robotarm::Robot_GoTo::Request  &req,
                 robotarm::Robot_GoTo::Response &res);

        bool status(robotarm::Robot_Status::Request  &req,
                robotarm::Robot_Status::Response &res);

        bool stop(robotarm::Robot_Stop::Request  &req,
                robotarm::Robot_Stop::Response &res);
private:
	void goTo(double a1, double a2, double a3, double a4, double a5, double a6, double Time);
	std::string status();
	void stop();
	RobotSerial robotSerial;
	std::vector<Servo> ServoList;
		
	
	void sleep(unsigned long milliseconds);
};

#endif /* MATRIX_H_ */
