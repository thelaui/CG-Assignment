# include "include/Texture.hpp"

# include <IL/il.h>
# include <GL/glew.h>
# include <iostream>

Texture::Texture(std::string const& file):
    texID_(0) {

    ILuint imageID (0);
    ilGenImages(1, &imageID);
    ilBindImage(imageID);

    if (!ilLoadImage(file.c_str()))
        std::cout << "buh" << std::endl;


    // generate texture id
    glGenTextures(1, &texID_);
    if (texID_ == 0) {
        // OpenGL was not able to generate additional texture
        ilBindImage(0);
        ilDeleteImage(imageID);
        return;
    }

    glEnable(GL_TEXTURE_2D);
    // bind texture object
    glBindTexture(GL_TEXTURE_2D, texID_);
    // load image data as texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                ilGetInteger(IL_IMAGE_WIDTH),
                ilGetInteger(IL_IMAGE_HEIGHT),
                0, GL_RGB, GL_UNSIGNED_BYTE, ilGetData());

    //setting Texture Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    ilBindImage(0);
    ilDeleteImage(imageID);
}

Texture::~Texture() {
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &texID_);
}

void Texture::bind(unsigned texPos) {
    glActiveTexture(GL_TEXTURE0 + texPos);
    glBindTexture(GL_TEXTURE_2D, texID_);
}

void Texture::unbind(unsigned texPos) {
    glActiveTexture(GL_TEXTURE0 + texPos);
    glBindTexture(GL_TEXTURE_2D, 0);
}



