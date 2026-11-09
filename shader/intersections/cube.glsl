
vec2 check_axis(float origin, float direction)
{
	float tmin, tmax;
	float tmin_numerator = (-1.0 - origin);
	float tmax_numerator = (1.0 - origin);

	if (abs(direction) >= EPSILON) {
		tmin = tmin_numerator / direction;
		tmax = tmax_numerator / direction;
	} else {
		tmin = (tmin_numerator >= 0.0) ? INF : -INF;
		tmax = (tmax_numerator >= 0.0) ? INF : -INF;
	}

	if (tmin > tmax) {
		float tmp = tmin;
		tmin = tmax;
		tmax = tmp;
	}
	return vec2(tmin, tmax);
}

float intersect_cu(Ray r, Obj o)
{
	Ray newR;
	newR.origin    = r.origin    * o.inv_transform;
	newR.direction = r.direction * o.inv_transform;

	vec2 x = check_axis(newR.origin.x, newR.direction.x);
	vec2 y = check_axis(newR.origin.y, newR.direction.y);
	vec2 z = check_axis(newR.origin.z, newR.direction.z);

	float tmin = max(max(x[0], y[0]), z[0]);
	float tmax = min(min(x[1],y[1]), z[1]);

	if (tmin > tmax || tmax < EPSILON) {return INF;}
	return (tmin > EPSILON) ? tmin : tmax;
}
