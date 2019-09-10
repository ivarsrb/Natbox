#include "render_device.h"
#include <exception>

namespace engine::renderer {
// Definition of static variables
RenderDevice::RenderStats RenderDevice::render_stats;

gl::Shader RenderDevice::CreateVertexShader(std::string source_file) {
    gl::Shader vertex_shader(gl::Shader::ShaderType::kVertexShader);
    vertex_shader.LoadSource(source_file);
    vertex_shader.Compile();
    return vertex_shader;
}

gl::Shader RenderDevice::CreatePixelShader(std::string source_file) {
    gl::Shader vertex_shader(gl::Shader::ShaderType::kFragmentShader);
    vertex_shader.LoadSource(source_file);
    vertex_shader.Compile();
    return vertex_shader;
}

gl::Shader RenderDevice::CreateGeometryShader(std::string source_file) {
    gl::Shader geometry_shader(gl::Shader::ShaderType::kGeometryShader);
    geometry_shader.LoadSource(source_file);
    geometry_shader.Compile();
    return geometry_shader;
}

gl::Shader RenderDevice::CreateTessallationControlShader(std::string source_file) {
    gl::Shader tessallation_shader(gl::Shader::ShaderType::kTessControlShader);
    tessallation_shader.LoadSource(source_file);
    tessallation_shader.Compile();
    return tessallation_shader;
}

gl::Shader RenderDevice::CreateTessallationEvaluationShader(std::string source_file) {
    gl::Shader tessallation_shader(gl::Shader::ShaderType::kTessEvaluationShader);
    tessallation_shader.LoadSource(source_file);
    tessallation_shader.Compile();
    return tessallation_shader;
}

gl::Program RenderDevice::CreatePipeline(const std::vector<gl::Shader> &shaders) {
    gl::Program pipeline;
    if (shaders.size() > 6 || shaders.size() < 2) {
        throw std::runtime_error("(RenderDevice) incorrect shader count passed to pipeline");
    }
    for (auto &shader : shaders) {
        pipeline.AttachShader(shader);
    }
    pipeline.Link();
    // Detach so shaders get actually deleted when shader destructor is called
    for (auto &shader : shaders) {
        pipeline.DetachShader(shader);
    }
    return pipeline;
}

// Update uniform buffer with SubData() call
gl::Buffer RenderDevice::CreateUniformBuffer(uint32_t uniform_block_size, uint32_t binding_point) {
    gl::Buffer uniform_buffer;
    // Don't write data upon creation
    uniform_buffer.Storage(uniform_block_size, NULL, GL_DYNAMIC_STORAGE_BIT);
    // Bind this buffer to binding point (corresponds to 'binding' in shader)
    glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, uniform_buffer.GetName());
    return uniform_buffer;
}

gl::Buffer RenderDevice::CreateVertexBuffer( uint64_t size, const void * data, bool dynamic) {
    gl::Buffer vertex_buffer;
    GLbitfield flag = (dynamic ? GL_DYNAMIC_STORAGE_BIT : 0);
    vertex_buffer.Storage(size, data, flag);
    return vertex_buffer;
}

// VB Binding point in 0
gl::VertexArray RenderDevice::CreateVertexArray(const gl::Buffer &vertex_buffer, const std::vector<types::VertexAttributeDescr> &vertex_attributes) {
    gl::VertexArray vertex_array;

    GLuint binding_index = 0;
    // VBO
    vertex_array.VertexBuffer(binding_index, vertex_buffer, 0, vertex_attributes.at(0).stride);

    // Attributes
    for (size_t i = 0; i < vertex_attributes.size(); i++) {
        // Usually index comes from vertex shader
        GLint attribute_index = vertex_attributes.at(i).attribute_index;
        // Enable attributes
        vertex_array.EnableAttrib(attribute_index);
        // Set up attribute formats
        vertex_array.AttribFormat(attribute_index, vertex_attributes.at(i).size, types::ToApiType(vertex_attributes.at(i).type), GL_FALSE, vertex_attributes.at(i).offset);
        // To switch betwean buffers, use AttribBinding function by providing all attribute indexes and desired buffer binding index 
        // By default set all attributes to fech data from buffer bound to 0 slot
        // vertex_array.AttribBinding(attribute_index, 0);
    }
    // set all attributes to fech data from buffer bound to...
    BindVertexArrayAttributes(vertex_array, vertex_attributes, binding_index);
    return vertex_array;
}

