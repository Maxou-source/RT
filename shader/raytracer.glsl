
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


// vec4 shade_hit()
// {

// }

// Intersection computations(Ray r) {
// 	Intersection interMilan = intersect_world(r);
// 	if (interMilan.object_id < 0)
// 	{
// 		// FragColor = vec4(0.0, 0, 0.0, 1);
// 		return interMilan;
// 	}

// 	interMilan.point = r.origin + interMilan.t * r.direction;
// 	interMilan.eyev = -r.direction;

// 	vec4 normalv;
// 	if (objs[interMilan.object_id].type == OBJ_CYLINDER)
// 		normalv = normal_at_cy(objs[interMilan.object_id], interMilan.point);
// 	else if (objs[interMilan.object_id].type == OBJ_CUBE)
// 		normalv = normal_at_cu(objs[interMilan.object_id], interMilan.point);
// 	else if (objs[interMilan.object_id].type == OBJ_PLANE)
// 		normalv = normal_at_pl(objs[interMilan.object_id], interMilan.point);
// 	else if (objs[interMilan.object_id].type == OBJ_CONE)
// 		normalv = normal_at_co(objs[interMilan.object_id], interMilan.point);
// 	else
// 		normalv = normal_at(objs[interMilan.object_id],interMilan.point);

// 	if (dot(normalv, interMilan.eyev) < 0.0) {
// 		normalv = -normalv;
// 	}
// 	interMilan.point = interMilan.point + normalv * EPSILON * 100.0;
// 	interMilan.normalv = normalv;
// 	return interMilan;
// }


bool is_shadowed(vec4 point, Light lgt)
{
	vec4  v        = lgt.position - point;
	float distance = length(v.xyz);

	Ray shadowRay;
	shadowRay.origin    = point;
	shadowRay.direction = vec4(normalize(v.xyz), 0.0);

	Intersection h = intersect_world(shadowRay);
	return (h.object_id >= 0 && h.t < distance);
}

Computation computations(Ray r, Intersection i, Obj o)
{

	// assign the raw point value
	vec4 point = r.origin + i.t * r.direction;
	// assign the eye vector
	vec4 eyev = -r.direction;

	vec4 normalv;
	// assign the normal
	if (objs[i.object_id].type == OBJ_CYLINDER)
		normalv = normal_at_cy(objs[i.object_id], point);
	else if (objs[i.object_id].type == OBJ_CUBE)
		normalv = normal_at_cu(objs[i.object_id], point);
	else if (objs[i.object_id].type == OBJ_PLANE)
		normalv = normal_at_pl(objs[i.object_id], point);
	else if (objs[i.object_id].type == OBJ_CONE)
		normalv = normal_at_co(objs[i.object_id], point);
	else
		normalv = normal_at(objs[i.object_id],point);

	if (dot(normalv, eyev) < 0.0) {
		normalv = -normalv;
	}

	// this is the over_point replaces the current point to avoid inconsistency
	point = point + normalv * EPSILON * 100;
	return Computation(o, o.mat, point, eyev, normalv);
}

vec4 color_at(Ray r);

vec4 refelec_color(Computation comps)
{
	if (comps.obj.mat.reflective == 0)
		return vec4(0);
	Ray refRay;

	refRay.origin = comps.point;
	refRay.direction = vec4(reflect(-comps.eyev.xyz, comps.normalv.xyz), 0.0);

	return color_at(refRay);
}

vec4 shade_hit(Computation cmp)
{
	vec4 res;
	// PATTERN
	if (cmp.mat.pat.patType != NO_PATTERN)
		cmp.mat.color = applyPattern(cmp.obj, cmp.point);

	// ambient 
	vec4 amb = (ambientLgt.a * ambientLgt.a.w) * cmp.mat.ambient;
	res = cmp.mat.color * amb;

	// main lighting
	for (int i = 0; i < light_count; i++)
	{
		bool shadowed = is_shadowed(cmp.point, lights[i]);
		res += lighting(cmp.mat, lights[i] ,cmp.point, cmp.eyev, cmp.normalv, shadowed);
	}

	// reflect
	// res += refelec_color(cmp);
	return vec4(res.r, res.g, res.b, 1.0);
}

Ray reflected_ray(Computation c)
{
	Ray r;
	r.origin    = c.point;                                  // already an over point
	r.direction = vec4(reflect(-c.eyev.xyz, c.normalv.xyz), 0.0);
	return r;
}

vec4 color_at(Ray r)
{
	vec3  result = vec3(0.0);
	float weight = 1.0;

	for (int b = 0; b < 1000; b++)
	{
		Intersection it = intersect_world(r);
		if (it.t == INF)
			break;

		Computation c = computations(r, it, objs[it.object_id]);
		result += weight * shade_hit(c).rgb;

		weight *= c.mat.reflective;
		if (weight < 0.01)
			break;

		r = reflected_ray(c);
	}
	return vec4(result, 1.0);
}

void main()
{
	int px = int(gl_FragCoord.x);
	int py = int(gl_FragCoord.y);
	int index = py * 1000 + px;

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

	FragColor = color_at(r);
	if (px == 200 && py == 200)
	{
		debugData[index] = vec4(FragColor);
		debugData[index + 1] = objs[0].inv_transform[0];
		debugData[index + 2] = objs[0].inv_transform[1];
		debugData[index + 3] = objs[0].inv_transform[2];
		debugData[index + 4] = objs[0].inv_transform[3];

		debugData[index + 5] = objs[1].inv_transform[0];
		debugData[index + 6] = objs[1].inv_transform[1];
		debugData[index + 7] = objs[1].inv_transform[2];
		debugData[index + 8] = objs[1].inv_transform[3];
	}
}