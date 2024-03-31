#include "Texture.h"

#include <iostream>

unsigned int Texture::GetID() const { return ID; }

Texture::Texture(const char *texturePath, TextureType type, GLenum target) {
    glGenTextures(1, &ID);
    glBindTexture(target, ID);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data =
        stbi_load(texturePath, &width, &height, &nrChannels, 0);

    if (data) {

        if (type == JPG) {
            glTexImage2D(target, 0, GL_RGB, width, height, 0, GL_RGB,
                         GL_UNSIGNED_BYTE, data);
        } else if (type == PNG) {
            glTexImage2D(target, 0, GL_RGBA, width, height, 0, GL_RGBA,
                         GL_UNSIGNED_BYTE, data);
        }
        glGenerateMipmap(target);
    } else {
        std::cout << "ERROR::FAILED::TO::LOAD::TEXTURE" << std::endl;
    }
    stbi_image_free(data);
}
