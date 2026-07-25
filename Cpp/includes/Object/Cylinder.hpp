#ifndef __CYLINDER_HPP__
# define __CYLINDER_HPP__

# include "AObject.hpp"

class Cylinder : public AObject
{
    private:
        vec4    orient;
        float   width;
        float   height;

    public:
        Cylinder();
        bool intersect(Ray *r);
};

#endif