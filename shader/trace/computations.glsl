
vec4 wobble_at(Obj o, vec4 point, vec4 normalv)
{
	vec4 lp = point * o.inv_transform;
	vec3 d = vec3(sin(lp.y * o.mat.wob.frequency), sin(lp.z * o.mat.wob.frequency), sin(lp.x * o.mat.wob.frequency));
	d -= normalv.xyz * dot(d, normalv.xyz);
	return (vec4(normalize(normalv.xyz + o.mat.wob.amplitude * d), 0.0));
}

Computation computations(Ray r, Intersection i, Obj o)
{
	vec4 point = r.origin + i.t * r.direction;
	vec4 eyev  = -r.direction;

	vec4 normalv;
	if		(o.type == OBJ_CYLINDER)	normalv = normal_at_cy(o, point);
	else if	(o.type == OBJ_CUBE)		normalv = normal_at_cu(o, point);
	else if	(o.type == OBJ_PLANE)		normalv = normal_at_pl(o, point);
	else if	(o.type == OBJ_CONE)		normalv = normal_at_co(o, point);
	else if	(o.type == OBJ_TORUS)		normalv = normal_at_to(o, point);
	else if	(o.type == OBJ_PARABOLOID)	normalv = normal_at_pa(o, point);
	else	normalv = normal_at(o, point);

	bool inside = dot(normalv, eyev) < 0.0;
	if (inside)
		normalv = -normalv;

	vec4 over_point  = point + normalv * EPSILON * 1.0;
	vec4 under_point = point - normalv * EPSILON * 1.0;

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