#pragma once
#include "Matrix.hpp"

struct GpuMaterial
{
	float color[3];  float ambient;
	float diffuse;   float specular;
	float shininess; float reflective;
	int   pattern;   int   texture;
	int   _pad0 = 0, _pad1 = 0;
};

struct GpuObject
{
	t_m4		inv_transform;
	GpuMaterial	mat;
	int			type;
	int			_pad0 = 0, _pad1 = 0, _pad2 = 0;
};

struct GpuLight  { float position[3]; float _pad0 = 0; float intensity[3]; float _pad1 = 0; };

struct GpuCamera {
	t_m4	inv_transform;
	float	half_width, half_height, pixel_size, _pad0 = 0;
};

struct GpuScene
{
	GpuCamera cam;
	int       object_count, light_count, _pad0 = 0, _pad1 = 0;
	GpuLight  lights[MAX_LIGHTS];
	GpuObject objs[MAX_OBJECTS];
};
