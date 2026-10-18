
out vec4 FragColor;
const float INF     = 1e30;
//!include "common/objects.glsl"
//!include "common/scene.glsl"
//!include "intersections/sphere.glsl"

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

vec4 normal_at(Obj o, vec4 world_point)
{
	vec4 local_point  = world_point * o.inv_transform;        // same convention as your intersect_sp
	vec4 local_normal = vec4(local_point.xyz, 0.0);           // unit sphere at origin
	vec4 n            = o.inv_transform * local_normal;
	return vec4(normalize(n.xyz), 0.0);
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
		debugData[index+9] = world_point;
		debugData[index+10] = r.origin;
		debugData[index+11] = r.direction;
	}
	return r;
}

Intersection intersect_world(Ray r)
{
	Intersection best = Intersection(INF, -1);
	for (int i = 0; i < object_count; i++)
	{
		float t = INF;
		if (objs[i].type == OBJ_SPHERE) {
			t = intersect_sp(r, objs[i]);
		}
		
		if (t < best.t)
			best = Intersection(t, i);
	}
	return best;
}

void main()
{
	int px = int(gl_FragCoord.x);
	int py = int(gl_FragCoord.y);
	int index = py * 1000 + px; // one slot per pixel

	if (px == 200 && py == 200)
	{
		debugData[index] = vec4(cam.half_width, cam.half_height, 0.f, cam.pixel_size);
		debugData[index + 1] = objs[0].inv_transform[0];
		debugData[index + 2] = objs[0].inv_transform[1];
		debugData[index + 3] = objs[0].inv_transform[2];
		debugData[index + 4] = objs[0].inv_transform[3];

		debugData[index + 5] = objs[1].inv_transform[0];
		debugData[index + 6] = objs[1].inv_transform[1];
		debugData[index + 7] = objs[1].inv_transform[2];
		debugData[index + 8] = objs[1].inv_transform[3];
	}

	PointLight light;
	light.position = vec4(-10, 10, -10, 1.0);
	light.color = vec4(1, 1, 1, 1);
	light.ambient = 0.1;
	light.specular = 0.9;
	light.shininess = 200.0;

	Ray r = rayForPixel(px, py, index);

	Intersection interMilan = intersect_world(r);
	if (interMilan.object_id < 0)
	{
		FragColor = vec4(0.0, 0, 0.0, 1);
		return;
	}
	vec4 point = r.origin + interMilan.t * r.direction;
	vec4 eyev = -r.direction;

	vec4 normalv = normal_at(objs[interMilan.object_id],point);
	point = point + normalv* 0.00001 * 100.0;

	FragColor = lighting(objs[interMilan.object_id].mat, light,point, eyev, normalv);
}