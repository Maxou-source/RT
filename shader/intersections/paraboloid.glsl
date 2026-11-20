
float intersect_pa(Ray r, Obj o)
{
	vec3 origin = (o.inv_transform * vec4(r.origin.xyz, 1.0)).xyz;
	vec3 direction = (o.inv_transform * vec4(r.direction.xyz, 0.0)).xyz;

	float a = o.param;

	float A = direction.x * direction.x
			+ direction.z * direction.z;

	float B = 2.0 * (origin.x * direction.x
				   + origin.z * direction.z)
				   - 4.0 * a * direction.y;

	float C = origin.x * origin.x
			+ origin.z * origin.z
			- 4.0 * a * origin.y;

	// Handle the degenerate linear case.
	if (abs(A) < 0.000001)
	{
		if (abs(B) < 0.000001)
			return INF;

		float t = -C / B;
		return t > 0.0 ? t : INF;
	}

	float discriminant = B * B - 4.0 * A * C;

	if (discriminant < 0.0)
		return INF;
	float t1 = (-B - sqrt(discriminant)) / (2.0 * A);
	float t2 = (-B + sqrt(discriminant)) / (2.0 * A);
	float best = INF;

	if (t1 > 0.0001)
	{
		vec3 p = origin + t1 * direction;

		if (p.y >= 0.0 && p.y <= o.param2)
			best = t1;
	}

	if (t2 > 0.0001)
	{
		vec3 p = origin + t2 * direction;

		if (p.y >= 0.0 && p.y <= o.param2)
			best = min(best, t2);
	}

	return best;
}