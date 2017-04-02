#include <string>
#include <iostream>
#include <cstdio>

// OS Specific sleep
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif


#include "ros/ros.h"
#include "robotarm/Robot_GoTo.h"
#include "robotarm/Robot_Status.h"
#include "robotarm/Robot_Stop.h"
#include "Robot-Serial.hpp"
#include "Robot_Arm.hpp"

using std::string;
using std::exception;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

int main(int argc, char **argv)
{
  
	ros::init(argc, argv, "Robot_Driver_server");
        RobotArm MyRobot("/dev/pts/32", 9600);// /dev/ttyUSB0 // /dev/ttyACM0

	ros::NodeHandle n;

	ros::ServiceServer service = n.advertiseService("Robot_GoTo", &RobotArm::goTo, &MyRobot);
	ros::ServiceServer service2 = n.advertiseService("Robot_Stop", &RobotArm::stop, &MyRobot);
	ros::ServiceServer service3 = n.advertiseService("Robot_Status", &RobotArm::status, &MyRobot);
	std::cout << "Robot driver setup & init done\n";
	ros::spin();
  	return 0;
}







