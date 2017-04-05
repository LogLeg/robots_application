/*
 * interface.cpp
 *
 *  Created on: Mar 13, 2017
 *      Author: sidney
 */

#include "interface.hpp"

Interface::Interface() : specification(0)
{

}

Interface::~Interface()
{

}

string Interface::input()
{
	string input = "";
	getline(cin, input);

	return input;
}

int8_t Interface::parser(bool first_time)
{
	string user_input = "";
	int spec = 0;

	user_input = input();

	if(user_input[0] != 'e')
	{


		if(user_input.length() > 1)
		{
			cout << "the input is invalid. Too much information" << endl << endl;
			spec = NO_COLOUR;
			return 0;
		}

		if(first_time)
		{
			switch(user_input[0])
			{
			case 'w':
				spec = WHITE;
				break;
			case 'd':
				spec = BLACK;
				break;
			case 'r':
				spec = RED;
				break;
			case 'g':
				spec = GREEN;
				break;
			case 'b':
				spec = BLUE;
				break;
			case 'y':
				spec = YELLOW;
				break;
			default:
				cout << user_input[0] << " This is not a known colour!" << endl << endl;
				return 0;
			}
		}
		else
		{
			spec = user_input[0] - '0';
		}

		specification = (uint8_t)spec;

		cout << "spec = " << spec << endl;
	}
	else
	{
		return 1;
	}
	return 0;
}

int8_t Interface::await_input(bool first_time)
{
	if(first_time)
	{
		cout << "Please choose the colour of the object to grab." << endl <<
				"colours are: 'd' (black), 'r' (red), 'g' (green), 'b' (blue), 'y' (yellow)" << endl <<
				"fill in 'e' to exit" << endl << endl;
	}
	else
	{
		cout << "Please choose the number of the detected objects." << endl << endl;
	}


	if(parser(first_time) == 1)
	{
		return 1;
	}

	return 0;
}

uint8_t Interface::get_specification() const
{
	return specification;
}


