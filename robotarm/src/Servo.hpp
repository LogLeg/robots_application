/*
 * Servo.hpp
 *
 *  Created on: 8 mrt. 2017
 *      Author: stefan
 */

#ifndef SERVO_HPP_
#define SERVO_HPP_

#include "Robot-Serial.hpp"
class Servo
{
public:
	/**
	 * @brief constructor
	 * @param minPwm minimum pwm of the servo
	 * @param dRange range in degrees of the servo
	 * @param servoNr Nr of servo
	 * @param abnormality center angle of servo in degrees
	 * @param flip angle is flipped if true
	 * @param servoAbnormality center angle of servo(to correct for different robots
	 */
	Servo(unsigned short minPwm, unsigned short pwmRange, unsigned char dRange, unsigned char servoNr, RobotSerial* aRobotSerial, signed short abnormality = 0, bool flip = false, signed short servoAbnormality = 0);
	/**
	 * @brief go to given position
	 * @param servo number of the servo
	 * @param degrees angle of the servo
	 * @time time in ms
	 */
	void gotoPosition(signed short degrees, unsigned short time);
	/**
	 * @brief converts degrees to pwm
	 * @param degrees angle of the servo
	 * @param servo number of the servo
	 * @return servo angle in Pwm
	 */
	unsigned short degreesToPwm(signed short degrees);
	/**
	 * @brief converts degrees to pwm
	 * @param pwm angle of the servo in pwm
	 * @param servo number of the servo
	 * @return servo angle in degrees
	 */
	signed short pwmToDegrees(unsigned short pwm, unsigned char servo);
	/**
	 * @brief get the current angle of the servo
	 * @return the angle of the servo in degrees
	 */
	signed short getAngle();
	virtual ~Servo();
private:
	unsigned short minPwm;
	unsigned short pwmRange;
	unsigned char dRange;
	unsigned char servoNr;
	signed short abnormality;
	bool flip;
	signed short servoAbnormality;
	RobotSerial* robotSerial;
};

#endif /* SERVO_HPP_ */
