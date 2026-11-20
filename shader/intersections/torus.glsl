

int	SolveQuadratic(float b, float c, out float roots[2])
{
	float discriminant = b*b - 4.0*c;

	float tolerance = 1e-6 * max(1.0, b*b + abs(4.0*c));

	if (discriminant < -tolerance)
		return 0;

	if (discriminant < 0.0)
		discriminant = 0.0;

	float s = sqrt(discriminant);

	if (s == 0.0)
	{
		roots[0] = -b / 2.0;
		return 1;
	}

	float signB = (b >= 0.0) ? 1.0 : -1.0;
	float q = -0.5 * (b + signB * s);

	roots[0] = q;
	roots[1] = c / q;
	if (roots[0] > roots[1])
	{
		float temp = roots[0];
		roots[0] = roots[1];
		roots[1] = temp;
	}
	return 2;
}


void AddUniqueRoot(
	inout float roots[4],
	inout int count,
	float value)
{
	for (int i = 0; i < count; i++)
	{
		float tolerance = 1e-5 * max(1.0, abs(value));

		if (abs(roots[i] - value) < tolerance)
			return;
	}

	if (count < 4)
	{
		roots[count] = value;
		count++;
	}
}

float SolveResolventCubic(float p, float q, float r)
{
	float c2 = 2.0 * p;
	float c1 = p*p - 4.0*r;
	float c0 = -q*q;

	float hi = max(
		1.0,
		max(
			abs(c2),
			max(sqrt(abs(c1)), pow(abs(q), 2.0/3.0))
		)
	);

	float lo = 0.0;

	for (int i = 0; i < 64; i++)
	{
		float f = ((hi + c2)*hi + c1)*hi + c0;

		if (f >= 0.0)
			break;

		hi *= 2.0;
	}

	for (int i = 0; i < 64; i++)
	{
		float mid = (lo + hi) * 0.5;

		float f = ((mid + c2)*mid + c1)*mid + c0;

		if (f > 0.0)
			hi = mid;
		else
			lo = mid;
	}

	return (lo + hi) * 0.5;
}

int SolveQuarticEquation(
	float a,
	float b,
	float c,
	float d,
	float e,
	out float roots[4])
{
	if (a == 0.0)
		return 0;

	float A = b / a;
	float B = c / a;
	float C = d / a;
	float D = e / a;


	float p = B - 3.0*A*A/8.0;
	float q = C - A*B/2.0 + A*A*A/8.0;
	float r = D - 3.0*A*A*A*A/256.0
				+ A*A*B/16.0
				- A*C/4.0;
	int count = 0;


	if (q == 0.0)
	{
		float vRoots[2];

		int vCount = SolveQuadratic(p, r, vRoots);

		for (int i = 0; i < vCount; i++)
		{
			float v = vRoots[i];

			float tolerance = 1e-6 * max(1.0, abs(v));

			if (v < -tolerance)
				continue;

			v = max(v, 0.0);

			float y = sqrt(v);

			AddUniqueRoot(roots, count, y - A/4.0);

			if (y > 0.0)
				AddUniqueRoot(roots, count, -y - A/4.0);
		}
	}
	else
	{
		float z = SolveResolventCubic(p, q, r);

		if (z <= 0.0)
			return 0;

		float s = sqrt(z);
		float roots1[2];
		float b1 = -s;
		float c1 = (p + z)/2.0 + q/(2.0*s);
		int count1 = SolveQuadratic(b1, c1, roots1);
		float roots2[2];
		float b2 = s;
		float c2 = (p + z)/2.0 - q/(2.0*s);
		int count2 = SolveQuadratic(b2, c2, roots2);

		for (int i = 0; i < count1; i++)
		{
			AddUniqueRoot(
				roots,
				count,
				roots1[i] - A/4.0
			);
		}

		for (int i = 0; i < count2; i++)
		{
			AddUniqueRoot(
				roots,
				count,
				roots2[i] - A/4.0
			);
		}
	}

	for (int i = 0; i < count; i++)
	{
		for (int j = i + 1; j < count; j++)
		{
			if (roots[i] > roots[j])
			{
				float temp = roots[i];
				roots[i] = roots[j];
				roots[j] = temp;
			}
		}
	}

	return count;
}

float intersect_to(Ray r, Obj torus)
{
	Ray newR;
	newR.origin = r.origin * torus.inv_transform;
	newR.direction = r.direction * torus.inv_transform;
	vec3 D = newR.origin.xyz;
	vec3 E = newR.direction.xyz;
	float A = torus.param;
	float B = torus.param2;

	float G = 4 * A*A * (E.x*E.x + E.y*E.y);
	float H = 8 * A*A * (D.x*E.x + D.y*E.y);
	float I = 4 * A*A * (D.x*D.x + D.y*D.y);
	float J = dot(E, E);
	float K = 2 * dot(D, E);
	float L = dot(D, D) + A*A - B*B;

	float a = J*J;

	float b = 2 * J*K;

	float c = 2*J*L + K*K - G;

	float d = 2*K*L - H;

	float e = L*L - I;

	float roots[4];

	int count = SolveQuarticEquation(a, b, c, d, e, roots);

	float closest = INF;

	for (int i = 0; i < count; i++)
	{
		if (roots[i] > 0 && roots[i] < closest)
		{
			closest = roots[i];
		}
	}

	return closest;
}
