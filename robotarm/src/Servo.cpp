/*
 * Servo.cpp
 *
 *  Created on: 8 mrt. 2017
 *      Author: stefan
 */

#include "Servo.hpp"
#include <string>
#include <iostream>

Servo::Servo(unsigned short minPwm, unsigned short pwmRange, unsigned char dRange, unsigned char servoNr, RobotSerial* aRobotSerial, signed short abnormality, bool flip, signed short servoAbnormality) :
		minPwm(minPwm), pwmRange(static_cast<unsigned short>(pwmRange - minPwm)), dRange(dRange), servoNr(servoNr), abnormality(abnormality), flip(flip), servoAbnormality(servoAbnormality),
robotSerial(aRobotSerial)
{
	
}

void Servo::gotoPosition(signed short degrees, unsigned short time)
{
	std::string serialString = "#" + std::to_string(static_cast<int>(servoNr)) + "P" + std::to_string(degreesToPwm(degrees)) + "T" + std::to_string(time) + "\r";
	std::cout << serialString;
	robotSerial->send(serialString);
}

unsigned short Servo::degreesToPwm(signed short degrees)
{
	unsigned short pwm;

	degrees = static_cast<signed short>(abnormality + degrees);

	if (degrees >= dRange)
		degrees = dRange;
	if (degrees < 0)
		degrees = 0;
	degrees = static_cast<signed short>(servoAbnormality + degrees);

	float step = static_cast<float>(pwmRange) / static_cast<float>(dRange);
	if (flip)
	{
		pwm = static_cast<unsigned short>(pwmRange - (static_cast<unsigned short>(step * static_cast<float>(degrees))) + minPwm);
	}
	else
	{
		pwm = static_cast<unsigned short>(static_cast<unsigned short>(step * static_cast<float>(degrees)) + minPwm);
	}
	return pwm;
}

signed short Servo::getAngle()
{
	std::string serialString = "QP " + std::to_string(static_cast<int>(servoNr)) + "\r";
	robotSerial->send(serialString);
	std::string result = robotSerial->read(1);
	
	unsigned long pwm = (unsigned((uint8_t)result[0]) * 10) - minPwm;
	std::cout << "PWM " << pwm << "\n";
	float step = static_cast<float>(dRange) / static_cast<float>(pwmRange);
	signed short degrees;
	if (flip)
	{

		degrees = static_cast<signed short>(dRange - (step * static_cast<float>(pwm)));
	}
	else
	{
		degrees = static_cast<signed short>(step * static_cast<float>(pwm));
	}
	degrees = static_cast<signed short>(degrees - abnormality);
	degrees = static_cast<signed short>(degrees - servoAbnormality);
	return degrees;
}

Servo::~Servo()
{

}

