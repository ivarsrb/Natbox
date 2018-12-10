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
// Ale members are static
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

    // Shaders
    static gl::Shader CreateVertexShader(std::string source_file);
    static gl::Shader CreatePixelShader(std::string source_file);
    static gl::Shader CreateGeometryShader(std::string source_file);
    static gl::Shader CreateTessallationControlShader(std::string source_file);
    static gl::Shader CreateTessallationEvaluationShader(std::string source_file);
    static gl::Program CreatePipeline(const std::vector<gl::Shader> &shaders);
    static gl::Buffer CreateUniformBuffer(uint32_t uniform_block_size, uint32_t binding_point);
    // Vertex
    static gl::Buffer CreateVertexBuffer(const uint64_t size, const void *data, bool dynamic = false);
    static gl::VertexArray CreateVertexArray(const gl::Buffer &vertex_buffer, const std::vector<types::VertexAttributeDescr> &vertex_attributes);
    //gl::VertexArray CreateVertexArray(const std::vector<gl::Buffer> &vertex_buffers, const std::vector<types::VertexAttributeDescr> &vertex_attributes);
    // Index
    //gl::Buffer CreateIndexBuffer(const uint64_t size, const void *data);
    // Textures
    static gl::Texture2D CreateTexture2D(const std::string texture_file, const Image::BitSize bit_size = Image::BitSize::k8bit, 
                                  const bool create_mipmaps = true, const uint32_t unpack_pixel_align = 4);
    // Weather to bind this texture to a texture unit, if is false, unit number is ignored
    //void SetTexture2D(gl::Texture2D &texture, const bool bind = false, const uint32_t texture_unit = 0);
    // Draw
    static void Clear(const ClearBuffer clear = ClearBuffer::kAll);
    static void DrawPatches(uint32_t offset, uint32_t count, uint32_t vert_per_patch);
    static void DrawPoints(uint32_t offset, uint32_t count);
    static void DrawLineStrip(uint32_t offset, uint32_t count);
    static void DrawTriangles(uint32_t offset, uint32_t count);
    static void DrawTrianglesIndexed32(uint64_t offset, uint32_t count);
    static void DrawTriangleStrip(uint32_t offset, uint32_t count);
    static void DrawTriangleStripIndexed32(uint64_t offset, uint32_t count);

    static void SetViewport(glm::ivec2 lower_left, glm::ivec2 size);
    // States
    static void SetRasterState(const RasterState state);
    static void SetDepthStencilState(const DepthStencilState state); 
    // Actual states and statistics
    static RenderStats render_stats;
private:
    static void BindVertexArrayAttributes(gl::VertexArray& vertex_array, const std::vector<types::VertexAttributeDescr> &vertex_attributes, const uint32_t binding_index);
    static void ClearRenderStats();
};
}; // renderer