#version 450 core
layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

layout(std140, binding = 0) uniform SceneUniforms {
    mat4 view_from_world;
	mat4 projection_from_view;
	mat4 world_from_local;
} u;

in VertexDataTE {
    vec3 local_position;
    float texture_v;
    vec3 side_vec;
} i[];

out VertexDataG {
    vec3 color;
    vec2 texture;
} o;

//gl_PrimitiveIDIn  - may not worl with tessalation shader !

void main() 
{    
    // predetermined
    const float half_width = 0.1;
    const vec3 white_color = vec3(1.0, 1.0, 1.0);

    vec4 local_bottom_left = vec4(i[0].local_position + i[0].side_vec * -half_width, 1.0);
    vec4 local_bottom_right = vec4(i[0].local_position + i[0].side_vec * half_width, 1.0);
    vec4 local_top_left = vec4(i[1].local_position + i[1].side_vec * -half_width, 1.0);
    vec4 local_top_right = vec4(i[1].local_position + i[1].side_vec * half_width, 1.0);

    mat4 projection_from_local = u.projection_from_view * u.view_from_world * u.world_from_local;

    gl_Position = projection_from_local * local_bottom_left;
    o.color = white_color;
    o.texture = vec2(0.0, i[0].texture_v);
    EmitVertex();

    gl_Position = projection_from_local * local_bottom_right;
    o.color = white_color;
    o.texture = vec2(1.0, i[0].texture_v);
    EmitVertex();

    gl_Position = projection_from_local * local_top_left;
    o.color = white_color;
    o.texture = vec2(0.0, i[1].texture_v);
    EmitVertex();

    gl_Position = projection_from_local * local_top_right;
    o.color = white_color;
    o.texture = vec2(1.0, i[1].texture_v);
    EmitVertex();
} 