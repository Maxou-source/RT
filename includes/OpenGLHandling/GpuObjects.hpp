#pragma once
#include "Matrix.hpp"


struct GpuPattern {
	t_m4 i_m; // 64
	float color[4]; // 16
	int patType; // 4
	float param; // 4
	int	_pad1 = 0, _pad2 = 0;
};

struct GpuMaterial
{
	float	color[4]; // 16
	float	ambient; float diffuse; float specular; float shininess; // 16
	float	reflection; 
	float	transparency = 0;
	float	refractive_index = 0; // 16
	int _pad0 = 0;
	GpuPattern pat;
};

struct GpuObject
{
	t_m4			inv_transform; // 64
	GpuMaterial		mat; // 72 -> 12 + 72 -> 84
	int				type; // 4 -> 88
	float			param; // 4 -> 92
	int				_pad1 = 0, _pad2 = 0; // 100
};
/*
struct GpuMaterial
{
	float color[4]; // 16
	float ambient; float diffuse; float specular; float shininess; // 16
	
	float reflective; float transparency;
	int   _pad0 = 0, _pad_1 = 0;
	// 32 + 4 + 8

};


*/

struct GpuLight
{
	float position[4];    // slot 0
	float color[4];   // slot 1
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
