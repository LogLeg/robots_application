/*
 * interface.cpp
 *
 *  Created on: Mar 13, 2017
 *      Author: sidney
 */

#include "interface.hpp"

Interface::Interface()
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
	uint8_t spec = 0;

	user_input = input();

	if(user_input[0] != 'e')
	{


		if(user_input.length() > 1)
		{
			cout << "the input is invalid. Too much information" << endl << endl;
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
			spec = user_input[0];
		}

		specification = spec;

		//print_specification();
	}
	else
	{
		return 1;
	}
	return 0;
}

int8_t Interface::await_input(bool first_time)
{

	//TODO edit this text
	cout << "Please specify the desired shape and colour that needs to be detected." << endl <<
	"shapes are: 'c' (circle), 'h' (half circle), 'r' (rectangle), 'b' (bar), 't' (triangle)" << endl <<
	"colours are: 'w'(white), 'd' (black), 'r' (red), 'g' (green), 'b' (blue), 'y' (yellow)" << endl <<
	"fill in 'e' to exit" << endl <<
	"format is: [shape][space][colour]" << endl << endl;


	if(parser(first_time) == 1)
	{
		return 1;
	}

	return 0;
}

void Interface::print_specification()
{
	uint8_t shape_spec = specification;
	uint8_t colour_spec = specification;

	string shape = "";
	string colour = "";

	switch(shape_spec)
		{
		case CIRCLE:
			shape = "Circle";
			break;
		case RECTANGLE:
			shape = "Rectangle";
			break;
		case BAR:
			shape = "Bar";
			break;
		}

		switch(colour_spec)
		{
		case WHITE:
			colour = "White";
			break;
		case BLACK:
			colour = "Black";
			break;
		case RED:
			colour = "Red";
			break;
		case GREEN:
			colour = "Green";
			break;
		case BLUE:
			colour = "blue";
			break;
		case YELLOW:
			colour = "Yellow";
			break;
		}

	cout << "shape is: " << shape << " colour is: " << colour << endl << endl;
}

uint8_t Interface::get_specification() const
{
	return specification;
}


