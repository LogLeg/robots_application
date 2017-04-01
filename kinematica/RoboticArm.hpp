/*
 * RoboticArm.hpp
 *
 *  Created on: 30 mrt. 2017
 *      Author: stefan
 */

#ifndef ROBOTICARM_HPP_
#define ROBOTICARM_HPP_
#include <vector>
#include <tuple>
#include "AStar.hpp"
#include "Size.hpp"

struct Position
{
	signed short x;
	signed short y;
};
struct Servo
{
	signed short min;
	signed short max;
};

struct robotInput	{
	unsigned long x;
	signed short robotAngle;
	signed short gripperAngle;
	signed short objectWidth;
};

class RoboticArm
{
public:
	/*
	 * @brief constructor
	 */
	RoboticArm(unsigned short a, unsigned short b, unsigned short c, unsigned short d, Servo s1, Servo s2, Servo s3, Servo s4, Servo s5, Servo s6);

	/*
	 * @brief go to xyz position and turn gripper
	 * @param z z-axis of robotarm X from top down view
	 * @param x x-axis of robotarm Y from top down view
	 * @param Y y-axis of robotarm no top down view- height of gripper.
	 */
	void armGoto(signed long z, unsigned long x, unsigned long y, signed short objectAngle);
	/*
	 * @brief set the angle of the base
	 * @param angle angle to set
	 */
	void setBaseAngle(signed short angle);
	/*
	 * @brief set the angle of the gripper
	 * @param angle angle to set
	 */
	void setGripperAngle(signed short angle);
	/*
	 * @brief set the size of the gripper
	 * @param width width of the gripper
	 */
	void setGripperValue(signed short width);
	/*
	 * @brief copydestructor
	 */
	virtual ~RoboticArm();
	/*
	 * @brief calculate a path of configurations
	 * @param inputConf start configuration
	 * @param endpoint point where the arm goes to
	 * @return a path of configurations
	 */
	std::vector<std::vector<signed short>> calculatePath(std::vector<signed short> inputConf, Point endPoint);
	/*
	 * @brief make linear path of configurations
	 * @param phi servo that goes to endValue
	 * @param endValue last value of path
	 */
	std::vector<std::vector<signed short>> calculatePath(unsigned char phi, signed short endValue);
	/*
	 * @brief print path (verctor of configurations)
	 * @param confs path
	 */
	void printPath(std::vector<std::vector<signed short>> confs);
	/*
	 * @brief arm follow path TODO: (go to last configuration of path)
	 * @param path path to follow
	 */
	void followPath(const std::vector<std::vector<signed short>>& path);

	std::pair<double, double> forwardKinematics(double x0, double y0, double a, double p1, double b, double p2);


	std::pair<unsigned long, signed short> convertToXAngle(signed long x, unsigned long y);
	Position getPos() const;
	void setPos(Position pos);
	const std::vector<signed short>& getConf() const;
	void setConf(const std::vector<signed short>& conf);

	//lengths in mm
	unsigned short a; // ground to shoulder
	unsigned short b; // shoulder to elbow
	unsigned short c; // elbow to wrist
	unsigned short d; // wrist to gripper
	Servo s1;
	Servo s2;
	Servo s3;
	Servo s4;
	Servo s5;
	Servo s6;

private:
	std::vector<signed short> Conf;
	Position pos;



};

#endif /* ROBOTICARM_HPP_ */