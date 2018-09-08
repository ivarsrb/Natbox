#include "mesh.h"

namespace engine::renderer {

Mesh::Mesh() {

}

// Create vertex and index buffers from given data, set up VAO
void Mesh::SetData(RenderDevice &render_device, const std::vector<types::FPos> &vertices, 
                   const std::vector<uint32_t> &indices) {
    // Index and vertex buffers
    index_count_ = static_cast<uint32_t>(indices.size());
    // remove_reference because decltype cannot accept reference from function param 
    uint32_t vertex_type_size = sizeof(std::remove_reference_t<decltype(vertices)>::value_type);
    vertex_buffers_.push_back(render_device.CreateVertexBuffer(vertices.size() * vertex_type_size, vertices.data()));
    index_buffer_ = std::make_unique<gl::Buffer>();
    index_buffer_->Storage(indices.size() * sizeof(uint32_t), indices.data(), 0);
    // Vertex array
    std::vector<types::VertexAttributeDescr> vertex_attributes(1);
    vertex_attributes.at(0).attribute_index = RenderDevice::AttribLocation::kPosition;
    vertex_attributes.at(0).size = vertex_type_size / sizeof(float); // 3  // (start_of_next_attrib - start_of_this_attrib) / size_of_single_element_type
    vertex_attributes.at(0).type = renderer::types::RenderDataTypes::kFloat;
    vertex_attributes.at(0).offset = 0;
    vertex_attributes.at(0).stride = vertex_type_size; // always the same for all atributes within vertex array

    vertex_array_ = std::make_unique<gl::VertexArray>(render_device.CreateVertexArray(vertex_buffers_, vertex_attributes));
    vertex_array_->ElementBuffer(*index_buffer_);
}

void Mesh::Render(RenderDevice &render_device) {
    vertex_array_->Bind();
    render_device.DrawTrianglesIndexed32(0, index_count_);
    vertex_array_->Unbind();
}

void Mesh::RenderStrip(RenderDevice &render_device) {
    vertex_array_->Bind();
    render_device.DrawTriangleStripIndexed32(0, index_count_);
    vertex_array_->Unbind();
}


};