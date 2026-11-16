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
		bool init = false;
		t_f4  position = {0, 0, 0, 1};
		t_f4  orientation = {0, 0, 1, 0};
		float fov = 60.f;

		float	half_width;
		float	half_height;
		float	half_view;
		float	pixel_size;
		
		Matrix	transform;
		Matrix	inv_transform;

		float yaw;
		float pitch;

		// actual height needed to update camera for resize
		int width = 0.0;
		int height = 0.0;
	public:
		
		bool setProperty(const std::string& key, const std::string& val) override;
		std::vector<std::string> requiredKeys() const override;
		const char * typeString() const override;
		bool preBuild() override;
		bool packIntoGpuScene(GpuScene& s) const override;
		bool repeatableSubBlock() const override {return false;}
		// Constructors and Destructors 
		Camera();
		void print() const;

		// update
		void updateSize(int, int);
		void move(float forward, float right, float up);
		void rotate(float dYaw, float dPitch);

	// Methods
		void	rayForPixel(int px, int py, Ray *r);

		void	setOpenGlObject();

	// getters and setters
		t_f4 getPosition(void) const;
		void setViewPort(int, int);
};

#endif