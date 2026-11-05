
out vec4 FragColor;
//!include "utils.glsl"
//!include "common/objects.glsl"
//!include "common/scene.glsl"
//!include "intersections/sphere.glsl"
//!include "intersections/cylinder.glsl"
//!include "intersections/cube.glsl"
//!include "intersections/plane.glsl"
//!include "intersections/cone.glsl"
//!include "lights/lighting.glsl"
//!include "lights/normals.glsl"
//!include "lights/pattern.glsl"

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
		} else if (objs[i].type == OBJ_CYLINDER) {
			t = intersect_cy(r, objs[i]);
		} else if (objs[i].type == OBJ_CUBE) {
			t = intersect_cu(r, objs[i]);
		} else if (objs[i].type == OBJ_PLANE) {
			t = intersect_pl(r, objs[i]);
		} else if (objs[i].type == OBJ_CONE) {
			t = intersect_co(r, objs[i]);
		}
		
		if (t < best.t)
			best = Intersection(t, i);
	}
	return best;
}

bool is_shadowed(vec4 point, Light lgt)
{
	vec4  v        = lgt.position - point;
	float distance = length(v.xyz);

	Ray shadowRay;
	shadowRay.origin    = point;
	shadowRay.direction = vec4(normalize(v.xyz), 0.0);   // w = 0: it's a direction

	Intersection h = intersect_world(shadowRay);
	return (h.object_id >= 0 && h.t < distance);
}

void main()
{
	int px = int(gl_FragCoord.x);
	int py = int(gl_FragCoord.y);
	int index = py * 1000 + px; // one slot per pixel

	if (px == 200 && py == 200)
	{
		debugData[index] = vec4(objs[1].mat.pat.param, cam.half_height, 0.f, cam.pixel_size);
		debugData[index + 1] = objs[0].inv_transform[0];
		debugData[index + 2] = objs[0].inv_transform[1];
		debugData[index + 3] = objs[0].inv_transform[2];
		debugData[index + 4] = objs[0].inv_transform[3];

		debugData[index + 5] = objs[1].inv_transform[0];
		debugData[index + 6] = objs[1].inv_transform[1];
		debugData[index + 7] = objs[1].inv_transform[2];
		debugData[index + 8] = objs[1].inv_transform[3];
	}
	Ray r = rayForPixel(px, py, index);

	Intersection interMilan = intersect_world(r);
	if (interMilan.object_id < 0)
	{
		FragColor = vec4(0.0, 0, 0.0, 1);
		return;
	}

	vec4 point = r.origin + interMilan.t * r.direction;
	vec4 eyev = -r.direction;

	vec4 normalv;
	if (objs[interMilan.object_id].type == OBJ_CYLINDER)
		normalv = normal_at_cy(objs[interMilan.object_id], point);
	else if (objs[interMilan.object_id].type == OBJ_CUBE)
		normalv = normal_at_cu(objs[interMilan.object_id], point);
	else if (objs[interMilan.object_id].type == OBJ_PLANE)
		normalv = normal_at_pl(objs[interMilan.object_id], point);
	else if (objs[interMilan.object_id].type == OBJ_CONE)
		normalv = normal_at_co(objs[interMilan.object_id], point);
	else
		normalv = normal_at(objs[interMilan.object_id],point);

	Material m = objs[interMilan.object_id].mat;
	if (dot(normalv, eyev) < 0.0) {
		normalv = -normalv;
	}
	if (m.pat.patType != NO_PATTERN)
		m.color = applyPattern(objs[interMilan.object_id], point);
	point = point + normalv * EPSILON * 100.0;
	vec4 amb = (ambientLgt.a * ambientLgt.a.w) * m.ambient;   // m.ambient defaults to 1.0
	FragColor = m.color * amb;
	for (int i = 0; i < light_count; i++)
	{
		bool shadowed = is_shadowed(point, lights[i]);
		FragColor += lighting(m, lights[i] ,point, eyev, normalv, shadowed);
	}
}