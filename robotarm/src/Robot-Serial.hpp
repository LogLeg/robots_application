/*
 * Robot-Serial.h
 *
 *  Created on: Mar 9, 2017
 *      Author: ronald
 */

#include "serial/serial.h"
#include <string>
#include <iostream>

#ifndef ROBOTSERIAL_H_
#define ROBOTSERIAL_H_
class RobotSerial {
public:
	/**
	 * @brief constructor
	 * @param port serial port of robot
	 * @param baud serial speed
	 */
	RobotSerial(std::string port, unsigned long baud);
	~RobotSerial();
	/**
	 * @brief send message to robot by serial
	 * @param Message message understandable by robot
	 */
	void send(std::string Message);
	/**
	 * @brief read message from robot
	 * @param length length of message to read
	 * @return string with text from robot
	 */
	std::string read(uint32_t length);
private:
	serial::Serial *my_serial;
	std::string port;
	unsigned long baud;
	
};

#endif /* MATRIX_H_ */
