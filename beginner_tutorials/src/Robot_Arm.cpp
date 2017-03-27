#include "Robot_Arm.hpp"


RobotArm::RobotArm(std::string port, unsigned long baud):
			robotSerial(port, baud)
{
	std::cout << "RobotArm::RobotArm(std::string port, unsigned long baud):\n";
  	
	


	ServoList.push_back(Servo(553, 2425, 180, 0,&robotSerial, 90, false, 10));
	ServoList.push_back(Servo(700, 2300, 120, 1,&robotSerial, 30, true, 35));
	ServoList.push_back(Servo(700, 2300, 135, 2,&robotSerial, 0, false, 0));
	ServoList.push_back(Servo(554, 2520, 180, 3,&robotSerial, 90, false, -5));
	ServoList.push_back(Servo(553, 2425, 180, 4,&robotSerial, 90, false, 13));
	ServoList.push_back(Servo(700, 2300, 30, 5,&robotSerial, 0, true, 0));
		
	
	ServoList.at(1).gotoPosition(0, 1000);
	sleep(500);
	ServoList.at(2).gotoPosition(0, 1000);
	sleep(500);
	ServoList.at(0).gotoPosition(0, 1000);
	sleep(500);
	ServoList.at(3).gotoPosition(0, 1000);
	sleep(500);
	ServoList.at(4).gotoPosition(0, 1000);
	sleep(500);
	ServoList.at(5).gotoPosition(15, 1000);
	sleep(500);


	ServoList.at(0).gotoPosition(0, 1000);
	ServoList.at(1).gotoPosition(-45, 1000);
	ServoList.at(2).gotoPosition(110, 1000);
	ServoList.at(3).gotoPosition(-68, 1000);
	ServoList.at(4).gotoPosition(0, 1000);
	ServoList.at(5).gotoPosition(5, 1000);

	std::cout << "S0 position: " << ServoList.at(0).getAngle() << std::endl;
	std::cout << "S1 position: " << ServoList.at(1).getAngle() << std::endl;
	std::cout << "S2 position: " << ServoList.at(2).getAngle() << std::endl;
	std::cout << "S3 position: " << ServoList.at(3).getAngle() << std::endl;
	std::cout << "S4 position: " << ServoList.at(4).getAngle() << std::endl;
	std::cout << "S5 position: " << ServoList.at(5).getAngle() << std::endl;

	ROS_INFO("Ready to instruct the robot.");
	
}

RobotArm::~RobotArm()
{
	
}


bool RobotArm::goTo(beginner_tutorials::Robot_GoTo::Request  &req,
         beginner_tutorials::Robot_GoTo::Response &res)
{
	goTo(req.angle1,req.angle2,req.angle3,req.angle4,req.angle5,req.angle6, req.inTime);  	

  return true;
}

bool RobotArm::status(beginner_tutorials::Robot_Status::Request  &req,
         beginner_tutorials::Robot_Status::Response &res)
{
	//MyRobot->status();
	res.angle1=ServoList.at(0).getAngle();
	res.angle2=ServoList.at(1).getAngle();
	res.angle3=ServoList.at(2).getAngle();
	res.angle4=ServoList.at(3).getAngle();
	res.angle5=ServoList.at(4).getAngle();
	res.angle6=ServoList.at(5).getAngle();
	ROS_INFO("Status request");
	return true;
}

bool RobotArm::stop(beginner_tutorials::Robot_Stop::Request  &req,
         beginner_tutorials::Robot_Stop::Response &res)
{
	stop();
	ROS_INFO("request: Stop. we might think about it");
	return true;
}

void RobotArm::goTo(double a1, double a2, double a3, double a4, double a5, double a6, double Time)
{
	std::cout << "GOTO MESSAGE RECIVED!!!\n";
	ServoList.at(0).gotoPosition(a1, Time);
	ServoList.at(1).gotoPosition(a2, Time);
	ServoList.at(2).gotoPosition(a3, Time);
	ServoList.at(3).gotoPosition(a4, Time);
	ServoList.at(4).gotoPosition(a5, Time);
	ServoList.at(5).gotoPosition(a6, Time);
}
std::string RobotArm::status()
{
	
}

void RobotArm::stop()
{
	std::cout << "StopComannd ";
	std::string serialString = "STOP\r";
	std::cout << serialString;
	robotSerial.send(serialString);
}



void RobotArm::sleep(unsigned long milliseconds) {
#ifdef _WIN32
      Sleep(milliseconds); // 100 ms
#else
      usleep(milliseconds*1000); // 100 ms
#endif
}
