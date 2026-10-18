float intersect_sp(Ray r, Obj sphere)
{
	Ray newR;
	newR.origin = r.origin * sphere.inv_transform;
	newR.direction = r.direction * sphere.inv_transform;

	vec4 direction = newR.direction;
	vec4 sphere_to_ray = newR.origin - vec4(0,0,0,1);

	float a = dot(direction, direction);
	float b = 2 * (dot(direction, sphere_to_ray));
	float c = dot(sphere_to_ray, sphere_to_ray) - 1;

	float discriminant = (b * b) - (4 * a * c);

	if (discriminant < 0) {
		return INF;
	}
	float t1 = (-b - sqrt(discriminant)) / (2 * a);
	float t2 = (-b + sqrt(discriminant)) / (2 * a);
	if (t1 > 0)
		return t1;
	if (t2 > 0)
		return t2;
	return INF;

}
