#include "terrain.h"
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <engine/renderer/types.h>
#include <engine/renderer/image.h>
#include "config.h"

using namespace engine;

namespace app::study::terrain {
    
Terrain::TerrainDescr descr = { 129 , 128 , UINT16_MAX * 0.0005f , "terrain_129_16_flat.png" };

Terrain::Terrain(renderer::RenderDevice &render_device) : descr_(descr){
    Init(render_device);
}

void Terrain::Init(renderer::RenderDevice &render_device) {
    tile_ = std::make_unique<Tile>(render_device, descr_.resolution);
    GetHeightfield(render_device, descr_.heightfield_source_file);
    // Textures
    std::string tex_file_name = "rock64.png";
    diffuse_map_1_ = std::make_unique<renderer::gl::Texture2D>(render_device.CreateTexture2D(RES_PATH + tex_file_name, renderer::Image::BitSize::k8bit, false));
    // Scale
    world_from_local_ = glm::scale(glm::tmat4x4<tp::Real>(1.0), glm::tvec3<tp::Real>(descr_.extent, descr_.vertical_scale, descr_.extent));
}

// Get heightfield texture to read height data in shader
void Terrain::GetHeightfield(engine::renderer::RenderDevice &render_device, const std::string file_name) {
    heighfield_ = std::make_unique<renderer::gl::Texture2D>(render_device.CreateTexture2D(RES_PATH + file_name,
                                                             renderer::Image::BitSize::k16bit, false, 2));
    // Default is repeat, so sampling value on 1.0 gives the same as 0.0 (I don't know if this is bug or a feature)
    heighfield_->Parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    heighfield_->Parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Terrain::Render(renderer::RenderDevice &render_device, renderer::gl::Buffer &uniform_buffer_scene, ShaderData &shader_data) {
    heighfield_->BindTextureUnit(renderer::RenderDevice::TextureBindingPoint::kHeightmap);
    diffuse_map_1_->BindTextureUnit(renderer::RenderDevice::TextureBindingPoint::kDiffuse);

    shader_data.world_from_local = world_from_local_ * (glm::tmat4x4<tp::Real>)shader_data.world_from_local;
    uniform_buffer_scene.SubData(offsetof(ShaderData, world_from_local), sizeof(ShaderData::world_from_local), &shader_data.world_from_local[0]);
    tile_->Render(render_device);
}

void Terrain::RenderGui() {
    ImGui::Begin("Terrain");
    ImGui::End();
}
};

