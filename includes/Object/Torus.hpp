#pragma once

# include "AObject.hpp"

class Torus: public AObject
{
	private:
		float A;
		float B;
		t_f4	orientation;
	public:
		Torus();
		ObjectId type() const override {return ObjectId::TORUS;}
		void printSpecific(std::ostream& os) const override;
		std::vector<std::string> requiredKeys() const override;
		const char * typeString() const override;
		bool packIntoGpuScene(GpuScene& s) const override;
		bool preBuild() override;
		bool setProperty(const std::string& key, const std::string& val) override;
};