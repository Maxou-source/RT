
struct camera {
	mat4	inv_transform;
	float	half_width;
	float	half_height;
	float	pixel_size;
};

struct Ray {
	vec4	origin;
	vec4	direction;
};

struct Sphere {
	vec4	center;
	float	radius;

	mat4 i_matrix;
};

struct Material {
	vec4 color;
};

struct PointLight {
	vec4  position;
	vec4  color;
	float ambient;
	float specular;
	float shininess;
};

layout(std430, binding = 0) buffer DebugBuffer {
    vec4 debugData[];
};

uniform camera cam;