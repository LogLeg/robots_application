/*
 * vision.cpp
 *
 *  Created on: Mar 14, 2017
 *      Author: sidney
 */

#include "vision.hpp"

Vision::Vision() :
		src(Size(640, 480), CV_8UC3),
		feed_gray(Size(640, 480), CV_8UC1),
		screenshot_rgb(Size(640, 480), CV_8UC3),
		output(Size(1280, 480), CV_8UC3),
		drawing(Size(640, 480), CV_8UC3),
		binairy_mat_final(Size(640, 480), CV_8UC1),
		mm_per_pixel(0),
		distance_to_robotbase_x(0),
		distance_to_robotbase_y(187),
		max_grabbable_size(30)
{
	calibration_square_properties.center.x = 0;
	calibration_square_properties.center.y = 0;
	calibration_square_properties.width = 0;
	calibration_square_properties.height = 0;
	calibration_square_properties.angle = 0;

	//This is done so that the colours are already filled in if the user skips the calibration.
	set_colour_values();
}

Vision::~Vision()
{

}

uint8_t Vision::calibrate(const Mat& input)
{
	static uint8_t number_of_calibrations = 0;
	Mat out;

	int hmin = 0;
	int smin = 0;
	int vmin = 0;
	int h = 0;
	int s = 0;
	int v = 0;

	cout << "please select the " << colours.at(number_of_calibrations).name << " colour" << endl;

	createTrackbar("hmin", window_name, &colours.at(number_of_calibrations).hue_min, 180);
	createTrackbar("smin", window_name, &colours.at(number_of_calibrations).sat_min, 255);
	createTrackbar("vmin", window_name, &colours.at(number_of_calibrations).val_min, 255);
	createTrackbar("h", window_name, &colours.at(number_of_calibrations).hue_max, 180);
	createTrackbar("s", window_name, &colours.at(number_of_calibrations).sat_max, 255);
	createTrackbar("v", window_name, &colours.at(number_of_calibrations).val_max, 255);

	hmin = getTrackbarPos("hmin", window_name);
	smin = getTrackbarPos("smin", window_name);
	vmin = getTrackbarPos("vmin", window_name);
	h = getTrackbarPos("h", window_name);
	s = getTrackbarPos("s", window_name);
	v = getTrackbarPos("v", window_name);

	cvtColor(input, out, CV_RGB2HSV);

	inRange(out, Scalar(hmin, smin, vmin), Scalar(h, s, v), feed_gray);

	if (waitKey(1) == 97) // press a to continue
	{
		colours.at(number_of_calibrations).hue_min = hmin;
		colours.at(number_of_calibrations).sat_min = smin;
		colours.at(number_of_calibrations).val_min = vmin;
		colours.at(number_of_calibrations).hue_max = h;
		colours.at(number_of_calibrations).sat_max = s;
		colours.at(number_of_calibrations).val_max = v;

		destroyAllWindows();
		feed_gray = Mat::zeros(src.size(), CV_8UC1);
		number_of_calibrations++;
	}
	else if (waitKey(1) == 98) //press a to stop
	{
		destroyAllWindows();
		feed_gray = Mat::zeros(src.size(), CV_8UC1);
		number_of_calibrations = 7;
	}

	cvtColor(feed_gray, screenshot_rgb, CV_GRAY2BGR);

	show_image();

	return number_of_calibrations;
}

void Vision::initialize(const string& a_window_name, uint8_t device, bool test)
{
	window_name = a_window_name;

	namedWindow(window_name, CV_WINDOW_AUTOSIZE);

	if (!test)
	{
		if (!cap.open(device))
		{
			cout << "could not open device" << endl;
			return;
		}

		cap >> src;
	}
	else
	{
		src = imread("tabletop1.jpg", 1);

		if (src.empty())
		{
			cout << "cannot acces image" << endl;
			return;
		}
	}

	// Loop through six configurations
	while (calibrate(src) < 6);

	if (!get_calibration_square())
	{
		cout << "the calibration square was not found. Please restart the program" << endl;
	}
}

void Vision::show_image()
{
	take_frame(false);

	src.copyTo(output(Rect(0, 0, 640, 480)));
	screenshot_rgb.copyTo(output(Rect(640, 0, 640, 480)));

	imshow(window_name, output);

	waitKey(1);
}

void Vision::take_frame(bool screenshot)
{
	Mat HSV;

	if (!screenshot)
	{
		cap >> src;
	}
	else
	{
		cvtColor(src, HSV, CV_RGB2HSV);
		cvtColor(HSV, screenshot_rgb, CV_HSV2RGB);
	}
}

