/*
 * Servo.cpp
 *
 *  Created on: 8 mrt. 2017
 *      Author: stefan
 */

#include "Servo.hpp"
#include <string>
#include <iostream>

Servo::Servo(unsigned short minPwm, unsigned short pwmRange, unsigned char dRange, unsigned short servoNr, RobotSerial* aRobotSerial, signed short abnormality, bool flip, signed short servoAbnormality) :
		minPwm(minPwm), pwmRange(pwmRange - minPwm), dRange(dRange), servoNr(servoNr), abnormality(abnormality), flip(flip), servoAbnormality(servoAbnormality),
robotSerial(aRobotSerial)
{
	
}

void Servo::gotoPosition(signed short degrees, unsigned short time)
{
	std::string serialString = "#" + std::to_string(static_cast<int>(servoNr)) + "P" + std::to_string(degreesToPwm(degrees, servoNr)) + "T" + std::to_string(time) + "\r";
	std::cout << serialString;
	robotSerial->send(serialString);
}

unsigned short Servo::degreesToPwm(signed short degrees, unsigned char servo)
{
	unsigned long pwm;

	degrees += abnormality;

	if (degrees >= dRange)
		degrees = dRange;
	if (degrees < 0)
		degrees = 0;
	degrees += servoAbnormality;

	float step = static_cast<float>(pwmRange) / static_cast<float>(dRange);
	if (flip)
	{
		pwm = pwmRange - (static_cast<int>(step * static_cast<float>(degrees))) + minPwm;
	}
	else
	{
		pwm = static_cast<int>(step * static_cast<float>(degrees)) + minPwm;
	}
	return pwm;
}

signed short Servo::getAngle()
{
	std::string serialString = "QP " + std::to_string(static_cast<int>(servoNr)) + "\r";
	robotSerial->send(serialString);
	std::string result = robotSerial->read(1);
	
	const char *cstr = result.c_str();
	unsigned long pwm = (unsigned((uint8_t)result[0]) * 10) - minPwm;
	std::cout << "PWM " << pwm << "\n";
	float step = static_cast<float>(dRange) / static_cast<float>(pwmRange);
	signed short degrees;
	if (flip)
	{

		degrees = dRange - (step * static_cast<float>(pwm));
	}
	else
	{
		degrees = step * static_cast<float>(pwm);
	}
	degrees -= abnormality;
	degrees -= servoAbnormality;
	return degrees;
}

Servo::~Servo()
{

}

