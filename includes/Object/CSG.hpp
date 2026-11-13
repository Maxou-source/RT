#pragma once

#include "IParsable.hpp"
#include "AObject.hpp"
# include <memory>

class CSG: public AObject
{
	private:
		t_f4		orientation;
		ObjectId	ty;
		std::vector<std::unique_ptr<AObject>>  children; 
	public:
		CSG(ObjectId t): ty(t) {}
		CSG() {}
		ObjectId type() const override {return ty;}
		AObject* AddObject(ObjectId id);
		IParsable* getSubBlock(const std::string& name) override;
		std::vector<std::string> requiredKeys() const override;
		const char * typeString() const override;
		bool packIntoGpuScene(GpuScene& s) const override;
		bool repeatableSubBlock() const override;
		bool preBuild() override;
		bool setProperty(const std::string& key, const std::string& val) override;
};
