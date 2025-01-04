
#ifndef OBJECT_HPP
# define OBJECT_HPP

#include <iostream>

class AObject
{
	private:
		int id;
	public:
		virtual ~AObject() {}
		AObject();
};

#endif