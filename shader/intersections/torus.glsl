
// Solves a monic quadratic:
// x² + b*x + c = 0
// Returns the number of distinct real roots.

int SolveQuadratic(float b, float c, out float roots[2])
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

    // Numerically stable quadratic formula

    float signB = (b >= 0.0) ? 1.0 : -1.0;

    float q = -0.5 * (b + signB * s);

    roots[0] = q;
    roots[1] = c / q;

    // Sort roots

    if (roots[0] > roots[1])
    {
        float temp = roots[0];
        roots[0] = roots[1];
        roots[1] = temp;
    }

    return 2;
}


// Adds a root if it is not already present.

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


// Solves the cubic resolvent for a positive real z:
//
// z³ + 2*p*z² + (p² - 4*r)*z - q² = 0
//
// For q != 0, this cubic always has at least
// one positive real root.

float SolveResolventCubic(float p, float q, float r)
{
    float c2 = 2.0 * p;
    float c1 = p*p - 4.0*r;
    float c0 = -q*q;

    // Find an upper bound for the positive root.

    float hi = max(
        1.0,
        max(
            abs(c2),
            max(sqrt(abs(c1)), pow(abs(q), 2.0/3.0))
        )
    );

    float lo = 0.0;

    // Find an interval containing a positive root.

    for (int i = 0; i < 64; i++)
    {
        float f = ((hi + c2)*hi + c1)*hi + c0;

        if (f >= 0.0)
            break;

        hi *= 2.0;
    }

    // Bisection

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


// Solves:
// a*x⁴ + b*x³ + c*x² + d*x + e = 0
//
// Returns the number of distinct real roots.
// Roots are sorted in ascending order.

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

    // STEP 1: Normalize coefficients

    float A = b / a;
    float B = c / a;
    float C = d / a;
    float D = e / a;


    // STEP 2: Depress the quartic
    // Substitute x = y - A/4

    float p = B - 3.0*A*A/8.0;

    float q = C - A*B/2.0 + A*A*A/8.0;

    float r = D - 3.0*A*A*A*A/256.0
                + A*A*B/16.0
                - A*C/4.0;


    int count = 0;


    // STEP 3: Special case: biquadratic equation
    // If q = 0:
    // y⁴ + p*y² + r = 0
    //
    // Substitute v = y².

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
        // STEP 4: Solve Ferrari's resolvent cubic

        float z = SolveResolventCubic(p, q, r);

        if (z <= 0.0)
            return 0;

        float s = sqrt(z);


        // STEP 5: Split quartic into two quadratics

        // First quadratic:
        // y² - s*y + (p+z)/2 + q/(2*s) = 0

        float roots1[2];

        float b1 = -s;

        float c1 = (p + z)/2.0 + q/(2.0*s);

        int count1 = SolveQuadratic(b1, c1, roots1);


        // Second quadratic:
        // y² + s*y + (p+z)/2 - q/(2*s) = 0

        float roots2[2];

        float b2 = s;

        float c2 = (p + z)/2.0 - q/(2.0*s);

        int count2 = SolveQuadratic(b2, c2, roots2);


        // STEP 6: Collect real roots
        // Convert y back to x:
        // x = y - A/4

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


    // STEP 7: Sort roots from smallest to largest

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
    // STEP 1: Transform ray into object space

    Ray newR;
    newR.origin = r.origin * torus.inv_transform;
    newR.direction = r.direction * torus.inv_transform;

    vec3 D = newR.origin.xyz;
    vec3 E = newR.direction.xyz;

    float A = torus.param;
    float B = torus.param2;


    // STEP 2: Calculate the constants

    float G = 4 * A*A * (E.x*E.x + E.y*E.y);

    float H = 8 * A*A * (D.x*E.x + D.y*E.y);

    float I = 4 * A*A * (D.x*D.x + D.y*D.y);

    float J = dot(E, E);

    float K = 2 * dot(D, E);

    float L = dot(D, D) + A*A - B*B;


    // STEP 3: Build the quartic equation

    float a = J*J;

    float b = 2 * J*K;

    float c = 2*J*L + K*K - G;

    float d = 2*K*L - H;

    float e = L*L - I;


    // STEP 4: Solve the quartic equation

    float roots[4];

    int count = SolveQuarticEquation(a, b, c, d, e, roots);


    // STEP 5: Find the closest positive intersection

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
