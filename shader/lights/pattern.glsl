
vec4 stripe_at(Pattern pat, vec4 point, vec4 color)
{
	float scaled_x = point.x / pat.param;
	if (mod(floor(scaled_x), 2.0) == 0.0)
		return (pat.color);
	else
		return (color);
}

vec4 applyPattern(Obj obj, vec4 point)
{
	Material mater = obj.mat;
	vec4 world_point = obj.inv_transform * point;
	if (mater.pat.patType == STRIPE)
		return stripe_at(mater.pat, world_point, mater.color);
}