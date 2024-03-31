#ifndef SLOMMGL_TEXTURE_H
#define SLOMMGL_TEXTURE_H

#include "glad/glad.h"
#include "stb_image.h"

enum TextureType { JPG, PNG };

class Texture {
    unsigned int ID{};

  public:
    Texture(const char *texturePath, TextureType type, GLenum target);

    [[nodiscard]] unsigned int GetID() const;
};

#endif // SLOMMGL_TEXTURE_H
