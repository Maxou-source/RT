
#ifndef OBJECT_HPP
# define OBJECT_HPP

# include <iostream>
# include <stdbool.h>
# include <set>
# include "Intersection.hpp"
# include "Tuple.hpp"
# include "Matrix.hpp"
# include "rt.hpp"
# include "ParseUtils.hpp"
# include "IParsable.hpp"

class Ray;

using namespace ParseUtils;

struct Material
{
	t_clr       color     = {1.f, 1.f, 1.f};
	float       ambient   = 0.1f;
	float       diffuse   = 0.9f;
	float       specular  = 0.9f;
	float       shininess = 200.f;
	float       reflective = 0.f;
	bool        pattern   = false;
	std::string texture;               // empty = none

	bool setProperty(const std::string& key, const std::string& val)
	{
		try {
			if (key == "color")           return parseColor(val, color);
			if (key == "ambient")         ambient = std::stof(val);
			else if (key == "diffuse")    diffuse = std::stof(val);
			else if (key == "specular")   specular = std::stof(val);
			else if (key == "shininess")  shininess = std::stof(val);
			else if (key == "reflective") reflective = std::stof(val);
			else if (key == "pattern")    pattern = (val == "true");
			else if (key == "texture")    texture = val;
			else return false;           // not a material key
		} catch (const std::exception&) { return false; }
		return true;
	}
};

class AObject : public IParsable
{
	protected:
		t_f4		position;
		float		specular;
		Material	material;
		bool		pattern;

		Matrix		m;
		Matrix		i_m;
		Matrix		t_m;

	public:
	// Constructors and Destructors
		virtual ~AObject() {}
		AObject();
		virtual ObjectId type() const = 0;
		virtual const char * typeString() const override;
		virtual void printSpecific(std::ostream&) const {}
		std::vector<std::string> requiredKeys() const override;
		void	applyTransformations();
		bool packIntoGpuScene(GpuScene& s) override;

	// setters and getters
		bool	setProperty(const std::string& key, const std::string& val) override;

		// bool	buildMatrix() override {return true;}
		
		void	setPosition(t_f4);
		void	setSpecular(const float s);
		void	setMaterial(const Material&);
		void	setPattern(const bool b);
		
		t_f4			getPosition() const;
		float			getSpecular() const;
		const Material&	getMaterial() const;
		bool			getPattern() const;
};

#endif