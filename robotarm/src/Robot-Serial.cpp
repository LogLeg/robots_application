#include "Robot-Serial.hpp"

RobotSerial::RobotSerial(std::string aPort, unsigned long aBaud):
			port(aPort),
			baud(aBaud)//,
			//my_serial(port, baud, serial::Timeout::simpleTimeout(1000))
	
{
	my_serial = new serial::Serial(port, static_cast<uint32_t>(baud),serial::Timeout::simpleTimeout(1000));
	my_serial->setTimeout(serial::Timeout::max(), 250, 0, 250, 0);
	std::cout << "Is the serial port open?";
	if(my_serial->isOpen())
	{	
		std::cout << " Yes." << std::endl;
	}	
	else
	{	
		std::cout << " No." << std::endl;  
	}
}

RobotSerial::~RobotSerial()
{
	
}


void RobotSerial::send(std::string Message)
{
	my_serial->write(Message);
}


std::string RobotSerial::read(uint32_t length)
{	
	std::string ReadString = "";
	my_serial->read(ReadString, length);
	return ReadString;
}
