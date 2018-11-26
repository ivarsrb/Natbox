#version 450 core

layout (vertices = 4) out;

in VertexDataV {
    vec3 local_position;
} i[];

out VertexDataTC {
    vec3 local_position;
} o[];

void main() 
{
    o[gl_InvocationID].local_position = i[gl_InvocationID].local_position;
    // levels only need to be set once per patch (TODO maybe this 'if' is not neded because we draw only one path or all together?)
    if(gl_InvocationID == 0) { 
        // only one line is tessalted
        gl_TessLevelOuter[0] = 1; 
        // tessalte into n number of segments
        gl_TessLevelOuter[1] = 3; 
    }
}