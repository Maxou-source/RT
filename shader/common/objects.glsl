
struct camera {
	mat4	inv_transform;
	float	half_width;
	float	half_height;
	float	pixel_size;
	float	_paddingA;
};

struct Ray {
	vec4	origin;
	vec4	direction;
};

struct Light {
    vec3  position;   float _pad0;
    vec3  intensity;  float _pad1;
};

struct Sphere {
	vec4	center;
	float	radius;

	mat4 i_matrix;
};

struct Material {
    vec4  color;                          // slot 0: rgb + w (alpha / HDR intensity)
    float ambient;    float diffuse;      // slot 1
    float specular;   float shininess;
    float reflective; int   pattern;      // slot 2
    int   texture;    int   _pad0;
};

struct Obj {
    mat4     inv_transform;
    Material mat;
    int      type;        // OBJ_SPHERE, OBJ_CUBE, ...
    float    param;       // the one shape-specific value (meaning depends on type)
    int      _pad0, _pad1;
};

struct PointLight {
	vec4  position;
	vec4  color;
	float ambient;
	float specular;
	float shininess;
};

struct Intersection {
	float	t;
	int		object_id;
};

layout(std430, binding = 0) buffer DebugBuffer {
    vec4 debugData[];
};