// TODO: no point having to pass array if only one point is bound
// Binding index for each vertex buffer is index in buffers vector
// Index buffer should be bound separatly with member function of VAO
/*
gl::VertexArray RenderDevice::CreateVertexArray(const std::vector<gl::Buffer>& vertex_buffers, const std::vector<types::VertexAttributeDescr> &vertex_attributes) {
    gl::VertexArray vertex_array;

    // VBO
    for (uint32_t i = 0; i < vertex_buffers.size(); i++) {
        // Bind vertex buffer
        GLuint binding_index = i;
        // For stride take first attribute because stride should be the same for all attributes bceause it's meant for vbo binding here also legacy reasons
        vertex_array.VertexBuffer(binding_index, vertex_buffers.at(i), 0, vertex_attributes.at(0).stride);
    }

    // Attributes
    for (size_t i = 0; i < vertex_attributes.size(); i++) {
        // Usually index comes from vertex shader
        GLint attribute_index = vertex_attributes.at(i).attribute_index;
        // Enable attributes
        vertex_array.EnableAttrib(attribute_index);
        // Set up attribute formats
        vertex_array.AttribFormat(attribute_index, vertex_attributes.at(i).size, types::ToApiType(vertex_attributes.at(i).type), GL_FALSE, vertex_attributes.at(i).offset);
        // To switch betwean buffers, use AttribBinding function by providing all attribute indexes and desired buffer binding index 
        // By default set all attributes to fech data from buffer bound to 0 slot
        // vertex_array.AttribBinding(attribute_index, 0);
    }
    // By default set all attributes to fech data from buffer bound to 0 slot
    BindVertexArrayAttributes(vertex_array, vertex_attributes, 0);
    return vertex_array;
}*/

// Bind all attributes from given array to VBO binding index
void RenderDevice::BindVertexArrayAttributes(gl::VertexArray& vertex_array, const std::vector<types::VertexAttributeDescr> &vertex_attributes,  uint32_t binding_index) {
    for (size_t i = 0; i < vertex_attributes.size(); i++) {
        vertex_array.AttribBinding(vertex_attributes.at(i).attribute_index, binding_index);
    }
}


