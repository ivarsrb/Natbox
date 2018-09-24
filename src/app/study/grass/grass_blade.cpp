#include "grass_blade.h"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <engine/renderer/types.h>

using namespace engine;

namespace app::study::grass {
GrassBlade::GrassBlade(renderer::RenderDevice &render_device ) {
    Init(render_device);
}

void GrassBlade::Init(renderer::RenderDevice &render_device) {
    std::vector<glm::tvec3<tp::Real>> edges;
    edges.push_back(glm::vec3(0.0, 0.0, 0.0));
    edges.push_back(glm::vec3(0.0, 1.0, 0.0));
    edges.push_back(glm::vec3(0.5, 1.5, 0.0));
    edges.push_back(glm::vec3(1.5, 1.5, 0.0));

    blade_ = std::make_unique<Plant>(edges);
    // Vertices set tu dummy
    vertices_.resize(edges.size());
    
    // Dynamic vertex buffer
    vertex_buffers_.push_back(render_device.CreateVertexBuffer(vertices_.size() * sizeof(VertexType), 0, true));

    // Vertex array
    std::vector<renderer::types::VertexAttributeDescr> vertex_attributes(1);
    vertex_attributes.at(0).attribute_index = renderer::RenderDevice::AttribLocation::kPosition;
    vertex_attributes.at(0).size = sizeof(VertexType) / sizeof(float); // 3  // (start_of_next_attrib - start_of_this_attrib) / size_of_single_element_type
    vertex_attributes.at(0).type = renderer::types::RenderDataTypes::kFloat;
    vertex_attributes.at(0).offset = 0;
    vertex_attributes.at(0).stride = sizeof(VertexType); // always the same for all atributes within vertex array

    vertex_array_ = std::make_unique<renderer::gl::VertexArray>(render_device.CreateVertexArray(vertex_buffers_, vertex_attributes));
}

void GrassBlade::Update(engine::tp::Real dt) {
    blade_->Update(dt);
}

void GrassBlade::Render(renderer::RenderDevice &render_device, renderer::gl::Buffer &uniform_buffer_scene, ShaderData &shader_data, 
                   glm::tvec3<tp::Real> position) {
    shader_data.world_from_local = glm::translate(glm::tmat4x4<tp::Real>(1.0), position);
    uniform_buffer_scene.SubData(offsetof(ShaderData, world_from_local), sizeof(ShaderData::world_from_local), &shader_data.world_from_local[0]);
    
    // Vertices update
    blade_->FillVertices(vertices_, 0);
    vertex_buffers_.at(0).SubData(0, vertices_.size() * sizeof(VertexType), vertices_.data());

    vertex_array_->Bind();
    render_device.DrawLineStrip(0, vertices_.size());
    vertex_array_->Unbind();
 }

void GrassBlade::RenderGui() {
    ImGui::Begin("Grass blade");
    ImGui::End();
}
};