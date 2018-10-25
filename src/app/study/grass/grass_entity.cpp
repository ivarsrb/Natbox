#include "grass_entity.h"
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>

using namespace engine;

namespace app::study::grass {
GrassEntity::GrassEntity(renderer::RenderDevice &render_device, const Properties &properties): properties_(properties) {
    Init(render_device);
}

void GrassEntity::Init(renderer::RenderDevice &render_device) {
    // By definition edge count is always 4
    std::vector<GrassPhysics::EdgeProperties> edges(4);
    // TODO: should take edge data from file
    if (properties_.type == Type::kGrass1) {
        edges.at(0).position = tp::Vec3(0.0, 0.0, 0.0);
        edges.at(0).stiffness = std::numeric_limits<float>::infinity();
        edges.at(0).width = 0.0;
        edges.at(0).mass = 0.0;

        edges.at(1).position = tp::Vec3(0.0, 0.7, 0.7);
        edges.at(1).stiffness = 5.0;
        edges.at(1).width = 0.1;
        edges.at(1).mass = 0.0033;

        edges.at(2).position = tp::Vec3(0.0, 1.0, 1.5);
        edges.at(2).stiffness = 3.0;
        edges.at(2).width = 0.1;
        edges.at(2).mass = 0.0033;
        
        edges.at(3).position = tp::Vec3(0.0, 1.0, 2.5);
        edges.at(3).stiffness = 2.0;
        edges.at(3).width = 0.1;
        edges.at(3).mass = 0.0033;
    }
    if (properties_.type == Type::kGrass2) {
        edges.at(0).position = tp::Vec3(0.0, 0.0, 0.0);
        edges.at(0).stiffness = std::numeric_limits<float>::infinity();
        edges.at(0).width = 0.0;
        edges.at(0).mass = 0.0;

        edges.at(1).position = tp::Vec3(0.0, 0.3, 0.);
        edges.at(1).stiffness = 5.0;
        edges.at(1).width = 0.1;
        edges.at(1).mass = 0.0033;

        edges.at(2).position = tp::Vec3(0.0, 0.6, 0);
        edges.at(2).stiffness = 3.0;
        edges.at(2).width = 0.1;
        edges.at(2).mass = 0.0033;
        
        edges.at(3).position = tp::Vec3(0.0, 1.0, 0.0);
        edges.at(3).stiffness = 2.0;
        edges.at(3).width = 0.1;
        edges.at(3).mass = 0.0033;
    }

    // TODO: rotation and scaling here....

    // Physics
    physics_.emplace(edges);
    // Rendering
    vertices_.resize(edges.size());
    // Dynamic vertex buffer with currently no data
    vertex_buffers_.push_back(render_device.CreateVertexBuffer(vertices_.size() * sizeof(VertexType), 0, true));
    // Vertex array
    std::vector<renderer::types::VertexAttributeDescr> vertex_attributes(1);
    vertex_attributes.at(0).attribute_index = renderer::RenderDevice::AttribLocation::kPosition;
    vertex_attributes.at(0).size = sizeof(VertexType) / sizeof(float); // 3  // (start_of_next_attrib - start_of_this_attrib) / size_of_single_element_type
    vertex_attributes.at(0).type = renderer::types::RenderDataTypes::kFloat;
    vertex_attributes.at(0).offset = 0;
    vertex_attributes.at(0).stride = sizeof(VertexType); // always the same for all atributes within vertex array
    vertex_array_.emplace(render_device.CreateVertexArray(vertex_buffers_, vertex_attributes));
}

void GrassEntity::Update(tp::Real dt, const Wind &wind) {
    physics_->Update(dt, wind.VectorInPosition(properties_.world_position));
}

void GrassEntity::Render(renderer::RenderDevice &render_device, renderer::gl::Buffer &uniform_buffer_scene, ShaderData &shader_data) {
    shader_data.world_from_local = glm::translate(glm::tmat4x4<tp::Real>(1.0), properties_.world_position);
    uniform_buffer_scene.SubData(offsetof(ShaderData, world_from_local), sizeof(ShaderData::world_from_local), &shader_data.world_from_local[0]);
    
    // Vertices update
    EdgesToVertices(physics_->CurrentEdges(), 0);
    vertex_buffers_.at(0).SubData(0, vertices_.size() * sizeof(VertexType), vertices_.data());

    vertex_array_->Bind();
    render_device.DrawLineStrip(0, vertices_.size());
    vertex_array_->Unbind();
}

void GrassEntity::RenderGui() {
    ImGui::Begin("Grass entity");
    if (ImGui::Button("Reset")) {
       physics_->Reset();
    }
    ImGui::End();
}

void GrassEntity::EdgesToVertices(const std::vector<GrassPhysics::EdgeProperties> &edges, uint32_t start_index) {
    for (const auto& edge : edges) {
        vertices_.at(start_index++).position = edge.position;
    }
}

}; // grass