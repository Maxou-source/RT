
#version 460 core
out vec4 FragColor;

struct camera {
	vec3 position;
	vec3 direction;

	float	half_width;
	float	half_height;
	float	half_view;
	float	pixel_size;

	mat4	transform;
	mat4	inv_transform;
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

Ray	rayForPixel(int px, int py) {
	float	world_x;
	float	world_y;
	vec4	world_point;
	vec4	pixel;
	Ray r;

	world_x = cam.half_width - ((float(px) + 0.5) * cam.pixel_size);
	world_y = cam.half_height - ((float(py) + 0.5) * cam.pixel_size);
	world_point = vec4(world_x, world_y, -1.0, 1.0);
	pixel = world_point * cam.inv_transform;
	// pixel = matrix_tuple(inv_transform, world_point);
	r.origin = (vec4(0,0,0,1.0) * cam.inv_transform);
	// r->origin.w = 1.0;
	// r->origin = matrix_tuple(inv_transform, point(0, 0, 0, 1.0));
	r.direction = (normalize(pixel - r.origin));
	// r->direction = normalization(pixel - r->origin);
	// r->direction.w = 0;
	return r;
}

float intersect_sp(Ray r, Sphere sphere)
{
	Ray newR;
	newR.origin = r.origin * sphere.i_matrix;
	newR.direction = r.direction * sphere.i_matrix;

	vec4 direction = newR.direction;
	vec4 sphere_to_ray = newR.origin - vec4(0,0,0,1);

	float a = dot(direction, direction);
	float b = 2 * (dot(sphere_to_ray, direction));
	float c = dot(sphere_to_ray, sphere_to_ray) - 1;

	float discriminant = (b * b) - (4 * a * c);

	if (discriminant < 0) {
		return 0.0f;
	}
	return ((-b - sqrt(discriminant)) / (2 * a));

}


uniform PointLight light;

vec4 lighting(Material mat, PointLight lgt, vec4 point, vec4 eyev, vec4 normalv)
{
	vec4  eff_color         = mat.color * lgt.color;
	vec4  lightv            = normalize(lgt.position - point);
	vec4  new_ambient       = eff_color * lgt.ambient;
	float light_dot_normal  = dot(normalv, lightv);

	int px = int(gl_FragCoord.x);
	int py = int(gl_FragCoord.y);
	int index = py * 1080 + px; 
		debugData[index] = vec4(mat.color.xyz, 1.0);

	if (light_dot_normal < 0.0)
	{
		debugData[index] = vec4(mat.color.xyz, 2.0);
		return new_ambient;
	}
	else
	{
		vec4 diffuset = eff_color * 0.9 * light_dot_normal;
		// GLSL's built-in reflect(I, N) expects I as the incident vector
		// (pointing INTO the surface), which is exactly -lightv here —
		// same convention as your CPU Tuple::reflect(negate(lightv), normalv)
		vec4 reflectv = reflect(-lightv, normalv);
		float reflect_dot_eye = dot(reflectv, eyev);

		if (reflect_dot_eye < 0.0)
		{
			debugData[index] = vec4(mat.color.xyz, 3.0);
			vec4 new_specular = vec4(0, 0, 0, 0);
			return new_ambient + diffuset + new_specular;
		}
		
		debugData[index] = vec4(mat.color.xyz, 4.0);
		float factor = pow(reflect_dot_eye, lgt.shininess);
		vec4 specularv = lgt.color * lgt.specular * factor;
		return new_ambient + diffuset + specularv;
	}
}

void main()
{

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
	// light.diffuse = 0.9;
	light.shininess = 200.0;


	int px = int(gl_FragCoord.x);
    int py = int(gl_FragCoord.y);
    int index = py * 1080 + px; // one slot per pixel

    Ray r = rayForPixel(px, py);

	// r.origin = vec4(float(px)/200 - 1, float(py)/200 -1, -4, 1.0);
	// r.direction = vec4(0, 0, -1, 0);
    // dump whatever you want to inspect
    // debugData[index] = vec4(r.direction.xyz, 0.0);

    float t = intersect_sp(r, s);
	if (t == 0.0)
	{
		FragColor = vec4(0.0, 0, 0.0, 1);
		return;
	}
	vec4 point = r.origin + t * r.direction;
	vec4 eyev = -r.direction;

	vec4 normalv = normalize(point - s.center);
	// if (dot(eyev, normalv) < 0)
	// {
	// 	inside = true;
	// 	normalv.negating();
	// }
	// else
	// 	inside = false;
	point = point + normalv* 0.00001 * 100.0;

	FragColor = lighting(mat, light,point, eyev, normalv);

	if (px == 200 && py == 200)
	{
		debugData[index] = normalv;
		debugData[index+1] = eyev;
		debugData[index+2] = point;
		debugData[index+3] = vec4(t);
		debugData[index+4] = r.direction;
	}
}