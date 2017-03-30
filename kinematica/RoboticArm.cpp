/*
 * RoboticArm.cpp
 *
 *  Created on: 30 mrt. 2017
 *      Author: stefan
 */

#include "RoboticArm.hpp"
#include <iostream>

RoboticArm::RoboticArm(unsigned short a, unsigned short b, unsigned short c, unsigned short d) : a(a), b(b), c(c), d(d)
{

}

RoboticArm::~RoboticArm()
{

}

const Configuration& RoboticArm::getConf() const
{
	return Conf;
}

void RoboticArm::setConf(const Configuration& conf)
{
	Conf = conf;
}

Position RoboticArm::getPos() const
{
	return pos;
}

void RoboticArm::setPos(Position pos)
{
	this->pos = pos;
}

std::vector<Configuration> RoboticArm::calculatePath(Configuration inputConf, Point endPoint)
{
	std::vector<Configuration> confs;

	confs.push_back(Configuration{1,2,3,4,5,6});
	confs.push_back(Configuration{2,8,5,4,3,6});
	confs.push_back(inputConf);

	return confs;
}

void RoboticArm::printPath(std::vector<Configuration> confs)
{
	//std::vector<Configuration> confs = calculatePath(inputConf, endPoint);
	int c = 0;
	for (auto &i : confs)
	{
		++c;
		std::cout << "c: " << c;
		std::cout << " phi1: " << i.phi1;
		std::cout << " phi2: " << i.phi2;
		std::cout << " phi3: " << i.phi3;
		std::cout << " phi4: " << i.phi4;
		std::cout << " phi5: " << i.phi5;
		std::cout << " phi6: " << i.phi6 << std::endl;
	}

}

std::vector<Configuration> RoboticArm::calculatePath(unsigned char phi, signed short endValue)
{
	std::vector<Configuration> confs;
	signed short& phis = Conf.phi1;
//	if (phi == 1){
//		phis = Conf.phi1;
//	}
	for (unsigned short i = 0; i <= (endValue - phis); ++i){
		confs.push_back(Configuration{ phis + i, Conf.phi2, Conf.phi3, Conf.phi4, Conf.phi5, Conf.phi6});
	}

	std::cout << "phis " << phis << std::endl;

	return confs;
}