// Load texture from file, create storage upload data to storage, create mipmaps (optional), set parameters
gl::Texture2D RenderDevice::CreateTexture2D(const std::string& texture_file,   Image::BitSize bit_size, 
                                             bool create_mipmaps,  uint32_t unpack_pixel_align) {
    gl::Texture2D texture;
    GLenum format, sizedformat;
    // Load external texture into cpu memeory
    Image image(texture_file, bit_size);
    uint32_t channel_count = image.GetNumChanels();

    // Check if texture size is allowed
    int max_size = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_size);
    if (image.GetSize().x > max_size || image.GetSize().y > max_size) {
        throw std::runtime_error("(RenderDevice) loaded texture size is not allowed");
    }

    // Some data in texture storage is aligned to multiple of 4 bytes, in a lot of cases SubImage will 
    // crash because texture_wodth * data bytes is no divisible by 4, glPixelStorei(GL_UNPACK_ALIGNMENT, n) can be used to set alignemnt
    // for 16-bit single-channel texture it should be 2, by default it is 4
    glPixelStorei(GL_UNPACK_ALIGNMENT, unpack_pixel_align);

    if (bit_size == Image::BitSize::k8bit) {
        switch (channel_count)
        {
        case 1:
            format = GL_RED;
            sizedformat = GL_R8;
            break;
        case 2:
            format = GL_RG;
            sizedformat = GL_RG8;
            break;
        case 3:
            format = GL_RGB;
            sizedformat = GL_RGB8;
            break;
        case 4:
            format = GL_RGBA;
            sizedformat = GL_RGBA8;
            break;
        default:
            break;
        }

        uint32_t mipmap_levels = create_mipmaps ? image.GetMipMapLevels() : 1;
        texture.Storage(mipmap_levels, sizedformat, image.GetSize().x, image.GetSize().y);
        texture.SubImage(0, 0, 0, image.GetSize().x, image.GetSize().y, format, GL_UNSIGNED_BYTE, image.GetImage8());
    }
    else if (bit_size == Image::BitSize::k16bit) {
        switch (channel_count)
        {
        case 1:
            format = GL_RED;
            sizedformat = GL_R16;
            break;
        case 2:
            format = GL_RG;
            sizedformat = GL_RG16;
            break;
        case 3:
            format = GL_RGB;
            sizedformat = GL_RGB16;
            break;
        case 4:
            format = GL_RGBA;
            sizedformat = GL_RGBA16;
            break;
        default:
            break;
        }

        uint32_t mipmap_levels = create_mipmaps ? image.GetMipMapLevels() : 1;
        texture.Storage(mipmap_levels, sizedformat, image.GetSize().x, image.GetSize().y);
        texture.SubImage(0, 0, 0, image.GetSize().x, image.GetSize().y, format, GL_UNSIGNED_SHORT, image.GetImage16());
    }

    if (create_mipmaps) {
        texture.GenerateMipmap();
    }

    // Default parameters that could be often used, override if neceserry
    texture.Parameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    texture.Parameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (create_mipmaps) {
        texture.Parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
    else {
        texture.Parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    texture.Parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set back to default
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    return texture;
}


// Clear color, depth or stencil buffers of a currently bound framebuffer
void RenderDevice::Clear( ClearBuffer clear ) {
    // Assuming clear always gets called at the top of the scene
    ClearRenderStats();
    // Draw buffer to clear
    GLint draw_buffer = 0;
    static const GLfloat one = 1.0f; // TODO think how to go without static
    if (clear == ClearBuffer::kAll || clear == ClearBuffer::kColor || clear == ClearBuffer::kColorDepth) {
        glm::vec4 clear_color = glm::vec4(0.15f, 0.15f, 0.15f, 1.0f);
        glClearBufferfv(GL_COLOR, draw_buffer, &clear_color[0]);
    }
    if (clear == ClearBuffer::kAll || clear == ClearBuffer::kDepth || clear == ClearBuffer::kColorDepth) {
        glClearBufferfv(GL_DEPTH, draw_buffer, &one);
    }
    if (clear == ClearBuffer::kAll || clear == ClearBuffer::kStencil) {
        glClearBufferfv(GL_STENCIL, draw_buffer, &one);
    }
}

// offset - index of first elemnt in array, count - number of elements to draw
// vert_per_patch - how many vertices are per patch
void RenderDevice::DrawPatches(uint32_t offset, uint32_t count, uint32_t vert_per_patch) {
    assert(vert_per_patch > 0);
    glPatchParameteri(GL_PATCH_VERTICES, vert_per_patch);
    glDrawArrays(GL_PATCHES, offset, count);
    // Stats
    render_stats.num_drawcalls_per_frame++;
    render_stats.num_primitives_per_frame += count / vert_per_patch;
}

// offset - index of first elemnt in array, count - number of elements to draw
void RenderDevice::DrawPoints(uint32_t offset, uint32_t count) {
    glDrawArrays(GL_POINTS, offset, count);
    // Stats
    render_stats.num_drawcalls_per_frame++;
    render_stats.num_primitives_per_frame += count;
}

// offset - index of first elemnt in array, count - number of indices
void RenderDevice::DrawLineStrip(uint32_t offset, uint32_t count) {
    glDrawArrays(GL_LINE_STRIP, offset, count);
    // Stats
    render_stats.num_drawcalls_per_frame++;
    render_stats.num_primitives_per_frame += count - 1;
}


// offset - index of first elemnt in array, count - number of elements to draw
void RenderDevice::DrawTriangles(uint32_t offset, uint32_t count) {
    glDrawArrays(GL_TRIANGLES, offset, count);
    // Stats
    render_stats.num_drawcalls_per_frame++;
    render_stats.num_primitives_per_frame += count / 3;
}

// Use when index buffer is bound to vao
void RenderDevice::DrawTrianglesIndexed32(uint64_t offset, uint32_t count) {
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, reinterpret_cast<const void *>(offset));
    // Stats
    render_stats.num_drawcalls_per_frame++;
    render_stats.num_primitives_per_frame += count / 3;
}

