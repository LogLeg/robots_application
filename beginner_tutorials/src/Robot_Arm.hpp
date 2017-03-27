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
#include "beginner_tutorials/Robot_GoTo.h"
#include "beginner_tutorials/Robot_Status.h"
#include "beginner_tutorials/Robot_Stop.h"

#ifndef ROBOTARM_H_
#define ROBOTARM_H_
class RobotArm {
public:
	RobotArm(std::string port, unsigned long baud);
	~RobotArm();
	bool goTo(beginner_tutorials::Robot_GoTo::Request  &req,
	         beginner_tutorials::Robot_GoTo::Response &res);

	bool status(beginner_tutorials::Robot_Status::Request  &req,
         	beginner_tutorials::Robot_Status::Response &res);

	bool stop(beginner_tutorials::Robot_Stop::Request  &req,
         	beginner_tutorials::Robot_Stop::Response &res);
private:
	void goTo(double a1, double a2, double a3, double a4, double a5, double a6, double Time);
	std::string status();
	void stop();
	RobotSerial robotSerial;
	std::vector<Servo> ServoList;
		
	
	void sleep(unsigned long milliseconds);
};

#endif /* MATRIX_H_ */
