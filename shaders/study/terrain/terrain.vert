#version 450 core

layout (location = 0) in vec3 a_local_position;

layout(std140, binding = 0) uniform SceneUniforms {
    mat4 view_from_world;
	mat4 projection_from_view;
	mat4 world_from_local;
} u;

layout(binding = 20) uniform sampler2D u_heightmap;

out VertexData {
    vec3 color;
	vec2 texture;
} o;

void main() 
{
	float height = texture(u_heightmap, vec2(a_local_position.x, a_local_position.z)).r;
	vec3 local_position = vec3(a_local_position.x, a_local_position.y + height, a_local_position.z);

	o.color = vec3(1.0, 1.0, 1.0);
	
	vec4 world_position = u.world_from_local * vec4(local_position, 1.0);
	o.texture = vec2(world_position.x, world_position.z);

	gl_Position = u.projection_from_view * u.view_from_world * world_position;
}