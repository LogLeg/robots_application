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
		output(Size(1280, 480), CV_8UC3),
		binairy_mat_final(Size(640, 480), CV_8UC1),
		drawing(Size(640, 480), CV_8UC3),
		distance_to_robotbase_x(0),
		distance_to_robotbase_y(187),
		mm_per_pixel(0)
{
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

	cout << "hmin: " << hmin << endl;

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
		//boost::this_thread::sleep( boost::posix_time::seconds(1) );
		//this_thread::sleep_for(std::chrono::milliseconds(500)); // <-- change to boost thread!
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

		// Loop through six configurations
		//while(calibrate(src) < 6);
	}
	else
	{
		src = imread("tabletop3.jpg", 1);

		if (src.empty())
		{
			cout << "cannot acces image" << endl;
			return;
		}
	}

	// Loop through six configurations
	//while (calibrate(src) < 6);

	if (!get_calibration_square())
	{
		cout << "the calibration square was not found. Please restart the program" << endl;
	}

	cout << "calibrationsquare x: " << calibration_square_properties.center.x << " y: " << calibration_square_properties.center.y << endl;
	//transform_properties(&calibration_square_properties);
	cout << "calibrationsquare x: " << calibration_square_properties.center.x << " y: " << calibration_square_properties.center.y << endl;


	max_grabbable_size = 30;
}

void Vision::show_image()
{
	take_frame(false);

	//TODO temp!
	//Mat kaas;

	//cvtColor(binairy_mat_final, kaas, CV_GRAY2BGR);

	src.copyTo(output(Rect(0, 0, 640, 480)));
	//kaas.copyTo(output(Rect(640, 0, 640, 480)));
	//drawing.copyTo(output(Rect(0, 480, 640, 480)));
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

	inRange(blur_mat, Scalar(colour.hue_min, colour.sat_min, colour.val_min), Scalar(colour.hue_max, colour.sat_max, colour.val_max), binairy_mat);

	//TEMP
	binairy_mat_final = binairy_mat;

	return binairy_mat;
}

