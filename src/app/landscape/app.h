#pragma once
#include <engine/platform/i_platform_app.h>
#include "entity.h"

namespace app::landscape {
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

    EntityManager entity_manager_;
};
};