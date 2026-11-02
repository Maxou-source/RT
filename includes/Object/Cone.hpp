#pragma once

# include "AObject.hpp"

class Cone : public AObject
{
	private:
		float		size;
		float		diameter;
		t_f4	orientation;
	public:
		Cone();
		ObjectId type() const override {return ObjectId::CONE;}
        void printSpecific(std::ostream& os) const override;
		std::vector<std::string> requiredKeys() const override;
	    const char * typeString() const override;
	    bool packIntoGpuScene(GpuScene& s) const override;
        bool preBuild() override;

        bool setProperty(const std::string& key, const std::string& val) override;
};
