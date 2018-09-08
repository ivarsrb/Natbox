#version 450 core

in VertexData {
    vec3 color;
	vec2 texture;
} i;

layout(binding = 0) uniform sampler2D u_diffuse_map;

layout(location = 0) out vec4 out_fragment_color;

void main() 
{
	vec4 tex_color = texture(u_diffuse_map, i.texture);
	out_fragment_color = tex_color * vec4(i.color, 1.0f);
}