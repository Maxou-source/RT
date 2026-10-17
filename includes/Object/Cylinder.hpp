#ifndef __CYLINDER_HPP__
# define __CYLINDER_HPP__

# include "AObject.hpp"

class Cylinder : public AObject
{
    private:
        float height = 1.0f;
		float		diameter;
    public:
        Cylinder();
        ObjectId type() const override {return ObjectId::CYLINDER;}
        void printSpecific(std::ostream& os) const override;
		std::vector<std::string> requiredKeys() const override;
	    const char * typeString() const override;
	    bool packIntoGpuScene(GpuScene& s) override;
        bool preBuild() override;

        bool setProperty(const std::string& key, const std::string& val) override;
};

#endif