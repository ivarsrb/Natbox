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
} i[];

out VertexDataG {
    vec3 color;
    vec2 texture;
} o;

//gl_PrimitiveIDIn  - may not worl with selaation shader

void main() 
{    
    // predetermined
    const float section_count = 6.0;
    const float half_width = 0.5;
    const vec3 white_color = vec3(1.0, 1.0, 1.0);
    // each primitive it's own color
    const vec3 COLOR_MASKS[3] = vec3[]( vec3( 0.0, 1.0, 0.0 ),
                                        vec3( 0.5, 0.5, 0.5 ),
                                        vec3( 1.0, 0.5, 0.5 )
                                        );
    // line direction
    vec4 world_position_1 = u.world_from_local * vec4(i[0].local_position, 1.0);
    vec4 world_position_2 = u.world_from_local * vec4(i[1].local_position, 1.0);
    vec4 world_origin_vec = world_position_2 - world_position_1;
    // side vector to line
    vec4 world_line_normal = vec4(0.0, 0.0, 0.0, 0.0);
    world_line_normal.xyz = cross(vec3(0.0, 1.0, 0.0), world_origin_vec.xyz);
/*
    vec4 world_line_normal = vec4(0.0, 0.0, 0.0, 0.0);
    world_line_normal.x = world_position_1.x - world_position_2.z;
    world_line_normal.z = world_position_2.x - world_position_world_line_normal1.z;
  */
    world_line_normal = normalize(world_line_normal);
    
    // mat4 projection_from_local = u.projection_from_view * uworld_line_normal.view_from_world * u.world_from_local;
    mat4 projection_from_world = u.projection_from_view * u.view_from_world;
    //////////////////////////////////////////////////////////world_line_normal//////////////
    //gl_Position = projection_from_local * (gl_in[0].gl_Positworld_line_normalion + vec4(half_width, 0.0, 0.0, 0.0));
    gl_Position = projection_from_world * (world_position_1 + world_line_normal * half_width);
    //o.color = COLOR_MASKS[gl_PrimitiveIDIn];
    o.color = vec3(1.0, 0.5, 0.5);
    o.texture = vec2(0.0, i[0].texture_v);
    EmitVertex();

    //gl_Position = projection_from_local * (gl_in[0].gl_Position + vec4(-half_width, 0.0, 0.0, 0.0));
    gl_Position = projection_from_world * (world_position_1 + world_line_normal * -half_width);
    //o.color = COLOR_MASKS[gl_PrimitiveIDIn];
    o.color = white_color;
    o.texture = vec2(1.0, i[0].texture_v);
    EmitVertex();

    //gl_Position = projection_from_local * (gl_in[1].gl_Position + vec4(half_width, 0.0, 0.0 ,0.0));
    gl_Position = projection_from_world * (world_position_2 + world_line_normal * half_width);
    //o.color = COLOR_MASKS[gl_PrimitiveIDIn];
    o.color = vec3(1.0, 0.5, 0.5);
    o.texture = vec2(0.0, i[1].texture_v);
    EmitVertex();

    //gl_Position = projection_from_local * (gl_in[1].gl_Position + vec4(-half_width, 0.0, 0.0, 0.0));
    gl_Position = projection_from_world * (world_position_2 + world_line_normal * -half_width);
    //o.color = COLOR_MASKS[gl_PrimitiveIDIn];
    o.color = white_color;
    o.texture = vec2(1.0, i[1].texture_v);
    EmitVertex();
    /*
    for (n = 0; n < gl_in.length(); n++) {
        gl_Position = gl_in[n].gl_Position; 
        o.color = i[n].color;
        EmitVertex();
    }
    EndPrimitive();
    */
} 