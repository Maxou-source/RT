#include "Point.hpp"

Point::Point() {
	value = 0;
}

void Point::display()
{
	std::cout << "tuple : " << "x: " << value.x
	<< " y: " << value.y << " z: " << value.z << " w: " << value.w << std::endl;
}