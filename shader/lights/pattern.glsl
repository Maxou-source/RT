
vec4 stripe_at(Pattern pat, vec4 point, vec4 color)
{
	// float scaled_x = point.x / pat.param;
	if (mod(floor(point.x), 2.0) == 0.0)
		return (pat.color);
	else
		return (color);
}
// color(p, ca , cb ) = ca + (cb − ca ) ∗ (px − floor(px ))
vec4 gradient_at(Pattern pat, vec4 point, vec4 colorA)
{
	float f = clamp(point.x, 0.0, 1.0);
	return mix(colorA, pat.color, f);
}

vec4 checker_at(Pattern pat, vec4 point, vec4 colorA)
{
	point = point + 0.0001;
	if (mod((floor(point.x) + floor(point.y) + floor(point.z)), 2.0) == 0.0)
	{
		return colorA;
	}
	return pat.color;
}

vec2 spherical_map(vec4 p)
{
	float theta = atan(p.x, p.z);
	float r     = length(p);
	float phi   = acos(clamp(p.y / r, -1.0, 1.0));

	float raw_u = theta / (2.0 * 3.14159265359);
	float u = 1.0 - (raw_u + 0.5);
	float v = 1.0 - phi / 3.14159265359;
	return vec2(u, v);
}

vec4 checker_uv_at(Pattern pat, vec4 p, vec4 colorA)
{
	vec2 uv = spherical_map(p);

	float w = pat.param * 2.0;
	float h = pat.param;

	float s = floor(uv.x * w) + floor(uv.y * h);
	return (mod(s, 2.0) == 0.0) ? colorA : pat.color;
}

vec3 hash3(vec3 p)
{
	p = vec3(dot(p, vec3(127.1, 311.7,  74.7)),
	         dot(p, vec3(269.5, 183.3, 246.1)),
	         dot(p, vec3(113.5, 271.9, 124.6)));
	return -1.0 + 2.0 * fract(sin(p) * 43758.5453123);
}

float perlin(vec3 p)
{
	vec3 i = floor(p);
	vec3 f = fract(p);
	vec3 u = f * f * f * (f * (f * 6.0 - 15.0) + 10.0);   // fade

	float n000 = dot(hash3(i + vec3(0,0,0)), f - vec3(0,0,0));
	float n100 = dot(hash3(i + vec3(1,0,0)), f - vec3(1,0,0));
	float n010 = dot(hash3(i + vec3(0,1,0)), f - vec3(0,1,0));
	float n110 = dot(hash3(i + vec3(1,1,0)), f - vec3(1,1,0));
	float n001 = dot(hash3(i + vec3(0,0,1)), f - vec3(0,0,1));
	float n101 = dot(hash3(i + vec3(1,0,1)), f - vec3(1,0,1));
	float n011 = dot(hash3(i + vec3(0,1,1)), f - vec3(0,1,1));
	float n111 = dot(hash3(i + vec3(1,1,1)), f - vec3(1,1,1));

	return mix(mix(mix(n000, n100, u.x), mix(n010, n110, u.x), u.y),
	           mix(mix(n001, n101, u.x), mix(n011, n111, u.x), u.y), u.z);
}

vec4 perlin_at(Pattern pat, vec4 p, vec4 colorA)
{
	float n = perlin(p.xyz * pat.param) * 0.5 + 0.5;   // remap to 0..1
	return mix(pat.color, colorA, n);
}

vec4 ring_at(Pattern pat, vec4 p, vec4 colorA)
{
	if (mod(floor(sqrt((p.x * p.x) + (p.z* p.z))), 2.0) == 0)
		return colorA;
	return pat.color;
}

vec4 applyPattern(Obj obj, vec4 point)
{
	Material mater = obj.mat;
	vec4 world_point = point *  obj.inv_transform;
	vec4 pattern_point = world_point * mater.pat.i_m;
	// if (obj.mat.wobble != 0.0)
	// 	pattern_point = wobble_at(obj.mat.wobble, pattern_point);

	if (mater.pat.patType == STRIPE)
		return stripe_at(mater.pat, pattern_point, mater.color);
	else if (mater.pat.patType == GRADIENT)
		return gradient_at(mater.pat, pattern_point, mater.color);
	else if (mater.pat.patType == CHECKER && obj.type == OBJ_SPHERE)
		return checker_uv_at(mater.pat, world_point, mater.color);
	else if (mater.pat.patType == CHECKER)
		return checker_at(mater.pat, pattern_point, mater.color);
	else if (mater.pat.patType == PERLIN)
		return perlin_at(mater.pat, pattern_point, mater.color);
	else if (mater.pat.patType == RING)
		return ring_at(mater.pat, pattern_point, mater.color);
}