#include "plant.h"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <engine/renderer/types.h>

using namespace engine;

namespace app::study::grass {
Plant::Plant(renderer::RenderDevice &render_device ) {
    Init(render_device);
}

void Plant::Init(renderer::RenderDevice &render_device) {
    // Vertices
    std::vector<renderer::types::FPos> vertices;
    for(size_t i = 0; i < 3; i++) {
        vertices.push_back({ glm::vec3(0.0 + i*0.1, 0.0, 0.0) });
    }
    vertex_count_ = vertices.size();
    // Vertex buffer
    uint32_t vertex_type_size = sizeof(decltype(vertices)::value_type);
    vertex_buffers_.push_back(render_device.CreateVertexBuffer(vertex_count_ * vertex_type_size, vertices.data()));

    // Vertex array
    std::vector<renderer::types::VertexAttributeDescr> vertex_attributes(1);
    vertex_attributes.at(0).attribute_index = renderer::RenderDevice::AttribLocation::kPosition;
    vertex_attributes.at(0).size = vertex_type_size / sizeof(float); // 3  // (start_of_next_attrib - start_of_this_attrib) / size_of_single_element_type
    vertex_attributes.at(0).type = renderer::types::RenderDataTypes::kFloat;
    vertex_attributes.at(0).offset = 0;
    vertex_attributes.at(0).stride = vertex_type_size; // always the same for all atributes within vertex array

    vertex_array_ = std::make_unique<renderer::gl::VertexArray>(render_device.CreateVertexArray(vertex_buffers_, vertex_attributes));
}


void Plant::Render(renderer::RenderDevice &render_device, renderer::gl::Buffer &uniform_buffer_scene, ShaderData &shader_data, 
                   glm::tvec3<engine::tp::Real> position) {
    shader_data.world_from_local = glm::translate(glm::tmat4x4<tp::Real>(1.0), position);
    uniform_buffer_scene.SubData(offsetof(ShaderData, world_from_local), sizeof(ShaderData::world_from_local), &shader_data.world_from_local[0]);
    
    vertex_array_->Bind();
    render_device.DrawPoints(0, vertex_count_);
    vertex_array_->Unbind();
 }

void Plant::RenderGui() {
    ImGui::Begin("Plant");
    ImGui::End();
}
};