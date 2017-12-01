#include <iostream>
#include "texture.h"

//Texture constructor
Texture::Texture(GLenum TextureTarget, const std::string& FileName)
{
    m_textureTarget = TextureTarget;
    m_fileName      = FileName;
    m_pImage        = NULL;
}

//Loads the texture for an object
bool Texture::Load()
{
    //check if the texture file is viable
    try {
        m_pImage = new Magick::Image(m_fileName);
        m_pImage->write(&m_blob, "RGBA");
    }
    catch (Magick::Error& Error) {
        std::cout << "Error loading texture '" << m_fileName << "': " << Error.what() << std::endl;
        return false;
    }

    //Start loading the texture!
    glGenTextures(1, &m_textureObj);
    glBindTexture(m_textureTarget, m_textureObj);
    glTexImage2D(m_textureTarget, 0, GL_RGB, m_pImage->columns(), m_pImage->rows(), -0.5, GL_RGBA, GL_UNSIGNED_BYTE, m_blob.data());
    glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //return true because we loaded the texture!
    return true;
}

//Binds the texture to an object
void Texture::Bind(GLenum TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(m_textureTarget, m_textureObj);
}
