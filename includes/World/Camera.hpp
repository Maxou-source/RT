#ifndef CAMERA_HPP
# define CAMERA_HPP

# include "Tuple.hpp"
# include "Matrix.hpp"
# include "IParsable.hpp"
# include "ParseUtils.hpp"

class Ray;

using namespace ParseUtils;

class Camera : public IParsable {
	private:
		t_f4  position = {0, 0, 0, 1};
		t_f4  orientation = {0, 0, 1, 0};
		float fov = 60.f;

		float	half_width;
		float	half_height;
		float	half_view;
		float	pixel_size;
		
		Matrix	transform;
		Matrix	inv_transform;
	public:
		
		bool setProperty(const std::string& key, const std::string& val) override;
		std::vector<std::string> requiredKeys() const override;
	    const char * typeString() const override;
	// Constructors and Destructors 
		Camera();
		Camera(unsigned int shaderProgram, const Tuple&, const Tuple&, float );
		Camera(const Tuple& pov, const Tuple& orient, float f);

	// Methods
		void	rayForPixel(int px, int py, Ray *r);

		void	setOpenGlObject();

		bool	buildMatrix() override;

};

#endif