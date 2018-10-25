#pragma once
#include <vector>
#include <optional>
#include <glm/glm.hpp>
#include <engine/platform/i_platform_app.h>
#include <engine/renderer/render_device.h>
#include <engine/core/camera.h>
#include <engine/core/types.h>
#include <engine/util/timing.h>
#include "types.h"
#include "wind.h"
#include "grass.h"

namespace app::study::grass {
class App : public engine::platform::IPlatformApp {
public:
    App();
    ~App() noexcept;
    App(App const&) = delete;
    App operator=(App const&) = delete;
private:
    void Update(const engine::platform::Timer &timer, const engine::platform::Input &input) override;
    void Render() override;
    void RenderGUI(const engine::platform::Timer &timer) override;
    void Resize(const glm::ivec2 &size) override;
    void KeyPress(const engine::platform::Input *input) override;
    void MouseMove(const engine::platform::Input *input) override;
    void MouseButtonPress(const engine::platform::Input *input) override;
    void MouseScroll(const engine::platform::Input *input) override;
    void Init();
    engine::util::Timing loading_clock_;
    engine::renderer::RenderDevice render_device_;
    std::optional<engine::core::Camera> camera_;
    ShaderData shader_data_;
    std::optional<engine::renderer::gl::Buffer> uniform_buffer_scene_;
    engine::tp::Real z_near_;
    engine::tp::Real z_far_;
    std::optional<Wind> wind_;
    std::optional<Grass> grass_;
};
};