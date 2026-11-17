
//!include "../intersections/sphere.glsl"
//!include "../intersections/cylinder.glsl"
//!include "../intersections/cube.glsl"
//!include "../intersections/plane.glsl"
//!include "../intersections/cone.glsl"
//!include "../intersections/torus.glsl"
//!include "../intersections/group.glsl"
//!include "../intersections/paraboloid.glsl"
bool is_csg(int type)
{
	return type == CSG_UNION || type == CSG_INTERSECT || type == CSG_DIFFERENCE;
}

float intersect_obj(Ray r, Obj o)
{
	if (o.type == OBJ_SPHERE) {
		return(intersect_sp(r, o));
	}
	else if (o.type == OBJ_CYLINDER) {
		return(intersect_cy(r, o));
	}
	else if (o.type == OBJ_CUBE) {
		return(intersect_cu(r, o));
	}
	else if (o.type == OBJ_PLANE) {
		return(intersect_pl(r, o));
	}
	else if (o.type == OBJ_CONE) {
		return(intersect_co(r, o));
	}
	else if (o.type == OBJ_TORUS) {
		return (intersect_to(r, o));
	}
	else if (o.type == OBJ_PARABOLOID) {
		return (intersect_pa(r, o));
	}
	return INF;
}

Intersection intersect_top(Ray r, int idx)
{
	if (objs[idx].parent >= 0)
		return Intersection(INF, -1);
	if (is_csg(objs[idx].type))
		return group_intersect(r, objs[idx]);
	return Intersection(intersect_obj(r, objs[idx]), idx);
}

Intersection intersect_world(Ray r)
{
	Intersection best = Intersection(INF, -1);

	for (int i = 0; i < object_count; i++)
	{
		Intersection hit = intersect_top(r, i);

		if (hit.t < best.t && hit.object_id >= 0)
			best = hit;
	}
	return best;
}
