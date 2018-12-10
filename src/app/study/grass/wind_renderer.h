#pragma once
#include <optional>
#include <engine/renderer/mesh.h>
#include "types.h"
#include "wind_entity.h"

namespace app::study::grass {

class WindRenderer {
public:
    WindRenderer();
    void Render(const WindEntity &wind_entity, engine::renderer::gl::Buffer &uniform_buffer, UniformData &uniform_data);
    void RenderGUI(WindEntity &wind_entity);
private:
    engine::renderer::Mesh vector_mesh_;
    std::optional<engine::renderer::gl::Program> pipeline_;
};
}; // grass