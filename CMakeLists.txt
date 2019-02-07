cmake_minimum_required(VERSION 2.8.9)
# Set c++ copiler to use
# NOTE: this method to set variable here is not recomended (check CMAKE wiki "How do I use a different compiler?")
set(CMAKE_CXX_COMPILER "g++-8")
set(PROJ_NAME natbox)
project(${PROJ_NAME})

#### Sources and includes 
include_directories(src src/external)
# Add manually all source files, automatioc GLOB is not recommended
set(SOURCE_DIR src)
set(SOURCES 
            ${SOURCE_DIR}/main.cpp 
            
            # external compilable libraries
            ${SOURCE_DIR}/external/glad/glad.c
            ${SOURCE_DIR}/external/imgui/imgui.cpp
            ${SOURCE_DIR}/external/imgui/imgui_draw.cpp

            # engine source
            ${SOURCE_DIR}/engine/core/camera.cpp
            ${SOURCE_DIR}/engine/platform/gui/imgui_impl_glfw_gl3.cpp
            ${SOURCE_DIR}/engine/platform/debug.cpp
            ${SOURCE_DIR}/engine/platform/i_platform_app.cpp
            ${SOURCE_DIR}/engine/platform/input.cpp
            ${SOURCE_DIR}/engine/platform/timer.cpp
            ${SOURCE_DIR}/engine/renderer/image.cpp
            ${SOURCE_DIR}/engine/renderer/render_device.cpp
            ${SOURCE_DIR}/engine/renderer/mesh.cpp
            ${SOURCE_DIR}/engine/renderer/gl/buffer.cpp
            ${SOURCE_DIR}/engine/renderer/gl/frame_buffer.cpp
            ${SOURCE_DIR}/engine/renderer/gl/program.cpp
            ${SOURCE_DIR}/engine/renderer/gl/shader.cpp
            ${SOURCE_DIR}/engine/renderer/gl/texture_2d.cpp
            ${SOURCE_DIR}/engine/renderer/gl/vertex_array.cpp
            ${SOURCE_DIR}/engine/util/timing.cpp
            ${SOURCE_DIR}/engine/util/math.cpp

            # applications source
            #${SOURCE_DIR}/app/study/terrain/app.cpp
            #${SOURCE_DIR}/app/study/terrain/terrain.cpp
            #${SOURCE_DIR}/app/study/terrain/tile.cpp  

            ${SOURCE_DIR}/app/study/grass/app.cpp
            ${SOURCE_DIR}/app/study/grass/renderer.cpp
            ${SOURCE_DIR}/app/study/grass/grass_entity.cpp
            ${SOURCE_DIR}/app/study/grass/grass_model.cpp
            ${SOURCE_DIR}/app/study/grass/grass_physics.cpp
            ${SOURCE_DIR}/app/study/grass/grass_renderer.cpp
            ${SOURCE_DIR}/app/study/grass/grass_collection.cpp
            ${SOURCE_DIR}/app/study/grass/wind_entity.cpp
            ${SOURCE_DIR}/app/study/grass/wind_renderer.cpp

            #${SOURCE_DIR}/app/landscape/app.cpp
    )

#### Packages
find_package(glfw3 3.2 REQUIRED)
#find_package(GLM REQUIRED)

#### Compile and link flags
# -D _DEBUG is to define debug symbol to work like in Visual Studio
# -D NDEBUG will disable assert()
SET(GCC_COVERAGE_COMPILE_FLAGS "-g -Wall -std=c++17 -D _DEBUG")
SET(GCC_COVERAGE_LINK_FLAGS    "")
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )

add_executable(${PROJ_NAME} ${SOURCES})
#### Add libraries here
target_link_libraries(${PROJ_NAME}
  glfw
)