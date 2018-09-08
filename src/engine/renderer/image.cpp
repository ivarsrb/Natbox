#include "image.h"
#include <exception>
#include <engine/util/log.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace engine::renderer {
Image::Image(const std::string file, const BitSize bit_size) {
    //stbi_set_flip_vertically_on_load(true);
    bit_size_ = bit_size;
    if (bit_size_ == BitSize::k8bit) {
        image8_ = stbi_load(file.c_str(), &size_.x, &size_.y, &num_chanels_, 0);
        mipmap_levels_ = GetMipMapLevels(size_.x, size_.y);
        if (!image8_) {
            throw std::runtime_error("(Image) Failed to load image " + file);
        }
        util::Log::Write1("Image (8bit) loaded '", file, "', mipmap levels ", mipmap_levels_, ", channel num. ", num_chanels_, ", size ", size_.x, "x", size_.y);
    }
    else if (bit_size_ == BitSize::k16bit) {
        image16_ = stbi_load_16(file.c_str(), &size_.x, &size_.y, &num_chanels_, 0);
        mipmap_levels_ = GetMipMapLevels(size_.x, size_.y);
        if (!image16_) {
            throw std::runtime_error("(Image) Failed to load image " + file);
        }
        util::Log::Write1("Image (16bit) loaded '", file, "', mipmap levels ", mipmap_levels_, ", channel num. ", num_chanels_, ", size ", size_.x, "x", size_.y);
    }
}

Image::~Image() {
    if (bit_size_ == BitSize::k8bit) {
        stbi_image_free(image8_);
    }
    else if (bit_size_ == BitSize::k16bit) {
        stbi_image_free(image16_);
    }
}

uint32_t Image::GetMipMapLevels(uint32_t width, uint32_t height) {
    return uint32_t(floor(log2(glm::max(width, height))) + 1);
}

glm::ivec2 Image::GetSize() const{
    return size_;
}

int Image::GetNumChanels() const{
    return num_chanels_;
}

uint32_t Image::GetMipMapLevels() const{
    return mipmap_levels_;
}

unsigned char * Image::GetImage8() const{
    return image8_;
}

unsigned short * Image::GetImage16() const {
    return image16_;
}
}; // renderer