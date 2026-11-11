
const float PI = 3.14159265359;

vec2 map_sphere(vec3 p)
{
    float theta = atan(p.x, p.z);                       // -PI..PI around y
    float phi   = acos(clamp(p.y / length(p), -1.0, 1.0));
    float u = 1.0 - (theta / (2.0 * PI) + 0.5);
    float v = phi / PI;                                  // 0 at the top: matches image row 0
    return vec2(u, v);
}

vec2 map_plane(vec3 p)
{
    return vec2(fract(p.x), fract(p.z));
}

vec2 map_cylinder(vec3 p)
{
    float theta = atan(p.x, p.z);
    float u = 1.0 - (theta / (2.0 * PI) + 0.5);
    float v = 1.0 - fract(p.y);                          // repeats every unit in y
    return vec2(u, v);
}

vec2 map_cube(vec3 p)
{
    vec3 a = abs(p);
    vec2 uv;
    if (a.x >= a.y && a.x >= a.z)
        uv = (p.x > 0.0) ? vec2(-p.z, p.y) : vec2( p.z, p.y);   // +x / -x face
    else if (a.y >= a.z)
        uv = (p.y > 0.0) ? vec2( p.x, -p.z) : vec2( p.x, p.z);  // +y / -y face
    else
        uv = (p.z > 0.0) ? vec2( p.x, p.y) : vec2(-p.x, p.y);   // +z / -z face
    uv = uv * 0.5 + 0.5;                                          // -1..1 -> 0..1
    return vec2(uv.x, 1.0 - uv.y);
}

vec4 applyTexture(Obj obj, vec4 point)
{
    vec4 p = (point * obj.inv_transform) * obj.mat.text.i_m;      // object space, then texture space
    vec2 uv;

    if      (obj.type == OBJ_SPHERE)   uv = map_sphere(p.xyz);
    else if (obj.type == OBJ_PLANE)    uv = map_plane(p.xyz);
    else if (obj.type == OBJ_CYLINDER) uv = map_cylinder(p.xyz);
    else if (obj.type == OBJ_CUBE)     uv = map_cube(p.xyz);
    else                             uv = map_sphere(p.xyz);   // cone: placeholder

    return textureLod(textures, vec3(uv, float(obj.mat.text.layer)), 0.0);
}