Mat1b Vision::filter_colour(uint8_t colour_number)
{
	Mat blur_mat;
	Mat1b binairy_mat;
	Mat input_hsv(Size(640, 480), CV_8UC3);
	Colour colour = colours.at(colour_number);

	cvtColor(screenshot_rgb, input_hsv, CV_RGB2HSV);

	blur(input_hsv, blur_mat, Size(5, 5), Size(-1, -1), BORDER_DEFAULT);

	inRange(blur_mat, Scalar(colour.hue_min, colour.sat_min, colour.val_min),
					  Scalar(colour.hue_max, colour.sat_max, colour.val_max), binairy_mat);

	//TEMP
	//binairy_mat_final = binairy_mat;

	return binairy_mat;
}

vector<pair<vector<Point>, vector<vector<Point>>>> Vision::detect_shape(const Mat1b& input, uint8_t shape)
{
	vector<pair<vector<Point>, vector<vector<Point>>>> pairs;
	vector<vector<Point>> contours;
	vector<Point> square_contour;
	vector<vector<Point>> nested_contours;
	vector<Vec4i> hierarchy;
	int counter = 0;

	/// Find contours
	findContours(input, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	// These variables are dependent on the size of the contours vector, thats why they are not at the top of the function.
	vector<Point> contours_approxPoly(contours.size());

	//Loop through all the detected shapes.
	for(size_t i = 0; i < contours.size(); i++)
	{

		// Here the count of corners actually happens.
		approxPolyDP(Mat(contours.at(i)), contours_approxPoly, arcLength(Mat(contours[i]), true)*0.02, true);

		for(size_t j = 0; j < contours_approxPoly.size(); j++)
		{
			circle(screenshot_rgb, contours_approxPoly.at(j), 3, Scalar(0, 0, 255));
		}
		drawContours(screenshot_rgb, contours, counter, Scalar(0, 255, 0), 1, 8, hierarchy, 1, Point());

		// Only check what a shape is when the contour is fully connected from begin to end and when the area is larger that 100.
		if( fabs(contourArea(Mat(contours_approxPoly))) > 10
				&& isContourConvex(Mat(contours_approxPoly))
				&& contours_approxPoly.size() == 4)
		{
			if(check_rectangle(contours.at(i), contours_approxPoly) == shape)
			{
				square_contour = contours.at(i);   //.push_back(contours.at(i));
				for(size_t j = 0; j < hierarchy.size(); j++)
				{
					if(hierarchy[j][3] == counter)
					{
						if(hierarchy[j][0] == -1 && hierarchy[j][1] != -1)
						{
							nested_contours.push_back(contours.at(hierarchy[j][1]));
						}
						else if(hierarchy[j][0] != -1 && hierarchy[j][1] == -1)
						{
							nested_contours.push_back(contours.at(hierarchy[j][0]));
						}
					}
				}
				pairs.push_back(make_pair(square_contour, nested_contours));
			}
		}
		else
		{
			if(shape == CIRCLE)
			{
				if(detect_circle(contours[i]))
				{
					pairs.push_back(make_pair(contours[i], nested_contours));
				}
				if(pairs.size() > 1)
				{
					cout << "could not find a proper target" << endl;
				}
			}
		}
		counter++;
	}

	return pairs;
}

Properties Vision::get_properties(const vector<Point>& contour)
{
	Properties shape;

	// Create a rotatedRect around the detected shape
	RotatedRect rectangle = minAreaRect(contour);
	Point2f corners[4];
	rectangle.points(corners);

	shape.angle = rectangle.angle;

	shape.width = (int)rectangle.size.width;
	shape.height = (int)rectangle.size.height;

	if (shape.width > shape.height)
	{
		shape.angle += 90;
	}

	shape.center = rectangle.center;

	return shape;
}

void Vision::transform_properties(Properties* properties)
{
	double x_offset_to_calibration_square = fabs(calibration_square_properties.center.x - properties->center.x);
	double y_offset_to_calibration_square = fabs(calibration_square_properties.center.y - properties->center.y);

	x_offset_to_calibration_square /= mm_per_pixel;
	y_offset_to_calibration_square /= mm_per_pixel;

	if(properties->center.y > calibration_square_properties.center.y)
	{
		properties->center.y = (distance_to_robotbase_y - y_offset_to_calibration_square);
	}
	else
	{
		properties->center.y = (distance_to_robotbase_y + y_offset_to_calibration_square);
	}

	if (properties->center.x > calibration_square_properties.center.x)
	{
		properties->center.x = (distance_to_robotbase_x + x_offset_to_calibration_square);
	}
	else
	{
		properties->center.x = (distance_to_robotbase_x - x_offset_to_calibration_square);
	}
}

uint8_t Vision::number_selection(uint8_t colour)
{
	vector<pair<vector<Point>, vector<vector<Point>>> > pairs;
	stringstream stream;
	uint8_t size = 0;
	selection.clear();

	pairs = detect_shape(filter_colour(colour), BAR);

	for(size_t i = 0; i < pairs.size(); i++)
	{
		selection.push_back(pairs.at(i).first);
	}

	for(size_t i = 0; i < selection.size(); i++)
	{
		stream.str(string());
		size++;
		Properties properties;
		properties = get_properties(selection.at(i));
		stream << i;
		putText(screenshot_rgb, stream.str(), properties.center, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0));
	}
	return size;
}

