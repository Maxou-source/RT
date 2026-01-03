#ifndef __CYLINDER_HPP__
# define __CYLINDER_HPP__

# include "AObject.hpp"

class Cylinder : AObject
{
    public:
        Cylinder();
        bool intersect(Ray *r);
};

#endif