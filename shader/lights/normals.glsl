
vec4 normal_at(Obj o, vec4 world_point)
{
	vec4 local_point  = world_point * o.inv_transform;        // same convention as your intersect_sp
	vec4 local_normal = vec4(local_point.xyz, 0.0);           // unit sphere at origin
	vec4 n            = o.inv_transform * local_normal;
	return vec4(normalize(n.xyz), 0.0);
}

vec4 normal_at_cy(Obj o, vec4 world_point)
{
	vec4 lp = world_point * o.inv_transform;
	vec4 local_normal;

	float half_h = o.param / 2.0;
	float dist   = lp.x * lp.x + lp.z * lp.z;
	if (dist < 1.0 - 0.001 && lp.y >= half_h - 0.001)
		local_normal = vec4(0.0,  1.0, 0.0, 0.0);   // top cap
	else if (dist < 1.0 - 0.001 && lp.y <= -half_h + 0.001)
		local_normal = vec4(0.0, -1.0, 0.0, 0.0);   // bottom cap
	else
	{

		local_normal = vec4(lp.x, 0.0, lp.z, 0.0);  // side
	}

	vec4 n = o.inv_transform * local_normal;           // transpose(inverse) in your convention
	return vec4(normalize(n.xyz), 0.0);
}

vec4 normal_at_co(Obj o, vec4 world_point)
{
	vec4 lp = world_point * o.inv_transform;
	vec4 local_normal;

	float half_h = o.param / 2.0;
	float dist   = lp.x * lp.x + lp.z * lp.z;
	if (dist < 1.0 - 0.001 && lp.y >= half_h - 0.001)
		local_normal = vec4(0.0,  1.0, 0.0, 0.0);   // top cap
	else if (dist < 1.0 - 0.001 && lp.y <= -half_h + 0.001)
		local_normal = vec4(0.0, -1.0, 0.0, 0.0);   // bottom cap
	else
	{
		float y = sqrt((lp.x * lp.x) + (lp.z * lp.z));
		if (lp.y > 0.0)
			y = -y;
		local_normal = vec4(lp.x, y, lp.z, 0.0);  // side
	}

	vec4 n = o.inv_transform * local_normal;           // transpose(inverse) in your convention
	return vec4(normalize(n.xyz), 0.0);
}

vec4 normal_at_cu(Obj o, vec4 world_point)
{
	vec4 p = world_point * o.inv_transform;
	vec3 a = abs(p.xyz);
	float maxC = max(a.x, max(a.y, a.z));

	vec4 local_normal;
	if (maxC == a.x)
		local_normal = vec4(sign(p.x), 0.0, 0.0, 0.0);
	else if (maxC == a.y)
		local_normal = vec4(0.0, sign(p.y), 0.0, 0.0);
	else
		local_normal = vec4(0.0, 0.0, sign(p.z), 0.0);

	vec4 n = o.inv_transform * local_normal;
	return vec4(normalize(n.xyz), 0.0);
}

vec4 normal_at_pl(Obj o, vec4 p)
{
	vec4 world_point = o.inv_transform * vec4(0,1,0,0);
	world_point.w = 0;
	return (normalize(world_point));
}