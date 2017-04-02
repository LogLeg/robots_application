/*
 * vision.hpp
 *
 *  Created on: Mar 14, 2017
 *      Author: sidney
 */

#pragma once

#include <iostream>
#include <cstdint>
#include <cmath>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include "interface.hpp"

using namespace std;
using namespace cv;

/**
* @brief Struct for the colour thresholds
* @param hue_min the minimal hue
* @param sat_min the minimal saturation
* @param val_min the minimal value
* @param hue_max the maximal hue
* @param sat_max the maximal saturation
* @param val_max the maximal value
*/
struct Colour
{
	int hue_min;
	int sat_min;
	int val_min;
	int hue_max;
	int sat_max;
	int val_max;
	string name;
};

/**
 * @brief Struct for properties of a certain contour
 * @param center the center of a contour
 * @param width the width of a contour
 * @param height the height of a contour
 */
struct Properties
{
	Point2d center;
	int width;
	int height;
};

/**
 * The vision class does all the image recognition work. and also translates the pixels to millimeters
 */
class Vision
{
public:
	/**
	 * @ brief Constructor
	 */
	Vision();

	/**
	 * @brief Deconstructor
	 */
	~Vision();

	/**
	 * @brief This function will initialize the camera and will set additional parameters that depend on what the camera sees. Like calibrating the pixels per mm
	 * @param window_name The abaility to set a custom window name
	 * @param device the webcam that should be used
	 * @param test parameter to use a screenshot instead for testing
	 */
	void initialize(const string& window_name, uint8_t device, bool test);

	/**
	 * @brief This function will take all the loose ends of this class and combine them into one Mat so that everything is nice and tidy.
	 */
	void show_image();

	/**
	 * @brief This function will get the stream from the webcam and put it into the src Mat.
	 * @param screenshot With this parameter you can specify if you want a stream or only one frame
	 */
	void take_frame(bool screenshot);

	/**
	 * @brief This function will filter a colour out of the @see src Mat.
	 * @param colour_number This is the colour parameter
	 * @return Returns a binairy Mat to process further down the road.
	 */
	Mat1b filter_colour(uint8_t colour_number);

	/**
	 * @brief This function will find contours that meet the shape that has been given with the function. It will search all contours and then
	 * check for rectangles, bars or circles.
	 * @param input This should be the return value of @see filter_colour(uint8_t colour_number).
	 * @param shape The shape that needs to be searched for
	 * @return A vector of pairs with contours and nested contours This last one is only used by the @see get_calibration_square() function.
	 */
	vector<pair<vector<Point>, vector<vector<Point>>>> detect_shape(const Mat1b& input, uint8_t shape);

	/**
	 * @brief This function will return the properties of a given contour
	 * @param contour The contour that needs to be documented in properties
	 * @return The properties of the contour
	 */
	Properties get_properties(const vector<Point>& contour);

	/**
	 * @brief This function will transform the x and y coordinate of a shape defined in properties to actual millimeters measured
	 * from the base of the robotic arm
	 * @param properties A pointer to a properties object to transform.
	 */
	void transform_properties(Properties* properties);

	/**
	 * @brief This function will put numers on the objects that can be grabbed by the robotic arm.
	 * @param colour The colour to detect from all the objects.
	 */
	uint8_t number_selection(uint8_t colour);

	/**
	 * @brief This function will return the properties of the selected shape
	 * @param shape The number from the selection from @see number_selection(uint8_t colour)
	 * @return The properties of the shape to grab
	 */
	Properties shape2grab(uint8_t shape);

private:
	/**
	 * @brief This function will search for a specific calibration square in the picture and will adjust the pixel to mm ratio to that. It will
	 * also serve as a anchor to calculate where the robotic arm is.
	 * It will fill the @see calibration_square_properties variable.
	 * @return true if the square was found.
	 */
	bool get_calibration_square();

	/**
	 * @param pt1 point 1 from a rectangle.
	 * @param pt2 point 2 from a rectangle.
	 * @param pt1 point 0 from a rectangle. This is the point where the corner is calculated for.
	 * @return returns the angle of a corner.
	 */
	double angle(const Point& pt1, const Point& pt2, const Point& pt0);