Properties Vision::shape2grab(uint8_t shape)
{
	return get_properties(selection.at(shape));
}

bool Vision::get_calibration_square()
{
	vector<pair<vector<Point>, vector<vector<Point>>> > pairs;
	vector<Point> calibration_rectangle_contour;
	vector<vector<Point>> calibration_rectangles_nests;

	double x_factor = 0;
	double y_factor = 0;

	take_frame(true);
	pairs = detect_shape(filter_colour(BLACK), RECTANGLE);

	for(size_t i = 0; i < pairs.size(); i++)
	{
		calibration_rectangle_contour = pairs.at(i).first;
		calibration_rectangles_nests = pairs.at(i).second;

		calibration_square_properties = get_properties(calibration_rectangle_contour);

		if(calibration_rectangles_nests.size() == 2)
		{
			x_factor = calibration_square_properties.width;
			y_factor = calibration_square_properties.height;

			mm_per_pixel = ((x_factor + y_factor) / 2) / 50;

			return true;
		}
	}
	return false;
}

double Vision::angle(const Point& pt1, const Point& pt2, const Point& pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	// return the angle between two lines.
	return (dx1 * dx2 + dy1 * dy2) / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2));
}

uint8_t Vision::check_rectangle(const vector<Point>& contour, const vector<Point>& contours_approxPoly)
{
	double max_cos = 0;
	double smallest_side = 0;

	for (int i = 2; i < 5; i++)
	{
		// calculate the cosine from the angle of the connected corners
		double cosine = fabs(angle(contours_approxPoly[i % 4], contours_approxPoly[i - 2], contours_approxPoly[i - 1]));

		if (max_cos < cosine)
		{
			max_cos = cosine;
		}
	}

	//if the cosine is small enough, the angle of a corner is near 90 degrees.
	if (max_cos < 0.3)
	{
		// Create a rotatedRect around the detected shape
		RotatedRect rectangle = minAreaRect(contour);
		Point2f corners[4];
		rectangle.points(corners);

		// Get the width and height of the rotatedRect
		double width = norm(corners[0] - corners[1]);
		double height = norm(corners[0] - corners[3]);

		if (height < width)
		{
			smallest_side = height;
		}
		else
		{
			smallest_side = width;
		}

		smallest_side /= mm_per_pixel;

		// If the aspect ratio of the shape is near zero, it must be a square, if not, must be a bar
		if (fabs((width / height) - 1) < 0.1)
		{
			return RECTANGLE;
		}
		else
		{
			//if(smallest_side <= max_grabbable_size)
			//{
			return BAR;
			//}
		}
	}
	return NO_SHAPE;
}

bool Vision::detect_circle(const vector<Point>& contour)
{
	//get the actual area of the shape and also a calculated area of a circle with the same diameter.
	double contour_area = contourArea(contour);
	double circle_area = (M_PI * pow(mm_per_pixel * 50, 2));

	// Check if detected shape has almost the same area as a calculated circle with the same diameter.
	// Also check if the circle is not an ellipse or something like that.
	if (fabs(1 - (contour_area / circle_area)) < 0.2)
	{
		return true;
	}
	return false;
}

void Vision::set_colour_values()
{

	white.hue_min = 0;
	white.sat_min = 40;
	white.val_min = 150;
	white.hue_max = 180;
	white.sat_max = 80;
	white.val_max = 255;

	black.hue_min = 0;
	black.sat_min = 0;
	black.val_min = 0;
	black.hue_max = 180;
	black.sat_max = 100;
	black.val_max = 50;

	red.hue_min = 11;
	red.sat_min = 90;
	red.val_min = 42;
	red.hue_max = 126;
	red.sat_max = 239;
	red.val_max = 144;

	green.hue_min = 45;
	green.sat_min = 43;
	green.val_min = 64;
	green.hue_max = 91;
	green.sat_max = 255;
	green.val_max = 255;

	blue.hue_min = 0;
	blue.sat_min = 47;
	blue.val_min = 20;
	blue.hue_max = 16;
	blue.sat_max = 200;
	blue.val_max = 70;

	yellow.hue_min = 105;
	yellow.sat_min = 80;
	yellow.val_min = 76;
	yellow.hue_max = 120;
	yellow.sat_max = 170;
	yellow.val_max = 147;

	white.name = "white";
	black.name = "black";
	red.name = "red";
	green.name = "green";
	blue.name = "blue";
	yellow.name = "yellow";

	colours.push_back(white);
	colours.push_back(black);
	colours.push_back(red);
	colours.push_back(green);
	colours.push_back(blue);
	colours.push_back(yellow);
}

