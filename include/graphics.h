#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <array>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"
#include "texture.h"
#include <btBulletDynamicsCommon.h>

#include "light.h"

class Graphics
{
  public:
    Graphics();                                                                 //Graphics constructor
    ~Graphics();                                                                //Graphics destructor
    bool Initialize(int width, int height, std::string config_file);            //Initialize all of the values, shaders, matrices, objects, camera, etc.
    void Update(unsigned int dt);                                               //Update the objects and the camera for every frame
    void Render(bool newInput, char input);                                     //Render the objects
    void getUserInput(char input);                                              //User inputs to control the camera and speed of objects
    void parse_config(std::string config_file);                                 //Obtain values from the config file

    //the shader!
    Shader *m_shader;
  private:
    float leftBumperAngle = 2.5f;
    float rightBumperAngle = 0.6f;
    bool leftHold = 0;
    bool rightHold = 0;
    int launchFrames = 0;
    int livesRemaining = 2;
    int currentScore = 0;
    int deathFrames = 0;
    int startFrames = 1;

    float zval = 0;
    bool shaderType1 = true;
    float tableAmbient = -0.9;//-0.2
    float tableDiffuse = 1;
    float tableSpecular = 1.0;

    //Displays errors encountered by OpenGL on the terminal
    std::string ErrorString(GLenum error);

    //the textures!
    Texture* pTextureSphere;
    Object *sphere;

    Texture* pTextureTable;
    Object *table;

    Texture* pTextureCube;
    Object *cube;

    Texture* pTextureCylinder;
    Object *cylinder;
    Object *cylinder2;
    Object *cylinder3;

    Texture* pTexturePaddle;
    Object *paddleLeft;
    Object *paddleLeft2;
    Object *paddleRight;
    Object *paddleRight2;

    //the camera!
    Camera *m_camera;

    //the shader!
    //Shader *m_shader;

    //the matrices!
    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;


    //physics
    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *dynamicsWorld;

    btTriangleMesh triMeshSphere;
    btCollisionShape *shapeSphere;
    btDefaultMotionState *shapeMotionStateSphere;
    btRigidBody *rigidBodySphere;

    btTriangleMesh triMeshCube;
    btCollisionShape *shapeCube;
    btDefaultMotionState *shapeMotionStateCube;
    btRigidBody *rigidBodyCube;

    btTriangleMesh triMeshCylinder;
    btCollisionShape *shapeCylinder;
    btDefaultMotionState *shapeMotionStateCylinder;
    btRigidBody *rigidBodyCylinder;

    btTriangleMesh triMeshCylinder2;
    btCollisionShape *shapeCylinder2;
    btDefaultMotionState *shapeMotionStateCylinder2;
    btRigidBody *rigidBodyCylinder2;

    btTriangleMesh triMeshCylinder3;
    btCollisionShape *shapeCylinder3;
    btDefaultMotionState *shapeMotionStateCylinder3;
    btRigidBody *rigidBodyCylinder3;


    btTriangleMesh triMeshPaddleLeft;
    btCollisionShape *shapePaddleLeft;
    btDefaultMotionState *shapeMotionStatePaddleLeft;
    btRigidBody *rigidBodyPaddleLeft;

    btTriangleMesh triMeshPaddleLeft2;
    btCollisionShape *shapePaddleLeft2;
    btDefaultMotionState *shapeMotionStatePaddleLeft2;
    btRigidBody *rigidBodyPaddleLeft2;

    btTriangleMesh triMeshPaddleRight;
    btCollisionShape *shapePaddleRight;
    btDefaultMotionState *shapeMotionStatePaddleRight;
    btRigidBody *rigidBodyPaddleRight;

    btTriangleMesh triMeshPaddleRight2;
    btCollisionShape *shapePaddleRight2;
    btDefaultMotionState *shapeMotionStatePaddleRight2;
    btRigidBody *rigidBodyPaddleRight2;


    btTriangleMesh triMeshCubeLeft;
    btCollisionShape *shapeCubeLeft;
    btDefaultMotionState *shapeMotionStateCubeLeft;
    btRigidBody *rigidBodyCubeLeft;


    btTriangleMesh triMeshTable;
    btCollisionShape *shapeTable;
    btDefaultMotionState *shapeMotionStateTable;
    btRigidBody *rigidBodyTable;

    btCollisionShape *shapeGround;
    btDefaultMotionState *shapeMotionStateGround;
    btRigidBody *rigidBodyGround;

    btCollisionShape *shapeTopWall;
    btDefaultMotionState *shapeMotionStateTopWall;
    btRigidBody *rigidBodyTopWall;

    btCollisionShape *shapeLeftWall;
    btDefaultMotionState *shapeMotionStateLeftWall;
    btRigidBody *rigidBodyLeftWall;

    btCollisionShape *shapeRightWall;
    btDefaultMotionState *shapeMotionStateRightWall;
    btRigidBody *rigidBodyRightWall;

    btCollisionShape *shapeBottomWall;
    btDefaultMotionState *shapeMotionStateBottomWall;
    btRigidBody *rigidBodyBottomWall;




    //private variable that controls speed of rotations/orbits
    double gameSpeed;
};


#endif /* GRAPHICS_H */
