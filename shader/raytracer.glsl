
layout(local_size_x = 8, local_size_y = 8) in;
layout(rgba8, binding = 0) uniform writeonly image2D outImage;
//!include "common/objects.glsl"
//!include "common/scene.glsl"
//!include "trace/manage_intersect.glsl"
//!include "lights/lighting.glsl"
//!include "lights/normals.glsl"
//!include "lights/pattern.glsl"
//!include "lights/texture.glsl"
//!include "trace/color_at.glsl"

Ray	rayForPixel(int px, int py, int index) {
	float	world_x;
	float	world_y;
	vec4	world_point;
	vec4	pixel;
	Ray r;

	world_x = cam.half_width - float(px) * cam.pixel_size;
	world_y = -cam.half_height + float(py) * cam.pixel_size;
	world_point = vec4(world_x, world_y, -1.0, 1.0);
	pixel = world_point * cam.inv_transform;
	r.origin = (vec4(0,0,0,1.0) * cam.inv_transform);
	r.direction = (normalize(pixel - r.origin));
	if (px == 200 && py == 200)
	{
		debugData[index+9] = world_point;
		debugData[index+10] = r.origin;
		debugData[index+11] = r.direction;
	}
	return r;
}

void main()
{
	ivec2 size = imageSize(outImage);
	ivec2 p = ivec2(gl_GlobalInvocationID.xy);

	if (p.x >= size.x || p.y >= size.y)
		return ;

	// Pixel center, equivalent to gl_FragCoord
	vec2 frag = vec2(p) + vec2(0.5);

	// Pixel index
	int index = p.y * size.x + p.x;

	// Generate ray and trace it
	Ray ray = rayForPixel(p.x, p.y, index);
	vec4 color = color_at(ray);

	// Write final pixel
	imageStore(outImage, p, vec4(color.rgb, 1.0));
}