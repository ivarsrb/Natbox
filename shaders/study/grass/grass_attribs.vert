#version 450 core

layout (location = 0) in vec3 a_local_position;
layout (location = 1) in vec3 a_edge_vec;
layout (location = 2) in vec3 a_normal_vec;
layout (location = 3) in vec3 a_side_vec;

out VertexDataV {
    vec3 local_position;
    vec3 edge_vec;
    vec3 normal_vec;
    vec3 side_vec;
} o;

void main() 
{
	o.local_position = a_local_position;
    o.edge_vec = a_edge_vec;
    o.normal_vec = a_normal_vec;
    o.side_vec = a_side_vec;
}