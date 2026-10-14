
out vec4 FragColor;

//!include "common/objects.glsl"
//!include "intersections/sphere.glsl"

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