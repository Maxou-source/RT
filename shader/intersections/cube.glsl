
// xtmin, xtmax ← check_axis(ray.origin.x, ray.direction.x)
// ytmin, ytmax ← check_axis(ray.origin.y, ray.direction.y)
// ztmin, ztmax ← check_axis(ray.origin.z, ray.direction.z)

/*
function check_axis(origin, direction)
tmin_numerator = (-1 - origin)
tmax_numerator = (1 - origin)
if abs(direction) >= EPSILON
tmin ← tmin_numerator / direction
tmax ← tmax_numerator / direction
else
tmin ← tmin_numerator * INFINITY
tmax ← tmax_numerator * INFINITY
end if
if tmin > tmax then swap(tmin, tmax)
return tmin, t*/

float check_axis(vec4 origin, vec4 direction)
{
	vec4 tmin = (-1 - origin);
	vec4 tmax = (1 - origin);
	return tmin.x;
}

float intersect_cu(Ray r, Obj o)
{
	Ray newR;
	newR.origin    = r.origin    * o.inv_transform;
	newR.direction = r.direction * o.inv_transform;

	return INF;
}