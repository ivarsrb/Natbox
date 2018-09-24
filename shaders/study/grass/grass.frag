#version 450 core

in VertexDataV {
    vec3 color;
} i;

layout(location = 0) out vec4 out_fragment_color;

void main() 
{
	out_fragment_color = vec4(i.color, 1.0f);
}