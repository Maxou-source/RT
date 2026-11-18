
//!include "computations.glsl"

float shadow_factor(vec4 point, Light lgt)
{
	vec4  L;
	float max_t;

	if (lgt.type == DIRECTIONAL)
	{
		L	 = normalize(-lgt.position);
		max_t = INF;
	}
	else
	{
		vec4 v = lgt.position - point;
		L	  = normalize(v);
		max_t  = length(v);
	}


	Ray sr;
	sr.origin	= point;
	sr.direction = vec4(normalize(L));

	float factor = 1.0;

	for (int i = 0; i < object_count; i++)
	{
		Intersection it = intersect_top(sr, i);
		if (it.t >= max_t || it.t <= 0.0001 || it.object_id < 0)
			continue;

		if (objs[i].mat.transparency == 0.0)
			return 0.0;
		Computation comps = computations(sr, it, objs[i]);

		float cos_theta = clamp(dot(normalize(sr.direction.xyz), normalize(comps.normalv.xyz)), -1.0, 1.0);
		float sin_theta = sqrt(1.0 - cos_theta * cos_theta);

		float eta = 1.0 / objs[i].mat.refractive_index;
		float sin_theta_t = eta * sin_theta;

		if (sin_theta_t > 1.0) {
			factor *= 0.0;
			continue;
		}

		float cos_theta_t = sqrt(1.0 - sin_theta_t * sin_theta_t);

		float r0 = ((1.0 - objs[i].mat.refractive_index) / (1.0 + objs[i].mat.refractive_index));
		r0 = r0 * r0;
		float fresnel = r0 + (1.0 - r0) * pow(1.0 - abs(cos_theta), 5.0);

		float dispersion = pow(sin_theta, 2.0) * (1.0 - eta);
		
		float transmission = (1.0 - fresnel) * (1.0 - dispersion);

		factor *= clamp(transmission, 0.0, 1.0) * objs[i].mat.transparency;
	}
	return factor;
}

vec4 shade_hit(Computation cmp)
{
	vec4 res;

	if (cmp.mat.pat.patType != NO_PATTERN)
		cmp.mat.color = applyPattern(cmp.obj, cmp.point);

	if (cmp.mat.text.layer >= 0)
		cmp.mat.color = applyTexture(cmp.obj, cmp.under_point);

	vec4 amb = (ambientLgt.a * ambientLgt.a.w) * cmp.mat.ambient;
	res = cmp.mat.color * amb;

	for (int i = 0; i < light_count; i++)
	{
		float shadowed = shadow_factor(cmp.point, lights[i]);
		res += lighting(cmp.mat, lights[i] ,cmp.point, cmp.eyev, cmp.normalv, shadowed);
	}
	return vec4(res.r, res.g, res.b, 1.0);
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

vec4 color_at(Ray r)
{
	vec3 result = vec3(0.0);
	float weight = 1.0;

	for (int b = 0; b < 100; b++)
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
			r.origin    = c.point;
			r.direction = vec4(reflect(-c.eyev.xyz, c.normalv.xyz), 0.0);
		}
		if (weight < 0.01)
			break;
	}
	return vec4(result, 1.0);
}
