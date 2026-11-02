
//!include "../common/objects.glsl"
//!include "../intersections/cube.glsl"

void main()
{
	Ray			ray;
	Obj			obj;

	obj.inv_transform = mat4(1.0);
	obj.mat.color = vec4(1.0, 0.0, 0.0, 0.0);

	ray.origin = vec4(5.0, 0.5, 0, 0);
	ray.direction = vec4(-1, 0, 0, 0);

	float t1 = intersect_cu(ray, obj);
	debugData[0] = vec4(t1, vec3(0.0));
}
