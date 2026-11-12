
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
//!include "lights/texture.glsl"



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

float intersect_obj(Ray r, Obj o)
{
	if (o.type == OBJ_SPHERE) {
		return(intersect_sp(r, o));
	} else if (o.type == OBJ_CYLINDER) {
		return(intersect_cy(r, o));
	} else if (o.type == OBJ_CUBE) {
		return(intersect_cu(r, o));
	} else if (o.type == OBJ_PLANE) {
		return(intersect_pl(r, o));
	} else if (o.type == OBJ_CONE) {
		return(intersect_co(r, o));
	}
	return INF;
}

Intersection intersect_world(Ray r)
{
	Intersection best = Intersection(INF, -1);
	for (int i = 0; i < object_count; i++)
	{
		float t;
		t = intersect_obj(r, objs[i]);
		
		if (t < best.t)
			best = Intersection(t, i);
	}
	return best;
}

Computation computations(Ray r, Intersection i, Obj o);
float shadow_factor(vec4 point, Light lgt)
{
	vec4  v        = lgt.position - point;
	float distance = length(v);

	Ray sr;
	sr.origin    = point;
	sr.direction = vec4(normalize(v));

	float factor = 1.0;

	for (int i = 0; i < object_count; i++)
	{
		float t = intersect_obj(sr, objs[i]);
		if (t >= distance || t <= 0.0001)
			continue;

		if (objs[i].mat.transparency == 0.0)
			return 0.0;
		Intersection it = Intersection(t, i);
		Computation comps = computations(sr, it, objs[i]);

		float cos_theta = clamp(dot(normalize(sr.direction.xyz), normalize(comps.normalv.xyz)), -1.0, 1.0);
		float sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        float eta = 1.0 / objs[i].mat.refractive_index; // Air (1.0) to Object IOR
        float sin_theta_t = eta * sin_theta;

        // If total internal reflection happens inside our approximation loop
        if (sin_theta_t > 1.0) {
            factor *= 0.0; // Block light
            continue;
        }

		float cos_theta_t = sqrt(1.0 - sin_theta_t * sin_theta_t);
		// schlick
        float r0 = ((1.0 - objs[i].mat.refractive_index) / (1.0 + objs[i].mat.refractive_index));
        r0 = r0 * r0;
        float fresnel = r0 + (1.0 - r0) * pow(1.0 - abs(cos_theta), 5.0);

		float dispersion = pow(sin_theta, 2.0) * (1.0 - eta);
        
        // Combine transmission loss (fresnel) and directional scattering (dispersion)
        float transmission = (1.0 - fresnel) * (1.0 - dispersion);

        factor *= clamp(transmission, 0.0, 1.0) * objs[i].mat.transparency;
	}
	return factor;
}

bool refract_dir(Computation c, out vec4 dir)
{
	float ratio  = c.n1 / c.n2;
	float cos_i  = dot(c.eyev.xyz, c.normalv.xyz);
	float sin2_t = ratio * ratio * (1.0 - cos_i * cos_i);

	if (sin2_t > 1.0)
		return false;

	float cos_t = sqrt(1.0 - sin2_t);
	dir = vec4(c.normalv.xyz * (ratio * cos_i - cos_t) - c.eyev.xyz * ratio, 0.0);
	return true;
}

vec4 wobble_at(Obj o, vec4 point, vec4 normalv)
{
	vec4 lp = point * o.inv_transform;            // exact point, object space
	vec3 d = vec3(sin(lp.y * o.mat.wob.frequency), sin(lp.z * o.mat.wob.frequency), sin(lp.x * o.mat.wob.frequency));
	d -= normalv.xyz * dot(d, normalv.xyz);       // tangent part only
	return (vec4(normalize(normalv.xyz + o.mat.wob.amplitude * d), 0.0));
}

Computation computations(Ray r, Intersection i, Obj o)
{
	vec4 point = r.origin + i.t * r.direction;
	vec4 eyev  = -r.direction;

	vec4 normalv;
	if      (o.type == OBJ_CYLINDER) normalv = normal_at_cy(o, point);
	else if (o.type == OBJ_CUBE)     normalv = normal_at_cu(o, point);
	else if (o.type == OBJ_PLANE)    normalv = normal_at_pl(o, point);
	else if (o.type == OBJ_CONE)     normalv = normal_at_co(o, point);
	else                             normalv = normal_at(o, point);

	bool inside = dot(normalv, eyev) < 0.0;
	if (inside)
		normalv = -normalv;                           // geometric normal, facing the eye

	vec4 over_point  = point + normalv * EPSILON * 100.0;   // after the flip
	vec4 under_point = point - normalv * EPSILON * 100.0;

	if (o.mat.wob.amplitude != 0.0)
		normalv = wobble_at(o, point, normalv);

	float n1 = 1.0, n2 = 1.0;
	if (o.mat.transparency > 0.0)
	{
		n1 = inside ? o.mat.refractive_index : 1.0;
		n2 = inside ? 1.0 : o.mat.refractive_index;
	}
	return Computation(o, o.mat, over_point, eyev, normalv, under_point, n1, n2);
}

vec4 color_at(Ray r);

vec4 refelec_color(Computation comps)
{
	if (comps.obj.mat.reflection == 0)
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

	if (cmp.mat.text.layer >= 0)
		cmp.mat.color = applyTexture(cmp.obj, cmp.under_point);

	// ambient 
	vec4 amb = (ambientLgt.a * ambientLgt.a.w) * cmp.mat.ambient;
	res = cmp.mat.color * amb;

	// main lighting
	for (int i = 0; i < light_count; i++)
	{
		float shadowed = 1.0;
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
	vec3 result = vec3(0.0);
	float weight = 1.0;

	for (int b = 0; b < 10; b++)
	{
		Intersection it = intersect_world(r);
		if (it.t == INF)
			break;

		Computation c = computations(r, it, objs[it.object_id]);
		vec3 local;
		local = shade_hit(c).rgb;

		vec4 dir;
		if (c.mat.transparency > 0.0 && refract_dir(c, dir))
		{
			result += weight * (1.0 - c.mat.transparency) * local;
			weight *= c.mat.transparency;
			r.origin    = c.under_point;
			r.direction = dir;
		}
		else
		{
			result += weight * local;
			weight *= c.mat.reflection;
			r.origin    = c.point;                     // over point
			r.direction = vec4(reflect(-c.eyev.xyz, c.normalv.xyz), 0.0);
		}
		if (weight < 0.01)
			break;
	}
	return vec4(result, 1.0);
}

void main()
{
	int px = int(gl_FragCoord.x);
	int py = int(gl_FragCoord.y);
	int index = py * 1000 + px;
	vec4 fakeSample = texture(textures, vec3(0.0)) * 0.0;
	Ray r = rayForPixel(px, py, index);

	FragColor = color_at(r) + fakeSample;
	if (px == 200 && py == 200)
	{
		debugData[index] = vec4(objs[0].mat.text.layer, objs[1].mat.text.layer, 0.0, 0.0);
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