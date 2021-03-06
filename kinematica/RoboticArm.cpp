/*
 * RoboticArm.cpp
 *
 *  Created on: 30 mrt. 2017
 *      Author: stefan
 */

#include "RoboticArm.hpp"
#include <iostream>
#include <cmath>

RoboticArm::RoboticArm(unsigned short a, unsigned short b, unsigned short c, unsigned short d, Servo s1, Servo s2, Servo s3, Servo s4, Servo s5, Servo s6) :
		a(a), b(b), c(c), d(d), s1(s1), s2(s2), s3(s3), s4(s4), s5(s5), s6(s6)
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

std::vector<std::vector<signed short>> RoboticArm::calculatePath(std::vector<
		signed short> inputConf, Point endPoint)
{
	std::vector<std::vector<signed short>> confs;

	confs.push_back(inputConf);

	PathAlgorithm::AStar astar;

	std::pair<double, double> currentPos = forwardKinematics(0, a, b, inputConf.at(1), c, inputConf.at(2));
	//std::cout << "x: " << currentPos.first << std::endl;
	//std::cout << "y: " << currentPos.second << std::endl;

	PathAlgorithm::Path pad = astar.search(PathAlgorithm::Vertex(currentPos.first, currentPos.second, static_cast<int>(inputConf.at(1)), static_cast<int>(inputConf.at(2))), PathAlgorithm::Vertex(endPoint.x, endPoint.y + d, 0, 0), Widgets::Size(1, 1), (*this));

	//std::cout << "pad: " << std::endl;

	for (PathAlgorithm::Vertex &i : pad)
	{
		//std::cout << i << std::endl;
		signed short newPhi3 = 180 - i.phi1 - i.phi2;
		newPhi3 = newPhi3 > s4.max ? s4.max : newPhi3;
		newPhi3 = newPhi3 < s4.min ? s4.min : newPhi3;
		confs.push_back(std::vector<signed short>
		{ inputConf.at(0), static_cast<signed short>(i.phi1), static_cast<signed short>(i.phi2), newPhi3, inputConf.at(4), inputConf.at(5) });
	}

	return confs;
}

void RoboticArm::printPath(std::vector<std::vector<signed short>> confs)
{
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

	auto editPhi = configuration.at(phi);
	if (endValue > editPhi)
	{
		for (unsigned short i = 0; i <= (endValue - editPhi); ++i)
		{
			//std::cout << i << std::endl;
			std::vector<signed short> newConf = configuration;
			newConf.at(phi) = editPhi + i;
			confs.push_back(newConf);
		}
	}else{
		for (unsigned short i = 0; i <= (editPhi - endValue); ++i)
		{
			//std::cout << i << std::endl;
			std::vector<signed short> newConf = configuration;
			newConf.at(phi) = editPhi - i;
			confs.push_back(newConf);
		}
	}
	return confs;
}

const std::vector<signed short>& RoboticArm::getConf() const
{
	return configuration;
}

void RoboticArm::setGripperValue(signed short width)
{
	std::vector<std::vector<signed short>> pad = calculatePath(5, width);
	//printPath(pad);
	followPath(pad);
}

void RoboticArm::setGripperAngle(signed short angle)
{
	angle = configuration.at(4) + angle + 0; //TODO: evt. + 90
	std::vector<std::vector<signed short>> pad = calculatePath(4, angle);
	//printPath(pad);
	followPath(pad);

}

void RoboticArm::setConf(const std::vector<signed short>& conf)
{
	configuration = conf;
}

std::pair<double, double> RoboticArm::forwardKinematics(double x0, double y0, double a, double p1, double b, double p2)
{
	using std::sin;
	using std::cos;

	return std::pair<double, double>((x0 + a * sin((p1) * M_PI / 180.0) + b * sin((p1 + p2) * M_PI / 180.0)), (y0 + a * cos((p1) * M_PI / 180.0) + b * cos((p1 + p2) * M_PI / 180.0)));
}

std::pair<double, double> RoboticArm::forwardKinematics(double x0, double y0, double a, double p1, double b, double p2, double c, double p3)
{
	using std::sin;
	using std::cos;

	return std::pair<double, double>((x0 + a * sin((p1) * M_PI / 180.0) + b * sin((p1 + p2) * M_PI / 180.0) + c * sin((p1 + p2 + p3) * M_PI / 180.0)), (y0 + a * cos((p1) * M_PI / 180.0) + b * cos((p1 + p2) * M_PI / 180.0) + c * cos((p1 + p2 + p3) * M_PI / 180.0)));
}

void RoboticArm::followPath(const std::vector<std::vector<signed short> >& path)
{
	configuration = path.back();
	std::cout << "0: " << getConf().at(0);
	std::cout << " 1: " << getConf().at(1);
	std::cout << " 2: " << getConf().at(2);
	std::cout << " 3: " << getConf().at(3);
	std::cout << " 4: " << getConf().at(4);
	std::cout << " 5: " << getConf().at(5) << std::endl;
	std::cout << "x: " << forwardKinematics(0, a, b, getConf().at(1), c, getConf().at(2), d, getConf().at(3)).first << " y: " << forwardKinematics(0, a, b, getConf().at(1), c, getConf().at(2), d, getConf().at(3)).second << std::endl;

	std::cin.ignore();
}

std::pair<unsigned long, signed short> RoboticArm::convertToXAngle(signed long x, unsigned long y)
{
	unsigned long rX = sqrt(pow(x, 2) + pow(y, 2));
	return std::pair<unsigned long, signed short>(rX, round(std::asin(double(x) / double(rX)) * 180.0 / M_PI));
}

void RoboticArm::armGoto(signed long z, unsigned long x, unsigned long y, signed short objectAngle)
{
	std::pair<unsigned long, signed short> XAngle = convertToXAngle(z, x);

	std::vector<std::vector<signed short>> pad = calculatePath(configuration, Point(XAngle.first, y));
	//printPath(pad);
	followPath(pad);
	pad = calculatePath(0, XAngle.second);
	//printPath(pad);
	followPath(pad);
	pad = calculatePath(4, -XAngle.second + objectAngle + 0); // TODO: evt. +90
	//printPath(pad);
	followPath(pad);

}
