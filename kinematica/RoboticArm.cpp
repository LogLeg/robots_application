/*
 * RoboticArm.cpp
 *
 *  Created on: 30 mrt. 2017
 *      Author: stefan
 */

#include "RoboticArm.hpp"
#include <iostream>
#include <cmath>

RoboticArm::RoboticArm(unsigned short a, unsigned short b, unsigned short c, unsigned short d) : a(a), b(b), c(c), d(d)
{

}

RoboticArm::~RoboticArm()
{

}


Position RoboticArm::getPos() const
{
	return pos;
}

void RoboticArm::setPos(Position pos)
{
	this->pos = pos;
}

std::vector<std::vector<signed short>> RoboticArm::calculatePath(std::vector<signed short> inputConf, Point endPoint)
{
	std::vector<std::vector<signed short>> confs;

	confs.push_back(inputConf);

	PathAlgorithm::AStar astar;

	std::pair<double, double> currentPos = forwardKinematics(0, 0, b, inputConf.at(1), c, inputConf.at(2));
	std::cout << "x: " << currentPos.first << std::endl;
	std::cout << "y: " << currentPos.second << std::endl;

	PathAlgorithm::Path pad = astar.search(PathAlgorithm::Vertex(currentPos.first,currentPos.second,static_cast<int>(inputConf.at(1)),static_cast<int>(inputConf.at(2))), PathAlgorithm::Vertex(endPoint.x,endPoint.y,0,0), Widgets::Size(1,1));

	std::cout << "pad: " << std::endl;

	for (PathAlgorithm::Vertex &i : pad)
	{
		std::cout << i << std::endl;
		//std::cout << "x: " << forwardKinematics(0, 0, b, i.phi1, c, i.phi2).first << " y: " << forwardKinematics(0, 0, b, i.phi1, c, i.phi2).second << std::endl;
		confs.push_back(std::vector<signed short>{inputConf.at(0),static_cast<signed short>(i.phi1),static_cast<signed short>(i.phi2),inputConf.at(3),inputConf.at(4),inputConf.at(5)});
	}


	return confs;
}

void RoboticArm::printPath(std::vector<std::vector<signed short>> confs)
{
	//std::vector<Configuration> confs = calculatePath(inputConf, endPoint);
	int c = 0;
	for (auto &i : confs)
	{
		std::cout << "c: " << c;
		std::cout << " phi1: " << i.at(0);
		std::cout << " phi2: " << i.at(1);
		std::cout << " phi3: " << i.at(2);
		std::cout << " phi4: " << i.at(3);
		std::cout << " phi5: " << i.at(4);
		std::cout << " phi6: " << i.at(5) << std::endl;
		++c;
	}
}

std::vector<std::vector<signed short>> RoboticArm::calculatePath(unsigned char phi, signed short endValue)
{
	std::vector<std::vector<signed short>> confs;

	auto editPhi = Conf.at(phi);
	for (unsigned short i = 0; i <= (endValue - editPhi); ++i){
		std::cout << i << std::endl;
		std::vector<signed short> newConf = Conf;
		newConf.at(phi) = editPhi + i;
		confs.push_back(newConf);
	}
	return confs;
}

const std::vector<signed short>& RoboticArm::getConf() const
{
	return Conf;
}

void RoboticArm::setConf(const std::vector<signed short>& conf)
{
	Conf = conf;
}

std::pair<double, double> RoboticArm::forwardKinematics(double x0, double y0, double a, double p1, double b, double p2)
{

	const double PI = 3.141592654;
	using std::sin;
	using std::cos;

	return std::pair<double, double>((x0 + a * sin((p1) * PI / 180.0) + b * sin((p1 + p2) * PI / 180.0)), (y0 + a * cos((p1) * PI / 180.0) + b * cos((p1 + p2) * PI / 180.0)));
}
