#include "grass_renderer.h"
#include <cstddef>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <engine/util/math.h>
#include "config.h"

namespace app::study::grass {
GrassRenderer::GrassRenderer(engine::renderer::RenderDevice &render_device, const GrassCollection &collection) {
    vertices_.resize(collection.GetTotalVertexCount());
    // Dynamic vertex buffer with currently no data
    vertex_buffer_.emplace(render_device.CreateVertexBuffer(vertices_.size() * sizeof(VertexTypeGrass), 0, true));
    // Vertex array
    std::vector<engine::renderer::types::VertexAttributeDescr> vertex_attributes(AttribLocation::kSizeMeasure);
    // Position
    vertex_attributes.at(0).attribute_index = AttribLocation::kPosition;
    vertex_attributes.at(0).size = sizeof(VertexTypeGrass::position) / sizeof(float); // 3  // (start_of_next_attrib - start_of_this_attrib) / size_of_single_element_type
    vertex_attributes.at(0).type = engine::renderer::types::RenderDataTypes::kFloat;
    vertex_attributes.at(0).offset = 0;
    vertex_attributes.at(0).stride = sizeof(VertexTypeGrass); // always the same for all atributes within vertex array (only first entry used, that's why no other has it)
    // Edge vector (from this position to next edge)
    vertex_attributes.at(1).attribute_index = AttribLocation::kEdge;
    vertex_attributes.at(1).size = sizeof(VertexTypeGrass::edge) / sizeof(float); // 3  // (start_of_next_attrib - start_of_this_attrib) / size_of_single_element_type
    vertex_attributes.at(1).type = engine::renderer::types::RenderDataTypes::kFloat;
    vertex_attributes.at(1).offset = offsetof(VertexTypeGrass, edge);
    // Normal
    vertex_attributes.at(2).attribute_index = AttribLocation::kNormal;
    vertex_attributes.at(2).size = sizeof(VertexTypeGrass::normal) / sizeof(float); // 3  // (start_of_next_attrib - start_of_this_attrib) / size_of_single_element_type
    vertex_attributes.at(2).type = engine::renderer::types::RenderDataTypes::kFloat;
    vertex_attributes.at(2).offset = offsetof(VertexTypeGrass, normal);
    // Side
    vertex_attributes.at(3).attribute_index = AttribLocation::kSide;
    vertex_attributes.at(3).size = sizeof(VertexTypeGrass::side) / sizeof(float); // 3  // (start_of_next_attrib - start_of_this_attrib) / size_of_single_element_type
    vertex_attributes.at(3).type = engine::renderer::types::RenderDataTypes::kFloat;
    vertex_attributes.at(3).offset = offsetof(VertexTypeGrass, side);
    vertex_array_.emplace(render_device.CreateVertexArray(*vertex_buffer_, vertex_attributes));
    // Textures
    std::string tex_file_name = "grass_blade.png";
    diffuse_map_.emplace(render_device.CreateTexture2D(TEXTURE_PATH + tex_file_name, engine::renderer::Image::BitSize::k8bit, false));
    // Shaders
/*
    std::string  vs_source = std::string(SHADER_PATH) + "grass.vert";
    std::string  gs_source = std::string(SHADER_PATH) + "grass.geom";
    std::string  tesc_source = std::string(SHADER_PATH) + "grass.tesc";
    std::string  tese_source = std::string(SHADER_PATH) + "grass.tese";
    std::string  ps_source = std::string(SHADER_PATH) + "grass.frag";
*/   
    std::string  vs_source = std::string(SHADER_PATH) + "grass_attribs.vert";
    std::string  gs_source = std::string(SHADER_PATH) + "grass_attribs.geom";
    std::string  tesc_source = std::string(SHADER_PATH) + "grass_attribs.tesc";
    std::string  tese_source = std::string(SHADER_PATH) + "grass_attribs.tese";
    std::string  ps_source = std::string(SHADER_PATH) + "grass_attribs.frag";

    std::vector<engine::renderer::gl::Shader> shaders;
    shaders.push_back(render_device.CreateVertexShader(vs_source));
    shaders.push_back(render_device.CreateGeometryShader(gs_source));
    shaders.push_back(render_device.CreateTessallationControlShader(tesc_source));
    shaders.push_back(render_device.CreateTessallationEvaluationShader(tese_source));
    shaders.push_back(render_device.CreatePixelShader(ps_source));
    pipeline_ .emplace(render_device.CreatePipeline(shaders));

}

void GrassRenderer::Render(engine::renderer::RenderDevice &render_device, const GrassCollection &collection, engine::renderer::gl::Buffer &uniform_buffer, 
                           UniformData &uniform_data) {
    // Update whole vertex buffer
    UpdateVertexBuffer(collection);
    // Rendering
    pipeline_->Use();
    vertex_array_->Bind();
    diffuse_map_->BindTextureUnit(engine::renderer::RenderDevice::TextureBindingPoint::kDiffuse);
    uint32_t entity_number = 0;
    for (const auto &entity : collection.collection) {
        engine::tp::Mat4 world_mat(1.0);
        world_mat = glm::translate(world_mat, entity.GetWorldPosition());
        world_mat = glm::rotate(world_mat, entity.GetOrientation(), engine::util::math::kGroundNormal);
        uniform_data.world_from_local = world_mat;
        uniform_buffer.SubData(offsetof(UniformData, world_from_local), sizeof(UniformData::world_from_local), &uniform_data.world_from_local[0]);
        uint32_t vertices_per_patch = entity.GetEdges().size();
        uint32_t start_vertex = entity_number * vertices_per_patch;
        render_device.DrawPatches(start_vertex, vertices_per_patch, vertices_per_patch);
        ++entity_number;
    }
    vertex_array_->Unbind();
}

// Update whole vertex buffer 
void GrassRenderer::UpdateVertexBuffer(const GrassCollection &collection) {
    for (uint32_t i = 0; i < collection.collection.size(); ++i) {
        const auto &entity = collection.collection.at(i); 
        auto edges = entity.GetEdges();
        // TO:O: this is wrong, it is dangerous to assume all grass entities have 4 vertices
        // start index in buffer and vertices per patch should be stored somewhere in grass entity or model
        uint32_t vertices_per_patch = edges.size();
        uint32_t start_vertex = i * vertices_per_patch;
        for (uint32_t i = 0; i < edges.size(); ++i) {
            vertices_.at(start_vertex + i).position = edges.at(i).position;
            vertices_.at(start_vertex + i).edge = edges.at(i).edge_vec;
            vertices_.at(start_vertex + i).normal = edges.at(i).normal_vec;
            vertices_.at(start_vertex + i).side = edges.at(i).side_vec;
        }
    }
    // Update whole grass collection buffer at once
    vertex_buffer_->SubData(0, vertices_.size() * sizeof(VertexTypeGrass), vertices_.data());
}

void GrassRenderer::RenderGUI(GrassCollection &collection) {
    ImGui::Begin("Grass collection");
    if (ImGui::Button("Reset")) {
       for (auto &entity : collection.collection) {
           entity.Reset();
       }
    }
    ImGui::End();
}
}; // grass