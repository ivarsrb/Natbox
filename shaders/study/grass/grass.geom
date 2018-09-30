#version 450 core
layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

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
    float half_width = 0.14;
    gl_Position = gl_in[0].gl_Position + vec4(half_width, 0.0, 0.0, 0.0);
    o.color = i[0].color;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(-half_width, 0.0, 0.0, 0.0);
    o.color = i[0].color;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position + vec4(half_width, 0.0 ,0.0 ,0.0);
    o.color = i[0].color;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position + vec4(-half_width, 0.0, 0.0, 0.0);
    o.color = i[0].color;
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