#ifndef SIZE_HPP_
#define SIZE_HPP_

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
 * @see http://docs.wxwidgets.org/stable/classwx_size
 */
//typedef wxSize Size;
struct Size
{
	Size(int x, int y) :
			x(x), y(y)
	{
	}
	;
	int x;
	int y;
};
} // namespace Widgets
using namespace Widgets;

#endif // SIZE_HPP_
