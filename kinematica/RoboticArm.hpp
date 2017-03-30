/*
 * RoboticArm.hpp
 *
 *  Created on: 30 mrt. 2017
 *      Author: stefan
 */

#ifndef ROBOTICARM_HPP_
#define ROBOTICARM_HPP_
#include <vector>

struct Configuration
{
	signed short phi1;
	signed short phi2;
	signed short phi3;
	signed short phi4;
	signed short phi5;
	signed short phi6;
};
struct Position
{
	signed short x;
	signed short y;
};
struct Point
{
	Point(int x, int y) :
			x(x), y(y)
	{
	}
	;
	int x;
	int y;
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
	std::vector<Configuration> calculatePath(Configuration inputConf, Point endPoint);


	std::vector<Configuration> calculatePath(unsigned char phi, signed short endValue);

	void printPath(std::vector<Configuration> confs);




	const Configuration& getConf() const;
	void setConf(const Configuration& conf);
	Position getPos() const;
	void setPos(Position pos);

private:
	Configuration Conf;
	Position pos;

	//lengths in mm
	unsigned short a; // ground to shoulder
	unsigned short b; // shoulder to elbow
	unsigned short c; // elbow to wrist
	unsigned short d; // wrist to gripper


};

#endif /* ROBOTICARM_HPP_ */
