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
	RobotSerial(std::string port, unsigned long baud);
	~RobotSerial();
	void send(std::string Message);
	std::string read(uint32_t length);
private:
	serial::Serial *my_serial;
	std::string port;
	unsigned long baud;
	
};

#endif /* MATRIX_H_ */
