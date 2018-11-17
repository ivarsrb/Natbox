#version 450 core

layout (isolines, equal_spacing) in;

in VertexDataTC {
    vec3 local_position;
} i[];

out VertexDataTE {
    vec3 local_position;
    float texture_v;
} o;

void main() 
{
    // interpolated value
    float t = gl_TessCoord.x;
    o.local_position = pow(1 - t, 3)*i[0].local_position + 3*pow(1 - t, 2)*t*i[1].local_position + 3*(1 - t)*t*t*i[2].local_position + t*t*t*i[3].local_position;
    o.texture_v = t;
}
