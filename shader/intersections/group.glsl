
float intersect_obj2(Ray r, Obj o)
{
	if (o.type == OBJ_SPHERE) {
		return(intersect_sp(r, o));
	} else if (o.type == OBJ_CYLINDER) {
		return(intersect_cy(r, o));
	} else if (o.type == OBJ_CUBE) {
		return(intersect_cu(r, o));
	} else if (o.type == OBJ_PLANE) {
		return(intersect_pl(r, o));
	} else if (o.type == OBJ_CONE) {
		return(intersect_co(r, o));
	} else if (o.type == OBJ_TORUS)
		return intersect_to(r, o);
	return INF;
}

Intersection intersect_world2(Ray r)
{
	Intersection best = Intersection(INF, -1);
	for (int i = 0; i < object_count; i++)
	{
		float t;
		t = intersect_obj2(r, objs[i]);
		
		if (t < best.t)
			best = Intersection(t, i);
	}
	return best;
}

bool intersect_allowed(int op_type, bool lhit, bool inl, bool inr)
{
	if (op_type == CSG_UNION)
		return (lhit && !inr) || (!lhit && !inl);
	else if (op_type == CSG_INTERSECT)
		return (lhit && inr) || (!lhit && inl);
	else if (op_type == CSG_DIFFERENCE)
		return (lhit && !inr) || (!lhit && inl);
	return false;
}


Intersection group_intersect(Ray r, Obj o)
{
    Intersection inter = Intersection(INF, -1);

    bool inl = false;
    bool inr = false;

    float traveled = 0.0f;
    float offset = 0.0001f;

    for (int i = 0; i < 100; i++)
    {
        float left  = intersect_obj2(r, objs[o.left]);
        float right = intersect_obj2(r, objs[o.right]);

        if (left == INF && right == INF)
            return inter;

        bool lhit = left < right;
        float t = lhit ? left : right;

        if (intersect_allowed(o.type, lhit, inl, inr))
        {
            return Intersection(traveled + t,
                                lhit ? o.left : o.right);
        }

        if (lhit)
            inl = !inl;
        else
            inr = !inr;

        float advance = t + offset;

        r.origin = r.origin + r.direction * advance;
        traveled += advance;
    }

    return inter;
}

