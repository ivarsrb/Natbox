#include "entity.h"
#include <fstream> 
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imgui/imgui.h>
#include <nlohmann/json.hpp>
#include "config.h"
#include <engine/util/log.h>

using namespace engine;

namespace app::study::grass {
Entity::Entity(renderer::RenderDevice &render_device, const Properties &properties): properties_(properties) {
    Init(render_device);
}

void Entity::Init(renderer::RenderDevice &render_device) {
    // All models should face +z direction, thats how physics calculation are applied (first vertices and last vertices x should match)
    // Actual positioning, rotation and scaling  of a model are done in rendering phase not simulation
    // For simulation position - to get wind vector, rotation - to rotate wind (as if model is rotated), 
    //                scale - to scale parameters (weight etc)
    std::string model_file_name;
    switch (properties_.type) {
        case Type::kGrass1:
            model_file_name = "grass_1.json";
            break;
        case Type::kGrass2:
            model_file_name = "grass_2.json";
            break;
        case Type::kGrass3:
            model_file_name = "grass_3.json";
            break;
    }
    std::vector<Physics::Edge> edges = GetEdgesFromFile(model_file_name);
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

void Entity::Update(tp::Real dt, const Wind &wind) {
    tp::Vec3  wind_vec =  wind.GetVectorFromPosition(properties_.world_position);
    // Adjust wind vector accordin to rotation of entity
    wind_vec = util::math::RotateSafe(wind_vec, -properties_.rotation, util::math::kGroundNormal);
    //util::Log::Write("M_grass: wind vec ", glm::to_string(wind_vec));
    physics_->Update(dt, wind_vec);
}

void Entity::Render(renderer::RenderDevice &render_device, renderer::gl::Buffer &uniform_buffer_scene, ShaderData &shader_data) {
    tp::Mat4 world_mat(1.0);
    world_mat = glm::translate(world_mat, properties_.world_position);
    world_mat = glm::rotate(world_mat, properties_.rotation, util::math::kGroundNormal);
    shader_data.world_from_local = world_mat;
    /*
    tp::Mat4 translation_mat = glm::translate(properties_.world_position);
    tp::Mat4 rotation_mat = glm::rotate(properties_.rotation, util::math::kGroundNormal);
    shader_data.world_from_local = translation_mat * rotation_mat;
    */
    uniform_buffer_scene.SubData(offsetof(ShaderData, world_from_local), sizeof(ShaderData::world_from_local), &shader_data.world_from_local[0]);
    
    // Vertices update
    GetVerticesFromEdges(physics_->GetTransformedEdges(), 0);
    vertex_buffers_.at(0).SubData(0, vertices_.size() * sizeof(VertexType), vertices_.data());

    vertex_array_->Bind();
    render_device.DrawLineStrip(0, vertices_.size());
    vertex_array_->Unbind();
}

void Entity::RenderGui() {
    ImGui::Begin("Grass entity");
    if (ImGui::Button("Reset")) {
       physics_->Reset();
    }
    ImGui::End();
}

void Entity::GetVerticesFromEdges(const std::vector<Physics::Edge> &edges, uint32_t start_index) {
    for (const auto& edge : edges) {
        vertices_.at(start_index++).position = edge.position;
    }
}

// Inserts grass model data fropm JSON file into edge struture
std::vector<Physics::Edge> Entity::GetEdgesFromFile(std::string mddel_file_name) {
    std::vector<Physics::Edge> edges;
    std::ifstream model_file(std::string(MODELS_PATH) + mddel_file_name);
    if (!model_file.fail()) {
        nlohmann::json model_data = nlohmann::json::parse(model_file);
        for (const auto &model_edge : model_data["edges"]) {
            Physics::Edge edge;
            model_edge["position"][0].get_to(edge.position.x);
            model_edge["position"][1].get_to(edge.position.y);
            model_edge["position"][2].get_to(edge.position.z);
            model_edge["mass"].get_to(edge.mass);
            model_edge["width"].get_to(edge.width);
            // Infinity is represented by null in file
            if (model_edge["stiffness"].is_null()) {
                edge.stiffness = std::numeric_limits<float>::infinity();
            }
            else {
                model_edge["stiffness"].get_to(edge.stiffness);
            }
            edges.push_back(edge);
            //std::cout << edge.mass <<  " " << edge.width << " " << edge.stiffness << std::endl;
        }
        //std::cout << model_data.dump(4) << std::endl;
    } else {
        throw std::runtime_error("(Grass) could not open file " + mddel_file_name);
    }
    return edges;
}

}; // grass