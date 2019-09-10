#pragma once
#include <engine/platform/i_platform_app.h>
#include "grass_collection.h"
#include "wind_entity.h"
#include "renderer.h"

// Main application file for grass rendering simulation.
// Based on paper  'A simulation on grass swaying with dynamic wind force'
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
    void KeyPress(const engine::platform::Input &input) override;
    void MouseMove(const engine::platform::Input &input) override;
    void MouseButtonPress(const engine::platform::Input &input) override;
    void MouseScroll(const engine::platform::Input &input) override;
    void Init();
    GrassCollection grass_collection_;
    WindEntity wind_entity_;
    Renderer renderer_;
};
};