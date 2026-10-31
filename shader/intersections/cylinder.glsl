
bool cap_ok(Ray l, float t)
{
	float x = l.origin.x + t * l.direction.x;
	float z = l.origin.z + t * l.direction.z;
	return (x * x + z * z) <= 1.0;
}

float intersect_cy(Ray r, Obj o)
{
	Ray newR;
	newR.origin    = r.origin    * o.inv_transform;
	newR.direction = r.direction * o.inv_transform;

	float	half_h = o.param / 2.0;
	float	best   = INF;

	float a = newR.direction.x * newR.direction.x + newR.direction.z * newR.direction.z;
	if (a > EPSILON)
	{
		float b    = 2.0 * (newR.origin.x * newR.direction.x + newR.origin.z * newR.direction.z);
		float c    = newR.origin.x * newR.origin.x + newR.origin.z * newR.origin.z - 1.0;
		float disc = b * b - 4.0 * a * c;
		if (disc >= 0.0)
		{
			float sq = sqrt(disc);
			float t0 = (-b - sq) / (2.0 * a);
			float t1 = (-b + sq) / (2.0 * a);
			if (t0 > EPSILON && abs(newR.origin.y + t0 * newR.direction.y) < half_h)
				best = min(best, t0);
			if (t1 > EPSILON && abs(newR.origin.y + t1 * newR.direction.y) < half_h)
				best = min(best, t1);
		}
	}

	if (abs(newR.direction.y) > EPSILON)
	{
		float tb = (-half_h - newR.origin.y) / newR.direction.y;
		float tt = ( half_h - newR.origin.y) / newR.direction.y;
		if (tb > EPSILON && cap_ok(newR, tb))
			best = min(best, tb);
		if (tt > EPSILON && cap_ok(newR, tt))
			best = min(best, tt);
	}
	return best;
}