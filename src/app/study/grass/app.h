#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <engine/platform/i_platform_app.h>
#include <engine/renderer/render_device.h>
#include <engine/core/camera.h>
#include <engine/core/types.h>
#include <engine/util/timing.h>
#include "types.h"
#include "grass_blade.h"
#include "wind.h"

namespace app::study::grass {
class App : public engine::platform::IPlatformApp {
public:
    App();
    ~App() noexcept;
    App(App const&) = delete;
    App operator=(App const&) = delete;
private:
    void Update(const engine::platform::IPlatformApp::Timing *time, const engine::platform::Input *input) override;
    void Render(const engine::platform::IPlatformApp::Timing *time) override;
    void Resize(const glm::ivec2 &size) override;
    void KeyPress(const engine::platform::Input *input) override;
    void MouseMove(const engine::platform::Input *input) override;
    void MouseButtonPress(const engine::platform::Input *input) override;
    void MouseScroll(const engine::platform::Input *input) override;
    void Init();
    void RenderGui();
    engine::util::Timing loading_clock_;
    engine::renderer::RenderDevice render_device_;
    std::unique_ptr<engine::core::Camera> camera_;
    ShaderData shader_data_;
    std::unique_ptr<engine::renderer::gl::Buffer> uniform_buffer_scene_;
    engine::tp::Real z_near_;
    engine::tp::Real z_far_;
    std::unique_ptr<engine::renderer::gl::Program> pipeline_;
    std::unique_ptr<GrassBlade> grass_blade_;
    Wind wind_;
};
};