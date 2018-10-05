#version 450 core

layout (location = 0) in vec3 a_local_position;

out VertexDataV {
    vec3 color;
} o;


void main() 
{
	//o.color = vec3(0.0, 1.0, 0.0);
	gl_Position = vec4(a_local_position, 1.0);
}