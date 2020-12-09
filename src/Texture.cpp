#include "Texture.hpp"
#include "Util.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using std::shared_ptr;
using std::string;

std::vector<TexInfo> Texture::textureSlate;
#include <iostream>
QSharedPointer<Texture> Texture::load(const string &filePath, bool flip) {
    auto texture_info = QSharedPointer<Texture>(new Texture(filePath, flip));
    return texture_info;
}

QSharedPointer<Texture> Texture::loadHDR(const std::string &filePath) {
    auto texture_info = QSharedPointer<Texture>(new Texture(filePath));
    return texture_info;
}

Texture::Texture(const string &fileName, bool flip) {
    stbi_set_flip_vertically_on_load(flip);
    unsigned char *image = stbi_load(fileName.c_str(), &width, &height, 0, STBI_rgb_alpha);

    if (image) {
        glGenTextures(1, &this->textureID);
        glBindTexture(GL_TEXTURE_2D, this->textureID);

        // TODO - make these customizable and generate mipmaps as well
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(image);
    } else {
        std::cout << "Failed to load regular image " << fileName << "\n";;
    }
}

Texture::Texture(const string &fileName) {
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponents;
    float *data = stbi_loadf(fileName.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else {
        std::cout << "Failed to load HDR image " << fileName << "\n";
    }
}

void Texture::bind(int slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, this->textureID);
}

// Stupid but the idea is to automatically bump the slots and be able to use the return value
int Texture::dynbind() {
    // TODO - throw an asset here in the future
    glActiveTexture(GL_TEXTURE0 + boundSlots++);
    glBindTexture(GL_TEXTURE_2D, this->textureID);

    return boundSlots - 1;
}

void Texture::unbind() {
    // pass
}

int Texture::ID() {
    return this->textureID;
}

//GLint Texture::loadFromFile(const char *path, string directory) {
//    stbi_set_flip_vertically_on_load(true);

//    string fileName = string(path);
//    string absFilePath = directory + '/' + fileName;

//    fileName = file_exists(absFilePath) ? absFilePath : fileName;

//    GLuint textureID;
//    glGenTextures(1, &textureID);

//    int width, height;
//    unsigned char* image = stbi_load(
//                                fileName.c_str(),
//                                &width, &height,
//                                0, STBI_rgb_alpha
//                            );

//    glBindTexture(GL_TEXTURE_2D, textureID);
//    glTexImage2D(
//        GL_TEXTURE_2D, 0, GL_RGBA,
//        width, height, 0, GL_RGBA,
//        GL_UNSIGNED_BYTE, image
//    );

////    glGenerateMipmap(GL_TEXTURE_2D);

////    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
////    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
////    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
////    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//    glBindTexture(GL_TEXTURE_2D, 0);
//    stbi_image_free(image);

//    return textureID;
//}
