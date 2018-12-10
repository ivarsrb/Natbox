#include "app.h"
#include "config.h"

using namespace engine;

namespace app::study::grass {
App::App() : IPlatformApp( { glm::ivec2(WIN_WIDTH, WIN_HEIGHT),
                                                 WIN_TITLE,
                                                 glm::ivec2(WIN_POS_X, WIN_POS_Y),
                                                 USE_GUI, 
                                                 FIXED_FPS, 
                                                 FIXED_DT } ),
                                                 renderer_(IPlatformApp::configuration.win_size, grass_collection_)

{
    Init();
}

App::~App() {

}

void App::Init() {

}

void App::Update(const platform::Timer &timer, const platform::Input &input) {
    tp::Real dt = timer.VirtualDt();
    grass_collection_.Update(dt, wind_entity_);
    renderer_.Update(dt, input);
}

void App::Render() {
    renderer_.Render(grass_collection_, wind_entity_);
}

// NOTE: actual rendering takes place after Render function, this only sets up what to render and how it reacts 
void App::RenderGUI(const platform::Timer &timer) {
    renderer_.RenderGUI(timer,grass_collection_, wind_entity_);
}

void App::Resize(const glm::ivec2 &size) {
    // height will be significantly larger than specified on retina displays.
    IPlatformApp::configuration.win_size = size;
    renderer_.Resize(size);
}

void App::KeyPress(const platform::Input &input) {
 
}

void App::MouseMove(const platform::Input &input) {
    renderer_.MouseMove(input);
}

void App::MouseButtonPress(const engine::platform::Input &input) {

}

void App::MouseScroll(const platform::Input &input) {
    renderer_.MouseScroll(input, IPlatformApp::configuration.win_size);
}
};