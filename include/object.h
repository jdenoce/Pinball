#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"
#include <string>
#include "texture.h"
#include <btBulletDynamicsCommon.h>

//This class handles all of the movement and rendering of every object, including planets, moons, rings, and orbit displays
class Object
{
  public:
    Object(std::string myFile, btTriangleMesh &objMesh);                                                                         //takes string of obj file to load, Object constructor
    ~Object();                                                                                          //Object destructor
    void planet_update(unsigned int dt, float radius, btRigidBody *rigidBody, btDiscreteDynamicsWorld *dynamicsWorld);        //Updates position, rotation, and orbit of a planet
    void Render(Texture* pTexture);  
                                                               //Commence rendering

    glm::mat4 GetModel();                                                                               //Returns the model

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    float angle;
};

#endif /* OBJECT_H */
