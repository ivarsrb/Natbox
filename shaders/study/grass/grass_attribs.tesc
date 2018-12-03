#version 450 core

layout (vertices = 4) out;

in VertexDataV {
    vec3 local_position;
    vec3 edge_vec;
    vec3 normal_vec;
    vec3 side_vec;
} i[];

out VertexDataTC {
    vec3 local_position;
    vec3 edge_vec;
    vec3 normal_vec;
    vec3 side_vec;
} o[];

void main() 
{
    o[gl_InvocationID].local_position = i[gl_InvocationID].local_position;
    o[gl_InvocationID].edge_vec = i[gl_InvocationID].edge_vec;
    o[gl_InvocationID].normal_vec = i[gl_InvocationID].normal_vec;
    o[gl_InvocationID].side_vec = i[gl_InvocationID].side_vec;

    // levels only need to be set once per patch (TODO maybe this 'if' is not neded because we draw only one path or all together?)
    if(gl_InvocationID == 0) { 
        // only one line is tessalted
        gl_TessLevelOuter[0] = 1; 
        // tessalte into n number of segments
        gl_TessLevelOuter[1] = 3; 
    }
}