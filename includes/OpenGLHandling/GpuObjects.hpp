#pragma once
#include "Matrix.hpp"


struct GpuPattern {
	t_m4 i_m;
	float color[4];
	int patType;
	float param;
	int	_pad1 = 0, _pad2 = 0;
};

struct GpuTexture
{
	t_m4  i_m;
	int   layer;
	float scale;
	int   _pad0 = 0, _pad1 = 0;
};

struct GpuWobble
{
	float amplitude;
	float frequency;
	float _pad0 = 0, _pad1 = 0;
};

struct GpuMaterial
{
	float	color[4]; // 16
	float	ambient; float diffuse; float specular; float shininess; // 16
	float	reflection; 
	float	transparency = 0;
	float	refractive_index = 0; // 16
	int	_pad0 = 0;
	GpuPattern	pat;
	GpuTexture	texture;
	GpuWobble	wob;
};

struct GpuObject
{
	t_m4			inv_transform; // 64
	GpuMaterial		mat; // 72 -> 12 + 72 -> 84
	int				type; // 4 -> 88
	float			param; // 4 -> 92
	int				param2;
	int				_pad2 = 0; // 100
};

struct GpuLight
{
	float source[4];
	float color[4];
	float ambient;
	float diffuse;
	float specular;
	int type;
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
