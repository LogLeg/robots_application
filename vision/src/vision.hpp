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
//#include "counter.hpp"
//#include "constants.hpp"

using namespace std;
using namespace cv;

/**
* @brief Struct for the colour thresholds
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

struct Properties
{
	Point2d center;
	int width;
	int height;
};

/**
 * The vision class does all the image recognition work.
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
	 * @brief This function will initialize the camera
	 * @param window_name The abaility to set a custom window name
	 */
	void initialize(const string& window_name, uint8_t device, bool test);

	/**
	 * @brief This function will take all the loose ends of this class and combine them into one Mat so that everyting is nice and tidy.
	 */
	void show_image();

	/**
	 * @brief This function will get the stream from the webcam and put it into the src Mat.
	 * @param screenshot With this parameter you can specify if you want a stream or only one frame
	 */
	void take_frame(bool screenshot);

	void get_calibration_square();

	Mat1b filter_colour(const Mat& input, const Colour& colour);

	vector<vector<Point>> find_shape(uint8_t shape, const Mat1b& input);

	vector<vector<Point>> detect_square(const Mat1b& input, uint8_t shape);

	uint8_t check_rectangle(const vector<Point>& contour, const vector<Point>& contours_approxPoly);

	Properties get_properties(const vector<Point>& contour);

	/**
	 * @brief This function sets all the colour values into their corresponding structs
	 */
	void set_colour_values();

	/**
	 * @brief Main function to call to detect a shape and colour. Returns true when found
	 * @param specification Specification pair. shape (first), colour (second)
	 * @param batch changes the behaviour of the data stream to the user.
	 * @return returns true when the object is found
	 */
	//bool detect(const pair<uint8_t, uint8_t>& specification, bool batch);







private:

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
	 * @param pt1 point 1 from a rectangle.
	 * @param pt2 point 2 from a rectangle.
	 * @param pt1 point 0 from a rectangle. This is the point where the corner is calculated for.
	 * @return returns the angle of a corner.
	 */
	double angle(const Point& pt1, const Point& pt2, const Point& pt0);

	/**
	 * @brief Colour objects.
	 */
	Colour white;
	Colour black;
	Colour red;
	Colour green;
	Colour blue;
	Colour yellow;

	int x_factor;
	int y_factor;

	vector<Point> calibration_square;
	Properties calibration_square_properties;

	/**
	 * @brief time variables to return the total time afterwards.
	 */
	//clock_t colour_time;
	//clock_t shape_time;

	/**
	 * @brief vector with the colours from above.
	 */
	vector<Colour> colours;

	/**
	 * @brief contour & hierarchy vector
	 */
	//vector<vector<Point>> contours;
	//vector<Vec4i> hierarchy;

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
//	Mat screenshot_rgb_copy;
//	Mat screenshot_bgr;
//	Mat screenshot_hsv;
	Mat output;
//	Mat input_gray;
//	Mat colour_recognition_colour;
	Mat1b binairy_mat_final;
//	Mat information;

	/**
	 * @brief The position at which the desired shape is located in the contours vector
	 */
	//int8_t actual_contour;
};
