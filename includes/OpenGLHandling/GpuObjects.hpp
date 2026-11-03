#pragma once
#include "Matrix.hpp"

struct GpuMaterial
{
	float color[4];  float ambient;
	float diffuse;   float specular;
	float shininess; float reflective;
	int   pattern;   int   texture;
	int   _pad0 = 0	;
};

struct GpuObject
{
	t_m4			inv_transform;
	GpuMaterial		mat;
	int				type;
	float			param;
	int				_pad1 = 0, _pad2 = 0;
};

struct GpuLight
{
	float position[3];  float _pad0 = 0;    // slot 0
	float color[3];     float _pad1 = 0;    // slot 1
	float ambient;      float diffuse;      // slot 2
	float specular;     float _pad2 = 0;
};

struct GpuAmbientLight {
	float colorAndIntensity[4] = {1.0,1.0,1.0, 0.2};
};


static_assert(sizeof(GpuLight) == 48, "GpuLight must be 48 bytes");

struct GpuCamera {
	t_m4	inv_transform;
	float	half_width, half_height, pixel_size, _pad0 = 0;
};

struct GpuScene
{
	GpuCamera		cam;
	int				object_count, light_count, _pad0 = 0, _pad1 = 0;
	GpuAmbientLight	ambient;
	GpuLight  lights[MAX_LIGHTS];
	GpuObject objs[MAX_OBJECTS];
};
