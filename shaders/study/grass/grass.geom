#version 450 core
layout (points) in;
layout (triangle_strip, max_vertices = 3) out;

// From Vertex shader
in VertexDataG {
  vec3 color;
} i[];

// To fragment shader
out VertexData {
  vec3 color;
} o;

void main() 
{    
    gl_Position = gl_in[0].gl_Position + vec4(0.01,0,0,0);
    o.color = i[0].color;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(-0.01,0,0,0);
    o.color = i[0].color;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(0,0.5,0,0);
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