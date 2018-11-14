#version 450 core
layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

layout(std140, binding = 0) uniform SceneUniforms {
    mat4 view_from_world;
	mat4 projection_from_view;
	mat4 world_from_local;
} u;

// From Vertex shader
in VertexDataV {
    vec3 color;
} i[];

// To fragment shader
out VertexDataG {
    vec3 color;
} o;

//gl_PrimitiveIDIn

void main() 
{    
    // each primitive it's own color
    const vec3 COLOR_MASKS[3] = vec3[]( vec3( 0.0, 1.0, 0.0 ),
                                        vec3( 0.5, 0.5, 0.5 ),
                                        vec3( 1.0, 0.5, 0.5 )
                                        );


    mat4 projection_from_local = u.projection_from_view * u.view_from_world * u.world_from_local;
    float half_width = 0.12;
    gl_Position = projection_from_local * (gl_in[0].gl_Position + vec4(half_width, 0.0, 0.0, 0.0));
    o.color = COLOR_MASKS[gl_PrimitiveIDIn];
    EmitVertex();

    gl_Position = projection_from_local * (gl_in[0].gl_Position + vec4(-half_width, 0.0, 0.0, 0.0));
    o.color = COLOR_MASKS[gl_PrimitiveIDIn];
    EmitVertex();

    gl_Position = projection_from_local * (gl_in[1].gl_Position + vec4(half_width, 0.0, 0.0 ,0.0));
    o.color = COLOR_MASKS[gl_PrimitiveIDIn];
    EmitVertex();

    gl_Position = projection_from_local * (gl_in[1].gl_Position + vec4(-half_width, 0.0, 0.0, 0.0));
    o.color = COLOR_MASKS[gl_PrimitiveIDIn];
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