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
    vec3 tangent;
} i[];

out VertexDataG {
    vec3 color;
    vec2 texture;
} o;

//gl_PrimitiveIDIn  - may not worl with selaation shader !

void main() 
{    
    // predetermined
    const float half_width = 0.5;
    const vec3 white_color = vec3(1.0, 1.0, 1.0);

    // find perpendocular for bottom and top to know direction of vertex extrusion
    const vec3 bottom_normal = vec3(0.0, 1.0, 0.0);
    vec3 bottom_perpendicular = normalize(cross(bottom_normal, i[0].tangent));
    const vec3 top_normal = vec3(0.0, 1.0, 0.0);
    vec3 top_perpendicular = normalize(cross(top_normal, i[1].tangent));
    // triangle strip points of the section
    vec4 local_bottom_left = vec4(i[0].local_position + bottom_perpendicular * half_width, 1.0);
    vec4 local_bottom_right = vec4(i[0].local_position + bottom_perpendicular * -half_width, 1.0);
    vec4 local_top_left = vec4(i[1].local_position + top_perpendicular * half_width, 1.0);
    vec4 local_top_right = vec4(i[1].local_position + top_perpendicular * -half_width, 1.0);

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