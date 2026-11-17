#pragma once

#include "AObject.hpp"

class Paraboloid: public AObject
{
	private:
		t_f4 orientation;
		float size;
		float height;
	public:
		Paraboloid();
		ObjectId type() const override {return ObjectId::PARABOLOID;}
		void printSpecific(std::ostream& os) const override;
		std::vector<std::string> requiredKeys() const override;
		const char * typeString() const override;
		bool packIntoGpuScene(GpuScene& s) const override;
		bool preBuild() override;
		bool repeatableSubBlock() const override;

		bool setProperty(const std::string& key, const std::string& val) override;
};