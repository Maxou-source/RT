

float intersect_pl(Ray r, Obj o) {
	Ray newR;
	newR.origin    = r.origin    * o.inv_transform;
	newR.direction = r.direction * o.inv_transform;

	if (abs(newR.direction.y - 0) < EPSILON || abs(newR.origin.y - 0) < EPSILON)
		return INF;
	float t = (-newR.origin.y) / newR.direction.y;
	if (t < 0)
		return INF;
	return t;
}