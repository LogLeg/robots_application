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
class RobotArm
{
public:
	/**
	 * @brief constructor
	 * @param port port of the serial robot
	 * @param baud boudrate of serial comm.
	 */
	RobotArm(std::string port, unsigned long baud);
	/**
	 * @brief destructor
	 */
	~RobotArm();
	/**
	 * @brief go to position
	 * @param req request
	 * @param res response
	 * @return bool
	 */
	bool goTo(robotarm::Robot_GoTo::Request &req, robotarm::Robot_GoTo::Response &res);
	/**
	 * @brief robot status
	 * @param req request
	 * @param res response
	 * @return bool
	 */
	bool status(robotarm::Robot_Status::Request &req, robotarm::Robot_Status::Response &res);
	/**
	 * @brief stop robot
	 * @param req request
	 * @param res response
	 * @return bool
	 */
	bool stop(robotarm::Robot_Stop::Request &req, robotarm::Robot_Stop::Response &res);
private:
	/**
	 * @brief go to configuration
	 * @param a1 angle of 1st servo
	 * @param a2 angle of 2st servo
	 * @param a3 angle of 3st servo
	 * @param a4 angle of 4st servo
	 * @param a5 angle of 5st servo
	 * @param a6 angle of 6st servo
	 * @param Time time to arrive
	 */
	void goTo(double a1, double a2, double a3, double a4, double a5, double a6, double Time);
	/**
	 * @brief get the status of the robot
	 * @return string with config angles
	 */
	std::string status();
	/**
	 * @brief emergency stop
	 */
	void stop();
	RobotSerial robotSerial;
	std::vector<Servo> ServoList;

	void sleep(unsigned long milliseconds);
};

#endif /* MATRIX_H_ */
