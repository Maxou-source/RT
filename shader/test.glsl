layout(local_size_x = 8, local_size_y = 8) in;

layout(rgba8, binding = 0) uniform writeonly image2D outputImage;

void main()
{
    ivec2 pixel = ivec2(gl_GlobalInvocationID.xy);

    if (pixel.x >= imageSize(outputImage).x ||
        pixel.y >= imageSize(outputImage).y)
        return;

    imageStore(outputImage, pixel, vec4(1.0, 0.0, 0.0, 1.0));
}