#pragma once
#include <glm/glm.hpp>
#include <string>

namespace engine::renderer {
// load image and accuaire it's parameters for use in textures
class Image {
public:
    // Component size
    enum class BitSize {
        k8bit = 0,
        k16bit
    };
    Image(const std::string file, const BitSize bit_size = BitSize::k8bit);
    ~Image();
    // non-copyable
    Image(Image const&) = delete;
    Image operator=(Image const&) = delete;
    uint32_t GetMipMapLevels(uint32_t width, uint32_t height);
    glm::ivec2 GetSize() const;
    int GetNumChanels() const;
    uint32_t GetMipMapLevels() const;
    // Return 8 bit image
    unsigned char* GetImage8() const;
    // Return 16 bit image
    unsigned short* GetImage16() const;

private:
    // Component size
    BitSize bit_size_ = BitSize::k8bit;
    unsigned char* image8_ = nullptr;
    unsigned short* image16_ = nullptr;
    // DImensions in pixels
    glm::ivec2 size_;
    int num_chanels_;
    // POssible mipmaps levels
    uint32_t mipmap_levels_;
};
}; //renderer

