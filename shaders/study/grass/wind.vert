#version 450 core

layout (location = 0) in vec3 a_local_position;

layout(std140, binding = 0) uniform SceneUniforms {
    mat4 view_from_world;
	mat4 projection_from_view;
	mat4 world_from_local;
} u;

out VertexDataV {
    vec3 color;
} o;

void main() 
{
	o.color = vec3(0.0, 0.0, 1.0);
	gl_Position = u.projection_from_view * u.view_from_world * u.world_from_local * vec4(a_local_position, 1.0);
}