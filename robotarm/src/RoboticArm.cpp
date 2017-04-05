/*
 * RoboticArm.cpp
 *
 *  Created on: 30 mrt. 2017
 *      Author: stefan
 */

#include "RoboticArm.hpp"
#include <iostream>
#include <cmath>
#include "ros/ros.h"
#include "robotarm/Robot_GoTo.h"
#include "robotarm/Robot_Set_Ready.h"
#include <cstdlib>
#include <chrono>
#include <thread>

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

bool RoboticArm::moveObject(signed long objectX, unsigned long objectY, signed short objectAngle, unsigned short objectWidth, signed long desX, unsigned long desY)
{

	const short hoverHeight = 80;
	const short objectHeight = 0;
	const long minMaxDistance[] = {100,320};

	std::pair<unsigned long, signed short> XAngle = convertToXAngle(objectX, objectY);
	if (XAngle.first < minMaxDistance[0] || XAngle.first > minMaxDistance[1])
	{
		std::cerr << "\033[1;31mObject out of reach!\033[0m\n" << std::endl;
		return false;
	}
	XAngle = convertToXAngle(desX, desY);
	if (XAngle.first < minMaxDistance[0] || XAngle.first > minMaxDistance[1])
	{
		std::cerr << "\033[1;31mTarget out of reach!\033[0m\n" << std::endl;
		return false;
	}

	std::cout << "\033[1;31mBlokje oppakken: \033[0m\n" << std::endl;
	//(2) base&gripper goed roteren & gripper openen
	setGripperValue(30);

	//(3) ga naar 2 cm boven blokje & gripper naar beneden richten
	armGoto(objectX, objectY, hoverHeight, objectAngle);

	//(4) ga naar beneden & gripper dichtknijpen
	armGoto(objectX, objectY, objectHeight, objectAngle);
	setGripperValue(/*objectWidth - 5*/5); //TODO: knijpkracht afstellen

	//(5) ga naar 2 cm boven grond
	armGoto(objectX, objectY, hoverHeight, 0);

	// Blokje neerleggen:
	std::cout << "\033[1;31mBlokje neerleggen: \033[0m\n" << std::endl;
	//(3) ga naar 2cm boven cirkel
	armGoto(desX, desY, hoverHeight, 0);

	//(4) zakken naar grond & gripper loslaten
	armGoto(desX, desY, objectHeight, 0);
	setGripperValue(30);

	//(3) ga naar 2cm boven cirkel
	armGoto(desX, desY, hoverHeight, 0);
	return true;
}

std::vector<std::vector<signed short>> RoboticArm::calculatePath(std::vector<
		signed short> inputConf, robotPoint::Point endPoint)
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

	signed short editPhi = configuration.at(phi);
	if (endValue > editPhi)
	{
		for (unsigned short i = 0; i <= (endValue - editPhi); ++i)
		{
			//std::cout << i << std::endl;
			std::vector<signed short> newConf = configuration;
			newConf.at(phi) = editPhi + i;
			confs.push_back(newConf);
		}
	}
	else
	{
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

	//std::cin.ignore();
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	ros::NodeHandle n;

	ros::ServiceClient client = n.serviceClient<robotarm::Robot_GoTo>("Robot_GoTo");
	robotarm::Robot_GoTo srv;
	srv.request.angle1 = getConf().at(0);
	srv.request.angle2 = getConf().at(1);
	srv.request.angle3 = getConf().at(2);
	srv.request.angle4 = getConf().at(3);
	srv.request.angle5 = getConf().at(4);
	srv.request.angle6 = getConf().at(5);
	srv.request.inTime = 500;
	if (client.call(srv))
	{

	}
}

std::pair<unsigned long, signed short> RoboticArm::convertToXAngle(signed long x, unsigned long y)
{
	unsigned long rX = sqrt(pow(x, 2) + pow(y, 2));
	return std::pair<unsigned long, signed short>(rX, round(std::asin(double(x) / double(rX)) * 180.0 / M_PI));
}

void RoboticArm::armGoto(signed long z, unsigned long x, unsigned long y, signed short objectAngle)
{
	std::pair<unsigned long, signed short> XAngle = convertToXAngle(z, x);

	std::vector<std::vector<signed short>> pad = calculatePath(configuration, robotPoint::Point(XAngle.first, y));
	//printPath(pad);
	configuration = pad.back();
	//followPath(pad);
	pad = calculatePath(0, XAngle.second);
	//printPath(pad);
	//followPath(pad);
	configuration = pad.back();
	pad = calculatePath(4, -XAngle.second + objectAngle + 0); // TODO: evt. +90
	//printPath(pad);
	followPath(pad);

}

bool RoboticArm::gotoPark()
{
	configuration = std::vector<signed short>
	{ -90, -30, 110, 90, 90, 5 };
	std::cout << "0: " << getConf().at(0);
	std::cout << " 1: " << getConf().at(1);
	std::cout << " 2: " << getConf().at(2);
	std::cout << " 3: " << getConf().at(3);
	std::cout << " 4: " << getConf().at(4);
	std::cout << " 5: " << getConf().at(5) << std::endl;
	std::cout << "x: " << forwardKinematics(0, a, b, getConf().at(1), c, getConf().at(2), d, getConf().at(3)).first << " y: " << forwardKinematics(0, a, b, getConf().at(1), c, getConf().at(2), d, getConf().at(3)).second << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	ros::NodeHandle n;

	ros::ServiceClient client = n.serviceClient<robotarm::Robot_GoTo>("Robot_GoTo");
	robotarm::Robot_GoTo srv;
	srv.request.angle1 = getConf().at(0);
	srv.request.angle2 = getConf().at(1);
	srv.request.angle3 = getConf().at(2);
	srv.request.angle4 = getConf().at(3);
	srv.request.angle5 = getConf().at(4);
	srv.request.angle6 = getConf().at(5);
	srv.request.inTime = 2000;

	if (client.call(srv))
	{

	}
}
