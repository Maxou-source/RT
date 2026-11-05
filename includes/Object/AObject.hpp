
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
# include "Material.hpp"

class Ray;

using namespace ParseUtils;

class AObject : public IParsable
{
	protected:
		t_f4		position;
		Material	material;

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
		bool packIntoGpuScene(GpuScene& s) const override;
		IParsable* getSubBlock(const std::string& name) override;

	// setters and getters
		bool	setProperty(const std::string& key, const std::string& val) override;

		// bool	buildMatrix() override {return true;}
		
		void	setPosition(t_f4);
		void	setMaterial(const Material&);
		
		t_f4			getPosition() const;
		const Material&	getMaterial() const;
};

#endif