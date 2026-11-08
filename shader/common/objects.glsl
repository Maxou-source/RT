
const float INF     = 1e30;

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
    vec4  position;
    vec4  color;
    float ambient;   float diffuse;    // slot 2 (16 bytes)
    float specular;  float _pad2;
};

struct AmbientLight {
	vec4 a;
};

struct Sphere {
	vec4	center;
	float	radius;

	mat4 i_matrix;
};

struct Pattern
{
	mat4 i_m;
	vec4 color;
	int patType;
	float param;
};

struct Material {
    vec4  color; // 16
    float ambient; float diffuse; float specular; float shininess; // 16
    float reflection;
	float transparency;
	float refractive_index;
    int _pad1; // 4 * 4 -> 16 again
	Pattern pat; // 24
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

struct Computation {
	Obj		obj;
	Material mat;
	vec4	point;
	vec4	eyev;
	vec4	normalv;
	vec4	under_point;
	float	n1;
	float	n2;
};

layout(std430, binding = 0) buffer DebugBuffer {
    vec4 debugData[];
};
