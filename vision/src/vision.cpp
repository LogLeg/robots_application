/*
 * vision.cpp
 *
 *  Created on: Mar 14, 2017
 *      Author: sidney
 */

#include "vision.hpp"

Vision::Vision() : src(Size(640, 480), CV_8UC3),
				   output(Size(1280, 960), CV_8UC3)
{
	set_colour_values();
}

Vision::~Vision()
{

}

void Vision::initialize(const string& a_window_name, uint8_t device, bool test)
{
	window_name = a_window_name;

	vector<vector<Point>> red_rectangles;
	vector<vector<Point>> black_rectangles;
	uint8_t counter = 0;

	namedWindow(window_name, CV_WINDOW_AUTOSIZE);

	if(!test)
	{
		if(!cap.open(device))
		{
			return;
		}

		cap >> src;

		// Loop through six configurations
		//while(calibrate(src) < 6);
	}
	else
	{
		src = imread("tabletop.jpg", 1);

		if(src.empty())
		{
			cout << "cannot acces image" << endl;
			return;
		}
	}

	take_frame(true);
	red_rectangles = find_shape(RECTANGLE, filter_colour(src, colours.at(RED)));
	black_rectangles = find_shape(RECTANGLE, filter_colour(src, colours.at(BLACK)));


	for(size_t i = 0; i < red_rectangles.size(); i++)
	{
		Properties red_rectangle = get_properties(red_rectangles[i]);
		Point2d middle_red = red_rectangle.center;

		for(size_t j = 0; j < black_rectangles.size(); j++)
		{
			Properties black_rectangle = get_properties(black_rectangles[i]);
			Point2d middle_black = black_rectangle.center;

			if(middle_black.x < middle_red.x+(red_rectangle.width/2) &&
			   middle_black.x > middle_red.x-(red_rectangle.width/2) &&
			   middle_black.y < middle_red.y+(red_rectangle.height/2) &&
			   middle_black.y > middle_red.y-(red_rectangle.height/2))
			{
				counter++;
			}
		}
		if(counter == 2) //if this is true, the red rectangle has the 2 black rectangles in it, and is thus the rectangle for size reference
		{
			centimeter_x = get_properties(red_rectangles[i]).width;
			centimeter_y = get_properties(red_rectangles[i]).height;
		}
		else
		{
			counter = 0;
		}
	}

	cout << "x pixels: " << centimeter_x << endl;
	cout << "y pixels: " << centimeter_y << endl;

}

void Vision::show_image()
{
	take_frame(true);

	//cvtColor(colour_recognition_gray, colour_recognition_colour, CV_GRAY2BGR);

	src.copyTo(output(Rect(0, 0, 640, 480)));
	//colour_recognition_colour.copyTo(output(Rect(640, 0, 640, 480)));
	//screenshot_rgb_copy.copyTo(output(Rect(0, 480, 640, 480)));
	//information.copyTo(output(Rect(640, 480, 640, 480)));

	imshow(window_name, output);


	waitKey(1);
}

void Vision::take_frame(bool screenshot)
{
	if(!screenshot)
	{
		cap >> src;
	}
	else
	{
		screenshot_rgb = src;
	}
}

Mat1b Vision::filter_colour(const Mat& input, const Colour& colour)
{
	Mat pyrdown;
	Mat pyrup;
	Mat1b binairy_mat;

	pyrDown(input, pyrdown, Size(320, 240));

	pyrUp(pyrdown, pyrup, Size(640, 480));


	inRange(pyrup, Scalar(colour.hue_min, colour.sat_min, colour.val_min),
				   Scalar(colour.hue_max, colour.sat_max, colour.val_max), binairy_mat);

	return binairy_mat;
}

vector<vector<Point>> Vision::find_shape(uint8_t shape, const Mat1b& input)
{
	vector<vector<Point>> contours;

	if(shape == CIRCLE)
	{
		//detect_circle()
	}
	else if(shape == RECTANGLE)
	{
		contours = detect_rectangle(input);
	}

	return contours;
}