void RenderDevice::DrawTriangleStrip(uint32_t offset, uint32_t count) {
    glDrawArrays(GL_TRIANGLE_STRIP, offset, count);
    // Stats
    render_stats.num_drawcalls_per_frame++;
    render_stats.num_primitives_per_frame += count - 2;
}

void RenderDevice::DrawTriangleStripIndexed32(uint64_t offset, uint32_t count) {
    glDrawElements(GL_TRIANGLE_STRIP, count, GL_UNSIGNED_INT, reinterpret_cast<const void *>(offset));
    // Stats
    render_stats.num_drawcalls_per_frame++;
    render_stats.num_primitives_per_frame += count - 2;
}

void RenderDevice::SetViewport(glm::ivec2 lower_left, glm::ivec2 size) {
    glViewport(lower_left.x, lower_left.y, size.x, size.y);
}

// OPTI: check previous values to not reset
void RenderDevice::SetRasterState( RasterState state) {
    // default state
    bool enabled_culling = false;
    GLenum front_face = GL_CCW;
    GLenum cull_face = GL_BACK;
    GLenum polygon_mode = GL_FILL;

    switch (state) {
    case RasterState::kNoCullSolid:
        // Default
        break;
    case RasterState::kBackCullSolid:
        enabled_culling = true;
        front_face = GL_CCW;
        cull_face = GL_BACK; 
        polygon_mode = GL_FILL;
        break;
    case RasterState::kNoCullWire:
        enabled_culling = false;
        front_face = GL_CCW;
        cull_face = GL_BACK;
        polygon_mode = GL_LINE;
        break;
    case RasterState::kBackCullWire:
        enabled_culling = true;
        front_face = GL_CCW;
        cull_face = GL_BACK;
        polygon_mode = GL_LINE;
        break;
    default:
        break;
    }

    if(enabled_culling)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
    glFrontFace(front_face);
    glCullFace(cull_face);
    glPolygonMode(GL_FRONT_AND_BACK, polygon_mode);

    // Stats
    render_stats.previous_raster_state = state;
}

// TODO: state names are likely to get ugly, need other way
void RenderDevice::SetDepthStencilState( DepthStencilState state) {
    // default state
    bool depth_enabled = false;
    bool stencil_enabled = false;

    switch (state) {
    case DepthStencilState::kDisabled:
        //default
        break;
    case DepthStencilState::kDepth:
        depth_enabled = true;
        stencil_enabled = false;
        break;
    case DepthStencilState::kStencil:
        depth_enabled = true;
        stencil_enabled = false;
        break;
    case DepthStencilState::kDepthStencil:
        depth_enabled = true;
        stencil_enabled = true;
        break;
    default:
        break;
    }

    if (depth_enabled )
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    if (stencil_enabled)
        glEnable(GL_STENCIL_TEST);
    else
        glDisable(GL_STENCIL_TEST);
    /*
    glDepthFunc();
    glDepthMask();
    glDepthRange();

    // front face
    glStencilFuncSeparate(GL_FRONT,);
    glStencilMaskSeparate(GL_FRONT, );
    glStencilOpSeparate(GL_FRONT, );

    // back face
    glStencilFuncSeparate(GL_BACK,);
    glStencilMaskSeparate(GL_BACK, );
    glStencilOpSeparate(GL_BACK, );
    */
}

void RenderDevice::ClearRenderStats() {
    render_stats.num_drawcalls_per_frame = 0;
    render_stats.num_primitives_per_frame = 0;
}

}; // renderer