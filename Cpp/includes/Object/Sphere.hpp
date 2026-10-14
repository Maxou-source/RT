#include "AObject.hpp"

class Sphere : public AObject
{
	private:
		float diameter = 1.f;
	public:
		Sphere();
		ObjectId type() const override {return ObjectId::SPHERE;}
		void printSpecific(std::ostream& os) const override;

		bool setProperty(const std::string& key, const std::string& val) override;
};