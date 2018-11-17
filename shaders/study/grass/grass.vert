#version 450 core

layout (location = 0) in vec3 a_local_position;

out VertexDataV {
    vec3 local_position;
} o;

void main() 
{
	o.local_position = a_local_position;
}