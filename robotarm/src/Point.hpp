#ifndef POINT_HPP_
#define POINT_HPP_
namespace robotPoint
{
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
}
#endif // POINT_HPP_
