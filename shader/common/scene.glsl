
layout(std140) uniform Scene {
    camera cam;
    int    object_count;
    int    light_count;
    int    _pad0, _pad1;
    Light  lights[MAX_LIGHTS];
    Obj    objs[MAX_OBJECTS];
};
