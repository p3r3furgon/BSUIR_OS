#include "Point.h"
#include <cmath>

double Point::Func()
{
	return pow(x, 3)/ 42;
}
void Point::SetX(double x)
{
	this->x = x;
}
double Point::GetX()
{
	return x;
}
void Point::SetY(double y)
{
	this->y = y;
}
double Point::GetY()
{
	return y;
}