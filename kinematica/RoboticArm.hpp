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

class RoboticArm
{
public:
	/*
	 * @brief constructor
	 */
	RoboticArm(unsigned short a, unsigned short b, unsigned short c, unsigned short d);
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
	 * @param
	 * @return a path of configurations
	 */
	std::vector<std::vector<signed short>> calculatePath(std::vector<signed short> inputConf, Point endPoint);

	std::vector<std::vector<signed short>> calculatePath(unsigned char phi, signed short endValue);

	void printPath(std::vector<std::vector<signed short>> confs);

	std::pair<double, double> forwardKinematics(double x0, double y0, double a, double p1, double b, double p2);


	Position getPos() const;
	void setPos(Position pos);
	const std::vector<signed short>& getConf() const;
	void setConf(const std::vector<signed short>& conf);

private:
	std::vector<signed short> Conf;
	Position pos;

	//lengths in mm
	unsigned short a; // ground to shoulder
	unsigned short b; // shoulder to elbow
	unsigned short c; // elbow to wrist
	unsigned short d; // wrist to gripper


};

#endif /* ROBOTICARM_HPP_ */
