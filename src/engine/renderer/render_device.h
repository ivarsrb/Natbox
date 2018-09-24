#pragma once
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "image.h"
#include "gl/shader.h"
#include "gl/program.h"
#include "gl/buffer.h"
#include "gl/vertex_array.h"
#include "gl/texture_2d.h"
#include "types.h"




// Rendering class that abstracts OpenGL calls and helps managing OpenGL resources and states
namespace engine::renderer {
class RenderDevice {
public:
    // Raster
    enum class RasterState {
        kNoCullSolid = 0,
        kBackCullSolid,
        kNoCullWire,
        kBackCullWire
    };
    // Depth stencil
    enum class DepthStencilState {
        // Depth and stencil disabled
        kDisabled = 0,
        // Only depth, no stencil
        kDepth,
        // Only stencil. no depth
        kStencil,
        // Depth and stencil enabled
        kDepthStencil
    };
    // What framebuffers to clear
    enum class ClearBuffer {
        kAll = 0,
        kColor,
        kColorDepth,
        kDepth,
        kStencil
    };
    // Locations of vertex array attributes in shader
    enum AttribLocation {
        kPosition = 0,
        kColor = 1,
        kNormal = 2,
        kTexCoord = 3
    };

    // Binding points of textures in shaders (shader binding should match these)
    enum TextureBindingPoint {
        // For ordinary texturing
        kDiffuse = 0,
        kDiffuse2 = 1,
        kDiffuse3 = 2,
        
        // To extract color values and use them as data 
        kColorMap = 10,

        // For sampilng height data
        kHeightmap = 20,
    };

    // Binding points of uniform buffers in shaders (shader binding should match these)
    enum UniformBufferBindingPoint {
        // For whole scene
        kScene = 0
    };

    // Used to monitor and store rendering statistics and states
    struct RenderStats {
        uint32_t num_drawcalls_per_frame;
        uint32_t num_primitives_per_frame;
        RasterState previous_raster_state;
    };

    RenderDevice();
    ~RenderDevice();
    RenderDevice(RenderDevice const&) = delete;
    RenderDevice operator=(RenderDevice const&) = delete;
    // Shaders
    gl::Shader CreateVertexShader(std::string source_file);
    gl::Shader CreatePixelShader(std::string source_file);
    gl::Shader CreateGeometryShader(std::string source_file);
    // TODO: when adding other shader types use ovearloading and new functions
    //gl::Program CreatePipeline(const gl::Shader &vertex_shader, const gl::Shader &pixel_shader);
    gl::Program CreatePipeline(const std::vector<gl::Shader> &shaders);

    gl::Buffer CreateUniformBuffer(uint32_t uniform_block_size, uint32_t binding_point);
    // Vertex
    gl::Buffer CreateVertexBuffer(const uint64_t size, const void *data, bool dynamic = false);
    gl::VertexArray CreateVertexArray(const std::vector<gl::Buffer> &vertex_buffers, const std::vector<types::VertexAttributeDescr> &vertex_attributes);
    void BindVertexArrayAttributes(gl::VertexArray& vertex_array, const std::vector<types::VertexAttributeDescr> &vertex_attributes, const uint32_t binding_index);
    // Index
    //gl::Buffer CreateIndexBuffer(const uint64_t size, const void *data);
    // Textures
    gl::Texture2D CreateTexture2D(const std::string texture_file, const Image::BitSize bit_size = Image::BitSize::k8bit, 
                                  const bool create_mipmaps = true, const uint32_t unpack_pixel_align = 4);
    // Weather to bind this texture to a texture unit, if is false, unit number is ignored
    //void SetTexture2D(gl::Texture2D &texture, const bool bind = false, const uint32_t texture_unit = 0);
    // Draw
    void Clear(const ClearBuffer clear = ClearBuffer::kAll);
    void DrawPoints(uint32_t offset, uint32_t count);
    void DrawLineStrip(uint32_t offset, uint32_t count);
    void DrawTriangles(uint32_t offset, uint32_t count);
    void DrawTrianglesIndexed32(uint64_t offset, uint32_t count);
    void DrawTriangleStrip(uint32_t offset, uint32_t count);
    void DrawTriangleStripIndexed32(uint64_t offset, uint32_t count);

    void SetViewport(glm::ivec2 lower_left, glm::ivec2 size);
    // States
    void SetRasterState(const RasterState state);
    void SetDepthStencilState(const DepthStencilState state); 
    // Actual states and statistics
    RenderDevice::RenderStats render_stats;
private:
    void ClearRenderStats();
};
}; // renderer