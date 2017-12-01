#ifndef SHADER_H
#define SHADER_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "graphics_headers.h"

using namespace std;

//This class is all about loading and handling shaders!
class Shader
{
  public:
    Shader();                                                   //Shader constructor
    ~Shader();                                                  //Shader destructor
    bool Initialize();                                          //Initialize the shader!
    void Enable();                                              //Allows the program to use shaders
    bool AddShader(GLenum ShaderType, bool type);               //Obtains the shaders from a file
    bool Finalize();                                            //Finish up and link the shaders!
    GLint GetUniformLocation(const char* pUniformName);         //Returns the location

  private:
    //Shader private variables!
    GLuint m_shaderProg;    
    std::vector<GLuint> m_shaderObjList;
};

#endif  /* SHADER_H */
