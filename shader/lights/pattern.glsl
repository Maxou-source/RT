
vec4 stripe_at(Pattern pat, vec4 point, vec4 color)
{
	// float scaled_x = point.x / pat.param;
	if (mod(floor(point.x), 2.0) == 0.0)
		return (pat.color);
	else
		return (color);
}

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

float fade(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }
float lerp(float t, float a, float b) { return a + t * (b - a); }
float grad(int hash, float x, float y, float z) {
	int h = hash & 15;                      // CONVERT LO 4 BITS OF HASH CODE
	float u = h<8 ? x : y,                 // INTO 12 GRADIENT DIRECTIONS.
			v = h<4 ? y : h==12||h==14 ? x : z;
	return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
}

float perlin(float x, float y, float z)
{
	int p[512] = {151,160,137,91,90,15,
   131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
   190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
   88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
   77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
   102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
   135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
   5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
   223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
   129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
   251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
   49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
   138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,151,160,137,91,90,15,
   131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
   190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
   88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
   77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
   102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
   135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
   5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
   223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
   129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
   251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
   49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
   138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
   };
    int X = int(floor(x)) & 255;
    int Y = int(floor(y)) & 255;
    int  Z = int(floor(z)) & 255;
      x -= floor(x);
      y -= floor(y);
      z -= floor(z);
      float u = fade(x),
             v = fade(y),
             w = fade(z);
      int A = p[X  ]+Y, AA = p[A]+Z, AB = p[A+1]+Z,
          B = p[X+1]+Y, BA = p[B]+Z, BB = p[B+1]+Z;

      return lerp(w, lerp(v, lerp(u, grad(p[AA  ], x  , y  , z   ),
                                     grad(p[BA  ], x-1, y  , z   )),
                             lerp(u, grad(p[AB  ], x  , y-1, z   ),
                                     grad(p[BB  ], x-1, y-1, z   ))),
                     lerp(v, lerp(u, grad(p[AA+1], x  , y  , z-1 ),
                                     grad(p[BA+1], x-1, y  , z-1 )),
                             lerp(u, grad(p[AB+1], x  , y-1, z-1 ),
                                     grad(p[BB+1], x-1, y-1, z-1 ))));
}

vec4 perlin_at(Pattern pat, vec4 p, vec4 colorA)
{
	float n = perlin(p.x * pat.param, p.y * pat.param, p.z * pat.param);
	 n = n * 0.5 + 0.5;
	return mix(colorA, pat.color, n);
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

	if (mater.pat.patType == STRIPE)
		return stripe_at(mater.pat, pattern_point, mater.color);
	else if (mater.pat.patType == GRADIENT)
		return gradient_at(mater.pat, pattern_point, mater.color);
	else if (mater.pat.patType == CHECKER && obj.type == OBJ_SPHERE)
		return checker_uv_at(mater.pat, world_point, mater.color);
	else if (mater.pat.patType == CHECKER)
		return checker_at(mater.pat, pattern_point, mater.color);
	else if (mater.pat.patType == PERLIN)
		return perlin_at(mater.pat, point, mater.color);
	else if (mater.pat.patType == RING)
		return ring_at(mater.pat, pattern_point, mater.color);
}