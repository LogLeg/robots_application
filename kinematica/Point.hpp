#ifndef POINT_HPP_
#define POINT_HPP_

/*
 * Copyright (c) 2016 Joost Kraaijeveld. See license.txt for details.
 * For information, bug reports and additions send an e-mail to DistributedRobotWorld@Askesis.nl.
 *
 * Author: jkr
 */

//#include <wx/gdicmn.h>
namespace Widgets
{
/**
 * @see http://docs.wxwidgets.org/stable/classwx_point
 */
//typedef wxPoint Point;
struct Point
{
	Point(int x, int y) :
			x(x), y(y)
	{
	}
	;
	int x;
	int y;
};
} // namespace Widgets
using namespace Widgets;

#endif // POINT_HPP_
