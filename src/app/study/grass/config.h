#pragma once
// Initial application configuration data
// Window
#define WIN_WIDTH 1024
#define WIN_HEIGHT 768
#define WIN_TITLE "Grass study"
#define WIN_POS_X 700
#define WIN_POS_Y 100
#define USE_GUI true
// Fixed to refresh rate of the monitor
#define FIXED_FPS true
// At what fixed interval simulation is updated (render dt is varying)
#define FIXED_DT (1/60.)
// PATHES
#define MODELS_PATH "res/study/grass/models/"
#define SHADER_PATH "shaders/study/grass/"