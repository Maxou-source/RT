#ifndef POINT_HPP
# define POINT_HPP


# include "ATuple.hpp"

class Point: public ATuple
{
	private:
		int w;
	public:
	
	// constructor and destuctors 
		Point();
		~Point() {}
	
	// methods
		void display();
};

#endif