	/**
	 * @brief This function will check if the given contour actually contains corners with 90 degrees and after that it will say if its either a
	 * rectangle or a square.
	 * @param contour The contour that needs to be checked
	 * @param contours_approxPoly The  corners of the contour
	 * @return returns the found shape.
	 */
	uint8_t check_rectangle(const vector<Point>& contour, const vector<Point>& contours_approxPoly);

	/**
	 * @brief This function detects circles. Only used to determine the target position of a shape.
	 * @param contour The contour that has been found by @see detect_shape()
	 * @return True if circle has been found
	 */
	bool detect_circle(const vector<Point>& contour);

	/**
	 * @brief This function sets all the colour values into their corresponding structs
	 */
	void set_colour_values();


	/**
	 * @brief Colour objects.
	 */
	Colour white;
	Colour black;
	Colour red;
	Colour green;
	Colour blue;
	Colour yellow;

	vector<vector<Point>> selection;
	vector<Point> calibration_square;
	Properties calibration_square_properties;

	/**
	 * @brief vector with the colours from above.
	 */
	vector<Colour> colours;

	/**
	 * @brief just the name of the window.
	 */
	string window_name;

	/**
	 * @brief capture object
	 */
	VideoCapture cap;

	/*
	 * @brief Mat objects to manipulate and copy into.
	 */
	Mat src;
	Mat screenshot_rgb;
	Mat output;
	Mat drawing;
	Mat1b binairy_mat_final;

	/**
	 * @brief Pixels per mm
	 */
	double pixels_per_mm;

	/**
	 * @brief translation to the robotbase in mm
	 */
	double distance_to_robotbase_x;
	double distance_to_robotbase_y;

	/**
	 * @brief maximal grabbable size in mm
	 */
	double max_grabbable_size;












	/**
	 * @brief used to calibrate the colour edges
	 * @param input Mat object to scan
	 * @return the number of calibrations done
	 */
	//uint8_t calibrate(const Mat& input);

	/**
	 * @brief This function returns the total computing time of the colour and shape detection.
	 * @return total computation time in ticks
	 */
	//clock_t get_total_detection_time();

	/**
	 * Function dedicated to finding colours
	 * @param colour parameter to select a colour
	 * @return return true when the colour is found
	 */
	//bool detect_colour(uint8_t colour);

	/**
	 * Function dedicated to finding shapes
	 * @param shape parameter to select a colour
	 * @return return true when the shape is found
	 */
	//bool detect_shape(uint8_t shape);

	/**
	 * @brief This function will draw useful data to a Mat object
	 * @param batch if batch is true, data will be written to cout instead of the Mat object
	 */
	//void draw_data(bool batch);

	/**
	 * @brief Detect rectangles and bars by checking if the corners of a shape with 4 corners are all ~90 degrees.
	 * @param contours_approxPoly vector from @see detect_shape()
	 * @param iterator iterator variable from @see detect_shape()
	 * @return shape
	 */
	//uint8_t detect_rectangles(const vector<Point>& contours_approxPoly, const size_t& iterator);

	/**
	 * @brief Detect circles and half circles by calculating the difference between the real area and a calculated area. Also uses houghCircles for
	 * detecting half circles.
	 * @param input a mat from @see detect_shape()
	 * @param iterator iterator variable from @see detect_shape()
	 * @return shape
	 */
	//uint8_t detect_circles(const Mat& input, const size_t& iterator);

	/**
	 * @brief Function dedicated to search for half circles. Calculates how much of a circle is present in a shape.
	 * @param input mat object from @see detect_shape()
	 * @return shape
	 */
	//uint8_t hough_circle(const Mat& input);




	/**
	 * @brief time variables to return the total time afterwards.
	 */
	//clock_t colour_time;
	//clock_t shape_time;




//	Mat information;

	/**
	 * @brief The position at which the desired shape is located in the contours vector
	 */
	//int8_t actual_contour;
};
