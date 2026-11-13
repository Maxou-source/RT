#pragma once

# include "AObject.hpp"

class Cube : public AObject
{
	private:
		float	size;
		t_f4	orientation;
	public:
		Cube();
		ObjectId type() const override {return ObjectId::CUBE;}
        void printSpecific(std::ostream& os) const override;
		std::vector<std::string> requiredKeys() const override;
	    const char * typeString() const override;
	    bool packIntoGpuScene(GpuScene& s) const override;
        bool preBuild() override;
		bool repeatableSubBlock() const override;

        bool setProperty(const std::string& key, const std::string& val) override;
};
