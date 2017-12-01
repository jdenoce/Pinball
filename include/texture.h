#ifndef TEXTURE_H
#define	TEXTURE_H

#include <string>

#include <GL/glew.h>
#include <Magick++.h>

//This class is for loading textures and binding them on the objects!
class Texture
{
  public:
      Texture(GLenum TextureTarget, const std::string& FileName);     //Texture constructor
      bool Load();                                                    //Loads the texture for an object
      void Bind(GLenum TextureUnit);                                  //Binds the texture to an object

  private:
      //private variables for the texture
      std::string m_fileName;
      GLenum m_textureTarget;
      GLuint m_textureObj;
      Magick::Image* m_pImage;
      Magick::Blob m_blob;
};


#endif