vector<vector<Point>> Vision::detect_rectangle(const Mat1b& input)
{
	vector<vector<Point>> contours;
	vector<vector<Point>> contours_to_return;
	vector<Vec4i> hierarchy;
	Mat canny_output;

	Canny(input, canny_output, 100, 200, 3 );

	//pyrUp(canny_output, pyrup_mat, Size(640, 480));

	/// Find contours
	findContours(input, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	// These variables are dependent on the size of the contours vector, thats why they are not at the top of the function.
	vector<Point> contours_approxPoly(contours.size());

	//Loop through all the detected shapes.
	for(size_t i = 0; i < contours.size(); i++)
	{
		// Here the count of corners actually happens.
		approxPolyDP(Mat(contours[i]), contours_approxPoly, arcLength(Mat(contours[i]), true)*0.02, true);

		// Only check what a shape is when the contour is fully connected from begin to end and when the area is larger that 100.
		if(   fabs(contourArea(Mat(contours_approxPoly))) > 100
		   && isContourConvex(Mat(contours_approxPoly))
		   && contours_approxPoly.size() == 4
		   && check_rectangle(contours[i], contours_approxPoly))
		{
			contours_to_return.push_back(contours.at(i));
			cout << "been here" << endl;
		}
	}
	return contours_to_return;
}

bool Vision::check_rectangle(const vector<Point>& contour, const vector<Point>& contours_approxPoly)
{
	double max_cos = 0;

	for(int i = 2; i < 5; i++ )
	{
		// calculate the cosine from the angle of the connected corners
		double cosine = fabs(angle(contours_approxPoly[i%4], contours_approxPoly[i-2], contours_approxPoly[i-1]));

		if(max_cos < cosine)
		{
			max_cos = cosine;
		}
	}

	//if the cosine is small enough, the angle of a corner is near 90 degrees.
	if(max_cos < 0.1)
	{
		// Create a rotatedRect around the detected shape
		RotatedRect rectangle = minAreaRect(contour);
		Point2f corners[4];
		rectangle.points(corners);

		// Get the width and height of the rotatedRect
		double width = norm(corners[0] - corners[1]);
		double height = norm(corners[0] - corners[3]);

		// If the aspect ratio of the shape is near zero, it must be a square, if not, must be a bar
		if(fabs(width/height) - 1 < 0.2)
		{
			return true;
		}
	}
	return false;
}

Properties Vision::get_properties(const vector<Point>& contour)
{
	Properties shape;
	Moments mu;

	// Create a rotatedRect around the detected shape
	RotatedRect rectangle = minAreaRect(contour);
	Point2f corners[4];
	rectangle.points(corners);

	// Get the width and height of the rotatedRect
	shape.width = norm(corners[0] - corners[1]);
	shape.height = norm(corners[0] - corners[3]);

	mu = moments(contour, false);

	shape.center = Point2d(mu.m10/mu.m00, mu.m01/mu.m00);

	return shape;
}

/*
uint8_t Vision::calibrate(const Mat& input)
{
	static uint8_t number_of_calibrations = 0;
	Mat out;
	Point line(30, 30);

	createTrackbar("hmin", window_name, 0, 180);
	createTrackbar("smin", window_name, 0, 255);
	createTrackbar("vmin", window_name, 0, 255);
	createTrackbar("h", window_name, 0, 180);
	createTrackbar("s", window_name, 0, 255);
	createTrackbar("v", window_name, 0, 255);

	int hmin = getTrackbarPos("hmin", window_name);
	int smin = getTrackbarPos("smin", window_name);
	int vmin = getTrackbarPos("vmin", window_name);
	int h = getTrackbarPos("h", window_name);
	int s = getTrackbarPos("s", window_name);
	int v = getTrackbarPos("v", window_name);

	cvtColor(input, out, CV_RGB2HSV);

	inRange(out, Scalar(hmin, smin, vmin), Scalar(h, s, v), colour_recognition_gray);

	putText(information, "calibrate the following colour:", line, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0));
	line.y = 60;
	putText(information, colours.at(number_of_calibrations).name, line, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0));
	line.y = 90;
	putText(information, "press 'a' to continue", line, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0));

	if(waitKey(1) == 97) // press a to continue
	{
		colours.at(number_of_calibrations).hue_min = hmin;
		colours.at(number_of_calibrations).sat_min = smin;
		colours.at(number_of_calibrations).val_min = vmin;
		colours.at(number_of_calibrations).hue_max = h;
		colours.at(number_of_calibrations).sat_max = s;
		colours.at(number_of_calibrations).val_max = v;

		destroyAllWindows();
		information = Mat::zeros(src.size(), CV_8UC3);
		number_of_calibrations++;
	}

	show_image();

	return number_of_calibrations;
}
*/
void Vision::set_colour_values()
{

	white.hue_min = 0;
	white.sat_min = 0;
	white.val_min = 0;
	white.hue_max = 180;
	white.sat_max = 72;
	white.val_max = 182;

	black.hue_min = 0;
	black.sat_min = 0;
	black.val_min = 0;
	black.hue_max = 20;
	black.sat_max = 30;
	black.val_max = 30;

	red.hue_min = 116;
	red.sat_min = 0;
	red.val_min = 0;
	red.hue_max = 124;
	red.sat_max = 255;
	red.val_max = 255;

	green.hue_min = 45;
	green.sat_min = 52;
	green.val_min = 27;
	green.hue_max = 85;
	green.sat_max = 255;
	green.val_max = 255;

	blue.hue_min = 0;
	blue.sat_min = 84;
	blue.val_min = 11;
	blue.hue_max = 31;
	blue.sat_max = 214;
	blue.val_max = 120;

	yellow.hue_min = 85;
	yellow.sat_min = 41;
	yellow.val_min = 11;
	yellow.hue_max = 109;
	yellow.sat_max = 150;
	yellow.val_max = 150;

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
/*
bool Vision::detect(const pair<uint8_t, uint8_t>& specification, bool batch)
{
	if(detect_colour(specification.second) && detect_shape(specification.first))
	{
		draw_data(batch);
		return true;
	}
	return false;
}





clock_t Vision::get_total_detection_time()
{
	return colour_time + shape_time;
}

bool Vision::detect_colour(uint8_t colour)
{
	int pixel_count = 0;

	Colour spec_colour = colours.at(colour);

	//do this swapping of mats so that the final mat that is being printed to the screen is actually a still image
	cvtColor(screenshot_rgb, screenshot_bgr, CV_RGB2BGR);
	cvtColor(screenshot_bgr, screenshot_rgb_copy, CV_RGB2BGR);

	// start time measurement
	counter.start_count();

	// Convert to HSV to get better colour separating.
	cvtColor(screenshot_rgb, screenshot_hsv, CV_RGB2HSV);

	// return a Mat filled with only the spots where the colour was seen.
	inRange(screenshot_hsv, Scalar(spec_colour.hue_min, spec_colour.sat_min, spec_colour.val_min),
			Scalar(spec_colour.hue_max, spec_colour.sat_max, spec_colour.val_max), colour_recognition_gray);

	// Loop through the whole Mat and check if there are enough pixels within the colour range. If so, the colour is detected.
	for(uint16_t i = 0; i < colour_recognition_gray.rows; i++)
	{
		for(uint16_t j = 0; j < colour_recognition_gray.cols; j++)
		{
			if(colour_recognition_gray.at<uint16_t>(i, j) != 0)
			{
				pixel_count += 1;
			}

			if(pixel_count >= 1000)
			{
				colour_time = counter.end_count();
				return true;
			}
		}
	}

	colour_time = counter.end_count();
	return false;
}

bool Vision::detect_shape(uint8_t shape)
{
	Mat pyrdown_mat;
	Mat pyrup_mat;
	Mat canny_output;
	Mat drawing = Mat::zeros( colour_recognition_gray.size(), CV_8UC3 );
	//vector<vector<Point>> contours;
	uint8_t detected_shape = NO_SHAPE;
	uint8_t actual_contour_counter = 0;

	counter.start_count();

	/* Here the image gets downscaled and after the canny it gets upscaled again.
	 * This is done so that the noise is cut out, and possibly so that the canny will take less time to compute. This is not tested.
	 */
/*
	pyrDown(colour_recognition_gray, pyrdown_mat, Size(320, 240));

	/// Detect edges using canny
	Canny(pyrdown_mat, canny_output, 100, 200, 3 );

	pyrUp(canny_output, pyrup_mat, Size(640, 480));

	/// Find contours
	findContours(colour_recognition_gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	// These variables are dependent on the size of the contours vector, thats why they are not at the top of the function.
	vector<Point> contours_approxPoly(contours.size());
	vector<Rect> bound_rect(contours.size());

	//Loop through all the detected shapes.
	for(size_t i = 0; i < contours.size(); i++)
	{
		// Here the count of corners actually happens.
		approxPolyDP(Mat(contours[i]), contours_approxPoly, arcLength(Mat(contours[i]), true)*0.02, true);

		// Only check what a shape is when the contour is fully connected from begin to end and when the area is larger that 100.
		if(fabs(contourArea(Mat(contours_approxPoly))) > 100 &&	isContourConvex(Mat(contours_approxPoly)))
		{
			/* When there are 3 corners, assume the shape is a triangle, when there are 4, check if the corners are 90 degrees,
			 * when its something else, check if the shape is either a full or a half circle.
			 */
/*
			if(contours_approxPoly.size() == 3)
			{
				detected_shape = TRIANGLE;
			}
			else if(contours_approxPoly.size() == 4)
			{
				detected_shape = detect_rectangles(contours_approxPoly, i);
			}
			else
			{
				detected_shape = detect_circles(pyrup_mat, i);
			}

			/*
			// This is used to label the shapes.
			switch(detected_shape)
			{
			case CIRCLE:
				text = "circle";
				break;
			case CIRCLE_HALF:
				text = "half circle";
				break;
			case RECTANGLE:
				text = "rectangle";
				break;
			case BAR:
				text = "bar";
				break;
			case TRIANGLE:
				text = "triangle";
				break;
			case NO_SHAPE:
				text = "could not detect a shape in here";
				break;
			}
			*/
/*

			// stop the loop when the first preferred shape is found
			if(shape == detected_shape)
			{
				actual_contour = actual_contour_counter;
				i = contours.size();
			}
			else
			{
				actual_contour = -1;
			}
		}
		actual_contour_counter++;
	}
	if(shape == detected_shape)
	{
		shape_time = counter.end_count();
		return true;
	}
	shape_time = counter.end_count();
	return false;
}

void Vision::draw_data(bool batch)
{
	Scalar green = Scalar(0, 255, 0);
	Scalar white = Scalar(255, 255, 255);
	string text = "";
	Moments mu;
	stringstream stream;
	Point line(30, 30);

	//clear the mat so that text will show properly.
	information = Mat::zeros(information.size(), CV_8UC3);

	if(actual_contour != -1)
	{
		//draw the contours around the shape.
		drawContours(screenshot_rgb_copy, contours, actual_contour, green, 2, 8, hierarchy, 0, Point());
		//get the moments for this contour.

		mu = moments(contours[actual_contour], false);

		// Get the mass centers so that the label will be at the center of every shape.
		Point2f mc;

		mc = Point2d(mu.m10/mu.m00, mu.m01/mu.m00);

		stream << "x: " << mc.x << " ";
		stream << "y: " << mc.y;

		text = stream.str();

		putText(screenshot_rgb_copy, text, mc, FONT_HERSHEY_SIMPLEX, 1, white);

		int area = cvRound(contourArea(contours[actual_contour]));

		if(batch)
		{
			cout << "The area of the shape is: " << area << endl;
		}
		else
		{
			stream.str(string());

			stream << "The area of the shape is: " << area;

			text = stream.str();

			putText(information, text, line, FONT_HERSHEY_SIMPLEX, 1, white);
		}

	}

	if(batch)
	{
		cout << "The total computing time is: " << get_total_detection_time() << endl;
	}
	else
	{
		stream.str(string());

		stream << "The total computing time is: " << get_total_detection_time();

		text = stream.str();

		line.y = 60;

		putText(information, text, line, FONT_HERSHEY_SIMPLEX, 1, white);
	}

}

uint8_t Vision::detect_rectangles(const vector<Point>& contours_approxPoly, const size_t& iterator)
{
	double max_cos = 0;

	for(int j = 2; j < 5; j++ )
	{
		// calculate the cosine from the angle of the connected corners
		double cosine = fabs(angle(contours_approxPoly[j%4], contours_approxPoly[j-2], contours_approxPoly[j-1]));

		if(max_cos < cosine)
		{
			max_cos = cosine;
		}
	}

	//if the cosine is small enough, the angle of a corner is near 90 degrees.
	if(max_cos < 0.1)
	{
		// Create a rotatedRect around the detected shape
		RotatedRect rectangle = minAreaRect(contours[iterator]);
		Point2f corners[4];
		rectangle.points(corners);

		// Get the width and height of the rotatedRect
		double width = norm(corners[0] - corners[1]);
		double height = norm(corners[0] - corners[3]);

		// If the aspect ratio of the shape is near zero, it must be a square, if not, must be a bar
		if(fabs(width/height) - 1 < 0.2)
		{
			return RECTANGLE;
		}
		else
		{
			return BAR;
		}
	}
	else if(max_cos > 0.1 && max_cos < 0.9) //might still be a triangle
	{
		return TRIANGLE;
	}
	return NO_SHAPE;
}


uint8_t Vision::detect_circles(const Mat& input, const size_t& iterator)
{
	// Put a rectangle around the detected shape.
	// This time not a rotatedRect since a circle has the same width and height at any orientation.
	Rect rectangle = boundingRect(contours[iterator]);
	int height = rectangle.height;
	int width = rectangle.width;

	//get the actual area of the shape and also a calculated area of a circle with the same diameter.
	double contour_area = contourArea(contours[iterator]);
	double circle_area = (M_PI * pow(rectangle.height/2, 2));

	// Check if detected shape has almost the same area as a calculated circle with the same diameter.
	// Also check if the circle is not an ellipse or something like that.
	if(fabs(1 - (contour_area/circle_area)) < 0.1 && fabs((width/height) -1) < 0.1)
	{
		return CIRCLE;
	}
	// If the shape was in no way a circle, it might still be half of a circle.
	else
	{
		//if this does not find a half cicle, NO_SHAPE will be returned, no need to put an extra return after here.
		return hough_circle(input);
	}
}

uint8_t Vision::hough_circle(const Mat& input)
{
	/* To detect half of a circle, another method than just findContours is needed. houghCircles creates a vector of circles.
	 * to get houghCircles to work, it needs a gray Mat from the source, so another color conversion is needed. This increases
	 * the complexity of the program but it is mandatory to detect half of a circle.
	 */
/*
	cvtColor(screenshot_rgb, input_gray, CV_RGB2GRAY);
	vector<Vec3f> circles;

	HoughCircles(input_gray, circles, CV_HOUGH_GRADIENT, 1, 60, 200, 20, 0, 0);

	/* Here a distanceTransform mat is created. In here, the distance to the nearest zero pixel is calculated.
	 * This is needed to later check if the calculated point in a circle matches the detected shape.
	 */
/*
	cv::Mat dt;
	cv::distanceTransform(255-(input>0), dt, CV_DIST_L2 ,3);

	// For all circles that have been found, check if calculated points match up with the actual shape.
	for( size_t j = 0; j < circles.size(); j++ )
	{
		// These variables are used to later determine the percentage of how much a shape is an actual circle.
		unsigned int counter = 0;
		unsigned int detected = 0;

		// Get the center of the circle and also the radius.
		cv::Point center(cvRound(circles[j][0]), cvRound(circles[j][1]));
		float radius = circles[j][2];

		// This is the distance between points that will be calculated to detect if the point is on the detected shape.
		float detector_distance = radius/25;

		/* Loop through the circle with steps of 0.1. for every step, increase the counter and check if
		 * the calculated point is at the edge of a shape (I want a circle here). if so, add one to the detected variable.
		 */
/*
		for(double k = 0; k < 2*M_PI; k += 0.1)
		{
			counter++;
			double cX = radius*cos(k) + circles[j][0];
			double cY = radius*sin(k) + circles[j][1];

			if(dt.at<float>(cY,cX) < detector_distance)
			{
				detected++;
			}
		}

		// calculate the percentage and when its more than 35 and less than 65, assume the shape is half a circle.
		float percentage = 100*((float)detected/(float)counter);

		if(percentage < 65 && percentage > 35)
		{
			return CIRCLE_HALF;
		}
	}
	return NO_SHAPE;
}
*/
double Vision::angle(const Point& pt1, const Point& pt2, const Point& pt0)
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    // return the angle between two lines.
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2));
}

