/*
 * interface.hpp
 *
 *  Created on: Mar 13, 2017
 *      Author: sidney
 */

#pragma once

#include <iostream>
#include <string>
#include <sstream>

#include "constants.hpp"

using namespace std;


/**
 * This class will handle anything related to the console interface.
 */
class Interface
{
public:
	/**
	 * @brief Constructor
	 */
	Interface();

	/**
	 * @brief Destructor
	 */
	~Interface();

	/**
	 * @brief Function that will read the input and will then check if it is correct and translate the input to a pair filled with the choices of the user.
	 * It calls @see input() first to get the input from the user. This is not passed to the function.
	 * When 'e' is put in, it will return 1
	 * It supports a batch mode and accepts a line from a file.
	 * @see input()
	 * @param line string with the read line
	 * @param batch parameter to set batch mode
	 * @return handling code
	 */
	int8_t parser(bool first_time);

	/**
	 * @brief This function will serve the user a form to fill in and will call the parser function afterwards.
	 * @see parser()
	 * @return handling code
	 */
	int8_t await_input(bool first_time);

	/**
	 * @brief getter for the specification.
	 * @return specification pair filled with shape and colour
	 */
	uint8_t get_specification() const;

private:
	/**
	 * @brief Function to wait and extract input data from user through the console.
	 * @return string of input characters
	 */
	string input();

	/**
	 * @brief Pair contains info about the chosen shape (first value) and the colour (second value).
	 */
	uint8_t specification;
};