vector<pair<vector<Point>, vector<vector<Point>>> > Vision::detect_shape(const Mat1b& input, uint8_t shape)
{
	vector<pair<vector<Point>, vector<vector<Point>>>> pairs;
	vector<vector<Point>> contours;
	vector<Point> square_contour;
	vector<vector<Point>> nested_contours;
	vector<Vec4i> hierarchy;
	//Mat canny_output;
	//drawing = Mat::zeros(drawing.size(), CV_8UC3);

	//Canny(input, canny_output, 100, 200, 3 );

	//pyrUp(canny_output, pyrup_mat, Size(640, 480));

	/// Find contours
	findContours(input, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	// These variables are dependent on the size of the contours vector, thats why they are not at the top of the function.
	vector<Point> contours_approxPoly(contours.size());

	//Loop through all the detected shapes.
	for(size_t i = 0; i < contours.size(); i++)
	{
		// Here the count of corners actually happens.
		approxPolyDP(Mat(contours.at(i)), contours_approxPoly, arcLength(Mat(contours[i]), true)*0.02, true);

		//cout << "polyDP size: " << contours_approxPoly.size() << endl;

		for(size_t j = 0; j < contours_approxPoly.size(); j++)
		{
			circle(screenshot_rgb, contours_approxPoly.at(j), 3, Scalar(0, 0, 255));
		}

		// Only check what a shape is when the contour is fully connected from begin to end and when the area is larger that 100.
		if( fabs(contourArea(Mat(contours_approxPoly))) > 10
				&& isContourConvex(Mat(contours_approxPoly))
				&& contours_approxPoly.size() == 4)
		{
			cout << "i = " << i << endl;
			if(check_rectangle(contours.at(i), contours_approxPoly) == shape)
			{
				square_contour = contours.at(i);   //.push_back(contours.at(i));
				for(size_t j = 0; j < hierarchy.size(); j++)
				{
					if(hierarchy[j][3] == i)
					{
						//cout << hierarchy.at(j) << endl;
						if(hierarchy[j][0] == -1 && hierarchy[j][1] != -1)
						{
							nested_contours.push_back(contours.at(hierarchy[j][1]));
						}
						else if(hierarchy[j][0] != -1 && hierarchy[j][1] == -1)
						{
							nested_contours.push_back(contours.at(hierarchy[j][0]));
						}
						//cout << "nested contours nr: " << nested_contours.size() << endl;
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
	}

	/**
	 * TEMP
	 */
	for(int i = 0; i < contours.size(); i++)
	{
		drawContours(screenshot_rgb, contours, i, Scalar(0, 255, 0), 1, 8, hierarchy, 1, Point());

	}
//	cout << "size of contours: " << contours.size() << endl;
//	cout << "size of pairs: " << pairs.size() << endl;
//	cout << "I should have drawn something now" << endl;

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

	shape.width = rectangle.size.width;
	shape.height = rectangle.size.height;

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

//	cout << "calibration_square_properties.center.x: " << calibration_square_properties.center.x << endl;
//	cout << "properties->center.x: " << properties->center.x << endl;
//
//	cout << "x_offset_to_calibration_square: " << x_offset_to_calibration_square << endl;
//	cout << "y_offset_to_calibration_square: " << y_offset_to_calibration_square << endl;

	x_offset_to_calibration_square /= mm_per_pixel;
	y_offset_to_calibration_square /= mm_per_pixel;

//	cout << "x_offset_to_calibration_square: " << x_offset_to_calibration_square << endl;
//	cout << "y_offset_to_calibration_square: " << y_offset_to_calibration_square << endl;

	if(properties->center.y > calibration_square_properties.center.y)
	{

		properties->center.y = (distance_to_robotbase_y - y_offset_to_calibration_square);
		cout << "properties object y: " << properties->center.y << endl;

	}
	else
	{

		properties->center.y = (distance_to_robotbase_y + y_offset_to_calibration_square);
		cout << "properties object y: " << properties->center.y << endl;

	}

	if (properties->center.x > calibration_square_properties.center.x)
	{

		properties->center.x = (distance_to_robotbase_x + x_offset_to_calibration_square);
		cout << "properties object x: " << properties->center.x << endl;

	}
	else
	{
		//properties->center.x = (distance_to_robotbase_x + x_offset_to_calibration_square);
		//properties->center.y = (distance_to_robotbase_y + y_offset_to_calibration_square);
		properties->center.x = (distance_to_robotbase_x - x_offset_to_calibration_square);
		cout << "properties object x: " << properties->center.x << endl;
	}
}

uint8_t Vision::number_selection(uint8_t colour)
{
	vector<pair<vector<Point>, vector<vector<Point>>> > pairs;
	stringstream stream;
	uint8_t size = 0;
	selection.clear();

	cout << "got in the number selection" << endl;

	pairs = detect_shape(filter_colour(colour), BAR);

	cout << "pairs.size = " << pairs.size() << endl;

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
	if (shape > selection.size())
	{
		//TODO revise this!!!
		throw 0;
	}
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

			cout << "x pixels: " << x_factor << endl;
			cout << "y pixels: " << y_factor << endl;

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
	if (max_cos < 0.2)
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
//			cout << "actual outcome of calc: " << fabs((width/height) - 1) << endl;
//			cout << "width of shape: " << width << endl;
//			cout << "height of shape: " << height << endl;
//			cout << "rectangle found" << endl;
			return RECTANGLE;
		}
		else
		{
			cout << "actual outcome of calc: " << fabs((width / height) - 1) << endl;
			cout << "width of shape: " << width << endl;
			cout << "height of shape: " << height << endl;
			cout << "bar found" << endl;
			cout << "smallest side in mm" << smallest_side << endl;
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
	Rect rectangle = boundingRect(contour);
	int height = rectangle.height;
	int width = rectangle.width;

	//get the actual area of the shape and also a calculated area of a circle with the same diameter.
	double contour_area = contourArea(contour);
	double circle_area = (M_PI * pow(mm_per_pixel * 50, 2));

	Properties props = get_properties(contour);

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

