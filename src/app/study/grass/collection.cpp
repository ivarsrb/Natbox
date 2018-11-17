#include "collection.h"
#include <imgui/imgui.h>
#include "config.h"

using namespace engine;

// TODO: grass should be of different types physically, for example current morions apply to grasses
//  thar are bent, but for grass types that go straight up and dont swing or twist use only bending
namespace app::study::grass {
Collection::Collection(renderer::RenderDevice &render_device) {
    Init(render_device);
}

void Collection::Init(renderer::RenderDevice &render_device) {
    Entity::Properties entity_descr;
    entity_descr.type = Entity::Type::kGrass1;
    entity_descr.world_position = tp::Vec3(0.0);
    entity_descr.rotation = 0.0;
    entity_descr.scale = 1.0;
    //entities_.emplace_back(render_device, entity_descr);

    entity_descr.type = Entity::Type::kGrass2;
    entity_descr.world_position = tp::Vec3(0.0, 0.0, 0.0);
    entity_descr.rotation = 0.0;
    entity_descr.scale = 0.0;
    //entities_.emplace_back(render_device, entity_descr);

    entity_descr.type = Entity::Type::kGrass3;
    entity_descr.world_position = tp::Vec3(0.0, 0.0, 0.0);
    entity_descr.rotation = M_PI_2;
    entity_descr.scale = 0.0;
    entities_.emplace_back(render_device, entity_descr);

    entity_descr.type = Entity::Type::kGrass1;
    entity_descr.world_position = tp::Vec3(0.0, 0.0, 0.0);
    entity_descr.rotation = M_PI+0.1;
    entity_descr.scale = 0.0;
    //entities_.emplace_back(render_device, entity_descr);

    entity_descr.type = Entity::Type::kGrass4;
    entity_descr.world_position = tp::Vec3(0.0, 0.0, 0.0);
    entity_descr.rotation = 0;
    entity_descr.scale = 0.0;
    //entities_.emplace_back(render_device, entity_descr);

    // Shader
    std::string  vs_source = std::string(SHADER_PATH) + "grass.vert";
    std::string  gs_source = std::string(SHADER_PATH) + "grass.geom";
    std::string  tesc_source = std::string(SHADER_PATH) + "grass.tesc";
    std::string  tese_source = std::string(SHADER_PATH) + "grass.tese";
    std::string  ps_source = std::string(SHADER_PATH) + "grass.frag";
    std::vector<renderer::gl::Shader> shaders;
    shaders.push_back(render_device.CreateVertexShader(vs_source));
    shaders.push_back(render_device.CreateGeometryShader(gs_source));
    shaders.push_back(render_device.CreateTessallationControlShader(tesc_source));
    shaders.push_back(render_device.CreateTessallationEvaluationShader(tese_source));
    shaders.push_back(render_device.CreatePixelShader(ps_source));
    pipeline_ .emplace(render_device.CreatePipeline(shaders));
}

void Collection::Update(tp::Real dt, const Wind &wind) {
    for (auto &entity : entities_) {
        entity.Update(dt, wind);
    }
}

void Collection::Render(renderer::RenderDevice &render_device, renderer::gl::Buffer &uniform_buffer_scene, ShaderData &shader_data) {
    pipeline_->Use();
    for (auto &entity : entities_) {
        entity.Render(render_device, uniform_buffer_scene, shader_data);
    }
}

void Collection::RenderGui() {
    ImGui::Begin("Grass");
    
    ImGui::End();

    // TODO: make grass entity selectable in dropdown
    entities_.at(0).RenderGui();
}

}; // grass