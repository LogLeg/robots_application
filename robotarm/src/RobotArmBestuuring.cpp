#include "ros/ros.h"
#include "robotarm/Robot_GoTo.h"
#include "robotarm/Robot_Set_Ready.h"
#include "robotarm/Robot_Set_Park.h"
#include "robotarm/Robot_Set_Straight.h"
#include "robotarm/Robot_Set_Test.h"
#include <cstdlib>

// OS Specific sleep
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void mySleep(unsigned long milliseconds)
{
#ifdef _WIN32
	Sleep(milliseconds); // 100 ms
#else
	usleep(milliseconds * 1000); // 100 ms
#endif
}

bool Ready(robotarm::Robot_Set_Ready::Request &req, robotarm::Robot_Set_Ready::Response &res)
{
	ros::NodeHandle n;
	ros::ServiceClient client = n.serviceClient<robotarm::Robot_GoTo>("Robot_GoTo");
	robotarm::Robot_GoTo srv;
	srv.request.angle1 = 0;
	srv.request.angle2 = -40;
	srv.request.angle3 = 80;
	srv.request.angle4 = -45;
	srv.request.angle5 = 0;
	srv.request.angle6 = 30;
	srv.request.inTime = req.inTime;
	if (client.call(srv))
	{

	}
	return true;
}

bool Park(robotarm::Robot_Set_Park::Request &req, robotarm::Robot_Set_Park::Response &res)
{
	ros::NodeHandle n;
	ros::ServiceClient client = n.serviceClient<robotarm::Robot_GoTo>("Robot_GoTo");
	robotarm::Robot_GoTo srv;
	srv.request.angle1 = 0;
	srv.request.angle2 = -45;
	srv.request.angle3 = 110;
	srv.request.angle4 = -68;
	srv.request.angle5 = 0;
	srv.request.angle6 = 5;
	srv.request.inTime = req.inTime;
	if (client.call(srv))
	{

	}
	return true;
}

bool Straight(robotarm::Robot_Set_Straight::Request &req, robotarm::Robot_Set_Straight::Response &res)
{
	ros::NodeHandle n;
	ros::ServiceClient client = n.serviceClient<robotarm::Robot_GoTo>("Robot_GoTo");
	robotarm::Robot_GoTo srv;
	srv.request.angle1 = 0;
	srv.request.angle2 = 0;
	srv.request.angle3 = 0;
	srv.request.angle4 = 0;
	srv.request.angle5 = 0;
	srv.request.angle6 = 30;
	srv.request.inTime = req.inTime;
	if (client.call(srv))
	{

	}

	return true;
}

bool Test(robotarm::Robot_Set_Test::Request &req, robotarm::Robot_Set_Test::Response &res)
{
	ros::NodeHandle n;
	ros::ServiceClient client = n.serviceClient<robotarm::Robot_GoTo>("Robot_Set_Straight");
	robotarm::Robot_Set_Straight srv;
	srv.request.inTime = 1000;
	if (client.call(srv))
	{

	}

	mySleep(1500);
	ros::ServiceClient client1 = n.serviceClient<robotarm::Robot_GoTo>("Robot_Set_Ready");
	robotarm::Robot_Set_Ready srv1;
	srv1.request.inTime = 1000;
	if (client.call(srv1))
	{

	}

	mySleep(1500);
	ros::ServiceClient client2 = n.serviceClient<robotarm::Robot_GoTo>("Robot_Set_Straight");
	robotarm::Robot_Set_Straight srv2;
	srv2.request.inTime = 1000;
	if (client.call(srv2))
	{

	}

	mySleep(1500);
	ros::ServiceClient client3 = n.serviceClient<robotarm::Robot_GoTo>("Robot_Set_Park");
	robotarm::Robot_Set_Park srv3;
	srv3.request.inTime = 1000;
	if (client.call(srv3))
	{

	}

	mySleep(1500);
	ros::ServiceClient client4 = n.serviceClient<robotarm::Robot_GoTo>("Robot_Set_Straight");
	robotarm::Robot_Set_Straight srv5;
	srv.request.inTime = 1000;
	if (client.call(srv))
	{

	}

	mySleep(1500);
	ros::ServiceClient client5 = n.serviceClient<robotarm::Robot_GoTo>("Robot_Set_Park");
	robotarm::Robot_GoTo srv6;
	srv6.request.inTime = 1000;
	if (client.call(srv6))
	{

	}

	mySleep(1500);

	return true;
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "RotboArmBesturing_client");

	ros::NodeHandle n;

	ros::ServiceServer service = n.advertiseService("Robot_Set_Ready", Ready);
	ros::ServiceServer service2 = n.advertiseService("Robot_Set_Park", Park);
	ros::ServiceServer service3 = n.advertiseService("Robot_Set_Straight", Straight);
	ros::ServiceServer service4 = n.advertiseService("Robot_Set_Test", Test);

	std::cout << "Robot Thing setup & init done\n";
	ros::spin();
	return 0;
}
