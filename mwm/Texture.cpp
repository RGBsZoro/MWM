#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include<GL/stb_image.h>
#include "Cuboid.h"
#include "Texture.h"
#include <iostream> 

void Texture::Use()
{
    glEnable(GL_TEXTURE_2D);								// Enable texture mapping
    glBindTexture(GL_TEXTURE_2D, textureID);				// Bind the texture as the current one
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}
void Texture::loadTexture(const char* filename) {
    stbi_set_flip_vertically_on_load(true);
    int width, height, channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);

    if (image) {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

        // استخدم هذه الدالة البديلة فهي أضمن للإصدارات القديمة
        gluBuild2DMipmaps(GL_TEXTURE_2D, format, width, height, format, GL_UNSIGNED_BYTE, image);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        std::cout << "SUCCESS: Loaded " << filename << std::endl;
        stbi_image_free(image);
    }
    else {
        // إذا طبع هذه الرسالة، فالمشكلة في مكان وجود الصورة
        std::cout << "ERROR: Could not find image at: " << filename << std::endl;
    }
}
