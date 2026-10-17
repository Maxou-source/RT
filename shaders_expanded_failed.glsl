#version 460 core
#define MAX_LIGHTS 100
#define MAX_OBJECTS 1000
#define OBJ_CYLINDER 2
#define OBJ_SPHERE 1
#line 1 0

out vec4 FragColor;

#line 1 1

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
#line 5 0
#line 1 2

layout(std140) uniform Scene {
    Camera cam;
    int    object_count;
    int    light_count;
    int    _pad0, _pad1;
    Light  lights[MAX_LIGHTS];
    Obj    objs[MAX_OBJECTS];
};
#line 6 0
#line 1 3
float intersect_sp(Ray r, Sphere sphere)
{
	Ray newR;
	newR.origin = r.origin * sphere.i_matrix;
	newR.direction = r.direction * sphere.i_matrix;

	vec4 direction = newR.direction;
	vec4 sphere_to_ray = newR.origin - vec4(0,0,0,1);

	float a = dot(direction, direction);
	float b = 2 * (dot(direction, sphere_to_ray));
	float c = dot(sphere_to_ray, sphere_to_ray) - 1;

	float discriminant = (b * b) - (4 * a * c);

	if (discriminant < 0) {
		return 0.0f;
	}
	float t1 = (-b - sqrt(discriminant)) / (2 * a);
	float t2 = (-b + sqrt(discriminant)) / (2 * a);
	if (t1 > 0)
		return t1;
	if (t2 > 0)
		return t2;
	return 0.0f;

}
#line 7 0

uniform PointLight light;

vec4 lighting(Material mat, PointLight lgt, vec4 point, vec4 eyev, vec4 normalv)
{
	vec4  eff_color         = mat.color * lgt.color;
	vec4  lightv            = normalize(lgt.position - point);
	vec4  new_ambient       = eff_color * lgt.ambient;
	float light_dot_normal  = dot(normalv, lightv);

	int px = int(gl_FragCoord.x);
	int py = int(gl_FragCoord.y);

	if (light_dot_normal < 0.0)
	{
		return new_ambient;
	}
	else
	{
		vec4 diffuset = eff_color * 0.9 * light_dot_normal;
		vec4 reflectv = reflect(-lightv, normalv);
		float reflect_dot_eye = dot(reflectv, eyev);

		if (reflect_dot_eye < 0.0)
		{
			// debugData[index] = vec4(mat.color.xyz, 3.0);
			vec4 new_specular = vec4(0, 0, 0, 0);
			return new_ambient + diffuset + new_specular;
		}
		
		// debugData[index] = vec4(mat.color.xyz, 4.0);
		float factor = pow(reflect_dot_eye, lgt.shininess);
		vec4 specularv = lgt.color * lgt.specular * factor;
		return new_ambient + diffuset + specularv;
	}
}

Ray	rayForPixel(int px, int py, int index) {
	float	world_x;
	float	world_y;
	vec4	world_point;
	vec4	pixel;
	Ray r;

	world_x = cam.half_width - gl_FragCoord.x * cam.pixel_size;
	world_y = -cam.half_height + gl_FragCoord.y * cam.pixel_size;
	world_point = vec4(world_x, world_y, -1.0, 1.0);
	pixel = world_point * cam.inv_transform;
	r.origin = (vec4(0,0,0,1.0) * cam.inv_transform);
	r.direction = (normalize(pixel - r.origin));
	if (px == 200 && py == 200)
	{
		debugData[index+5] = world_point;
		debugData[index+6] = r.origin;
		debugData[index+7] = r.direction;
	}
	return r;
}

void main()
{
	int px = int(gl_FragCoord.x);
	int py = int(gl_FragCoord.y);
	int index = py * 1000 + px; // one slot per pixel

	if (px == 200 && py == 200)
	{
		debugData[index] = vec4(cam.half_width, cam.half_height, 0.f, cam.pixel_size);
		debugData[index + 1] = cam.inv_transform[0];
		debugData[index + 2] = cam.inv_transform[1];
		debugData[index + 3] = cam.inv_transform[2];
		debugData[index + 4] = cam.inv_transform[3];
	}

	Sphere s;
	s.radius = 1.0;
	s.center = vec4(0, 0, 0, 1.0);
	s.i_matrix = mat4(1.0);

	Material mat;
	mat.color = vec4(1, 0, 0, 1);

	PointLight light;
	light.position = vec4(-10, 10, -10, 1.0);
	light.color = vec4(1, 1, 1, 1);
	light.ambient = 0.1;
	light.specular = 0.9;
	light.shininess = 200.0;

	Ray r = rayForPixel(px, py, index);

	float t = intersect_sp(r, s);
	if (t == 0.0)
	{
		FragColor = vec4(0.0, 0, 0.0, 1);
		return;
	}
	vec4 point = r.origin + t * r.direction;
	vec4 eyev = -r.direction;

	vec4 normalv = normalize(point - s.center);
	point = point + normalv* 0.00001 * 100.0;

	FragColor = lighting(mat, light,point, eyev, normalv);

	// debugData[index] = normalv;
	// debugData[index+1] = eyev;
	// debugData[index+2] = point;
	// debugData[index+3] = vec4(t);
	// debugData[index+4] = r.direction;
}
