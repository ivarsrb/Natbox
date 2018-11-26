#version 450 core

layout (isolines, equal_spacing) in;

in VertexDataTC {
    vec3 local_position;
} i[];

out VertexDataTE {
    vec3 local_position;
    float texture_v;
    vec3 tangent;
} o;

void main() 
{
    // interpolated value
    float t = gl_TessCoord.x;
    // Cubic Bezier curve equation
    o.local_position = pow(1 - t, 3)*i[0].local_position + 3*pow(1 - t, 2)*t*i[1].local_position + 3*(1 - t)*t*t*i[2].local_position + t*t*t*i[3].local_position;
    // tangent, first derivative
    o.tangent = 3.0*(1 - t)*(1 - t)*(i[1].local_position - i[0].local_position) + 6.0*(1 - t)*t*(i[2].local_position - i[1].local_position) + 3.0*t*t*(i[3].local_position - i[2].local_position);
    // TODO: not a precise way to determine texture cooridnates for each section
    o.texture_v = distance(i[0].local_position, o.local_position) / distance(i[0].local_position ,i[3].local_position );
}
