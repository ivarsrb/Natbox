#pragma once 
#include <engine/util/math.h>
#include <engine/renderer/mesh.h>
#include "types.h"

namespace app::study::grass {
class Wind {
public:
    Wind(engine::renderer::RenderDevice &render_device);
    void InitGeometry(engine::renderer::RenderDevice &render_device);
    void Reset();
    void Stop();
    void Update(engine::tp::Real dt);
    void SetSpeed(engine::tp::Real speed);
    // Rotation angle theta of wind vector in spherical coordinates
    void SetRotation(engine::tp::Real angle);
    void Render(engine::renderer::RenderDevice &render_device, engine::renderer::gl::Buffer &uniform_buffer_scene, ShaderData &shader_data);
    void RenderGui();
    engine::tp::Vec3 VectorInPosition(const engine::tp::Vec3 &pos) const;
private:
    // Radius is strength of wind, theta rotates it around y axis
    engine::util::math::SphericVector s_vector_;
    // To render wind vector as a line
    engine::renderer::Mesh vector_mesh_;
    std::unique_ptr<engine::renderer::gl::Program> pipeline_;
};
};