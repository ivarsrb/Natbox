#include "grass.h"
#include <imgui/imgui.h>
#include "config.h"

using namespace engine;

namespace app::study::grass {
Grass::Grass(renderer::RenderDevice &render_device) {
    Init(render_device);
}

void Grass::Init(renderer::RenderDevice &render_device) {
    GrassEntity::Properties entity_descr;

    entity_descr.type = GrassEntity::Type::kGrass1;
    entity_descr.world_position = tp::Vec3(0.0);
    entity_descr.rotation = 0.0;
    entity_descr.scale = 1.0;
    entities_.emplace_back(render_device, entity_descr);

    entity_descr.type = GrassEntity::Type::kGrass2;
    entity_descr.world_position = tp::Vec3(0.0, 0.0, 3.0);
    entity_descr.rotation = 0.0;
    entity_descr.scale = 0.0;
   // entities_.emplace_back(render_device, entity_descr);

    // Shader
    std::string  vs_source = std::string(SHADER_PATH) + "grass.vert";
    std::string  gs_source = std::string(SHADER_PATH) + "grass.geom";
    std::string  ps_source = std::string(SHADER_PATH) + "grass.frag";
    std::vector<renderer::gl::Shader> shaders;
    shaders.push_back(render_device.CreateVertexShader(vs_source));
    shaders.push_back(render_device.CreateGeometryShader(gs_source));
    shaders.push_back(render_device.CreatePixelShader(ps_source));
    pipeline_ = std::make_unique<renderer::gl::Program>(render_device.CreatePipeline(shaders));
}

void Grass::Update(tp::Real dt, const Wind &wind) {
    for (auto &entity : entities_) {
        entity.Update(dt, wind);
    }
}

void Grass::Render(renderer::RenderDevice &render_device, renderer::gl::Buffer &uniform_buffer_scene, ShaderData &shader_data) {
    pipeline_->Use();
    for (auto &entity : entities_) {
        entity.Render(render_device, uniform_buffer_scene, shader_data);
    }
}

void Grass::RenderGui() {
    ImGui::Begin("Grass");
    
    ImGui::End();

    // TODO: make grass entity selectable in dropdown
    entities_.at(0).RenderGui();
}

}; // grass