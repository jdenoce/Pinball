#include "graphics.h"
#include <Magick++.h>
#include "texture.h"
#include <GL/glew.h>
#include <fstream>
#include <btBulletDynamicsCommon.h>



//Graphics constructor
Graphics::Graphics()
{

}

//Graphics destructor
Graphics::~Graphics()
{

}

//Initialize all of the values, shaders, matrices, objects, camera, etc.
bool Graphics::Initialize(int width, int height, std::string config_file)
{
  // Used for the linux OS
  #if !defined(__APPLE__) && !defined(MACOSX)
    // cout << glewGetString(GLEW_VERSION) << endl;
    glewExperimental = GL_TRUE;

    auto status = glewInit();

    // This is here to grab the error that comes from glew init.
    // This error is an GL_INVALID_ENUM that has no effects on the performance
    glGetError();

    //Check for error
    if (status != GLEW_OK)
    {
      std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
      return false;
    }
  #endif

  // For OpenGL 3
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Init Camera
  m_camera = new Camera();
  if(!m_camera->Initialize(width, height))
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }

  broadphase = new btDbvtBroadphase();
  collisionConfiguration = new btDefaultCollisionConfiguration();
  dispatcher = new btCollisionDispatcher(collisionConfiguration);
  solver = new btSequentialImpulseConstraintSolver();

  dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration); 
  dynamicsWorld->setGravity(btVector3(0, -9.81, 10));

  GLuint gSampler;
  //intitialize planets and textures

  pTextureSphere = NULL;
  pTextureSphere = new Texture(GL_TEXTURE_2D, "../textures/blue.jpg");
  pTextureSphere->Load();
  triMeshSphere = new btTriangleMesh();
  sphere = new Object("../src/sphere.obj", triMeshSphere);

  pTextureTable = NULL;
  pTextureTable = new Texture(GL_TEXTURE_2D, "../textures/red.jpg");
  pTextureTable->Load();
  triMeshTable = new btTriangleMesh();
  table = new Object("../src/newtable.obj", triMeshTable);


  pTextureCylinder = NULL;
  pTextureCylinder = new Texture(GL_TEXTURE_2D, "../textures/green.png");
  pTextureCylinder->Load();
  triMeshCylinder = new btTriangleMesh();
  cylinder = new Object("../src/cylinder.obj", triMeshCylinder);

  triMeshCylinder2 = new btTriangleMesh();
  cylinder2 = new Object("../src/cylinder.obj", triMeshCylinder2);

  triMeshCylinder3 = new btTriangleMesh();
  cylinder3 = new Object("../src/cylinder.obj", triMeshCylinder3);

  pTexturePaddle = NULL;
  pTexturePaddle = new Texture(GL_TEXTURE_2D, "../textures/yellow.jpg");
  pTexturePaddle->Load();
  paddleLeft = new Object("../src/paddle.obj", triMeshPaddleLeft);

  paddleLeft2 = new Object("../src/paddle.obj", triMeshPaddleLeft2);

  paddleRight = new Object("../src/paddle.obj", triMeshPaddleRight);
    
  paddleRight2 = new Object("../src/paddle.obj", triMeshPaddleRight2);

  pTextureCube = NULL;
  pTextureCube = new Texture(GL_TEXTURE_2D, "../textures/yellow.jpg");
  pTextureCube->Load();
  cube = new Object("../src/cube.obj", triMeshCube);

  btScalar mass(0);
  btVector3 interia(0,0,0);
  
  //sphere
  shapeSphere = new btConvexTriangleMeshShape(&triMeshSphere, true);
  shapeMotionStateSphere = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(2.2,5.5,4.0)));//0,0.5,0
  mass = 10;
  shapeSphere->calculateLocalInertia(mass,interia);
  btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCISphere(mass, shapeMotionStateSphere, shapeSphere, interia);
  rigidBodySphere = new btRigidBody(shapeRigidBodyCISphere);
  dynamicsWorld->addRigidBody(rigidBodySphere,1, 1);

  //table
  shapeTable = new btBvhTriangleMeshShape(&triMeshTable, true);
  shapeMotionStateTable = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
  mass = 0;
  shapeTable->calculateLocalInertia(mass,interia);
  btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCITable(mass, shapeMotionStateTable, shapeTable, interia);
  rigidBodyTable = new btRigidBody(shapeRigidBodyCITable);
  dynamicsWorld->addRigidBody(rigidBodyTable,1, 1);

  mass = 0;//static objects beyond this point

  //cylinder
  shapeCylinder = new btCylinderShape(btVector3(0.2, 0.2, 0.2));
  shapeMotionStateCylinder = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(-0.2,0,-2)));
  shapeCylinder->calculateLocalInertia(mass,interia);
  btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCICylinder(mass, shapeMotionStateCylinder, shapeCylinder, interia);
  rigidBodyCylinder = new btRigidBody(shapeRigidBodyCICylinder);
  dynamicsWorld->addRigidBody(rigidBodyCylinder,1, 1);

  shapeCylinder2 = new btCylinderShape(btVector3(0.2, 0.2, 0.2));
  shapeMotionStateCylinder2 = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(-1.0,0,-1)));
  shapeCylinder2->calculateLocalInertia( mass,interia);
  btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCICylinder2(mass, shapeMotionStateCylinder2, shapeCylinder2, interia);
  rigidBodyCylinder2 = new btRigidBody(shapeRigidBodyCICylinder2);
  dynamicsWorld->addRigidBody(rigidBodyCylinder2,1, 1);

  shapeCylinder3 = new btCylinderShape(btVector3(0.2, 0.2, 0.2));
  shapeMotionStateCylinder3 = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0.6,0,-1)));
  shapeCylinder3->calculateLocalInertia(mass,interia);
  btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCICylinder3(mass, shapeMotionStateCylinder3, shapeCylinder3, interia);
  rigidBodyCylinder3 = new btRigidBody(shapeRigidBodyCICylinder3);
  dynamicsWorld->addRigidBody(rigidBodyCylinder3,1, 1);




//paddel
  //shapePaddleLeft = new btConvexTriangleMeshShape(&triMeshPaddleLeft, true);
  //shapeMotionStatePaddleLeft = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(-0.79,0.15,3.42)));
  //shapePaddleLeft->calculateLocalInertia( 0,interia);
  //btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCIPaddleLeft(0, shapeMotionStatePaddleLeft, shapePaddleLeft, interia);

  shapePaddleLeft = new btBvhTriangleMeshShape(&triMeshPaddleLeft, true);
  shapeMotionStatePaddleLeft = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(-1.4,0.4,3.3)));
  shapePaddleLeft->calculateLocalInertia( 1,interia);
  //btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCIPaddleLeft(1, shapeMotionStatePaddleLeft, shapePaddleLeft, interia);

  shapePaddleLeft = new btConvexTriangleMeshShape(&triMeshPaddleLeft, true);
  shapeMotionStatePaddleLeft = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(-0.99,0.15,3.42)));
  shapePaddleLeft->calculateLocalInertia( 0,interia);
  btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCIPaddleLeft(0, shapeMotionStatePaddleLeft, shapePaddleLeft, interia);
  rigidBodyPaddleLeft = new btRigidBody(shapeRigidBodyCIPaddleLeft);
  dynamicsWorld->addRigidBody(rigidBodyPaddleLeft,1, 1);


  shapePaddleLeft2 = new btConvexTriangleMeshShape(&triMeshPaddleLeft2, true);
  shapeMotionStatePaddleLeft2 = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(-1.90,0.12,1.72)));
  shapePaddleLeft2->calculateLocalInertia( 0,interia);
  btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCIPaddleLeft2(0, shapeMotionStatePaddleLeft2, shapePaddleLeft2, interia);
  rigidBodyPaddleLeft2 = new btRigidBody(shapeRigidBodyCIPaddleLeft2);
  dynamicsWorld->addRigidBody(rigidBodyPaddleLeft2,1, 1);

  shapePaddleRight = new btConvexTriangleMeshShape(&triMeshPaddleRight, true);
  shapeMotionStatePaddleRight = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(1.70,0.12,1.72)));
  shapePaddleRight->calculateLocalInertia( 0,interia);
  btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCIPaddleRight(0, shapeMotionStatePaddleRight, shapePaddleRight, interia);
  rigidBodyPaddleRight = new btRigidBody(shapeRigidBodyCIPaddleRight);
  dynamicsWorld->addRigidBody(rigidBodyPaddleRight,1, 1);

  shapePaddleRight2 = new btConvexTriangleMeshShape(&triMeshPaddleRight2, true);
  shapeMotionStatePaddleRight2 = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0.79,0.15,3.42)));
  shapePaddleRight2->calculateLocalInertia( 0,interia);
  btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCIPaddleRight2(0, shapeMotionStatePaddleRight2, shapePaddleRight2, interia);
  rigidBodyPaddleRight2 = new btRigidBody(shapeRigidBodyCIPaddleRight2);
  dynamicsWorld->addRigidBody(rigidBodyPaddleRight2,1, 1);

  shapeCubeLeft = new btBoxShape(btVector3(0.01,0.01,0.01));
  shapeMotionStateCubeLeft = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(-0.79,0.15,3.42)));
  shapeCubeLeft->calculateLocalInertia( 0,interia);
  btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCICubeLeft(0, shapeMotionStateCubeLeft, shapeCubeLeft, interia);
  rigidBodyCubeLeft = new btRigidBody(shapeRigidBodyCICubeLeft);
  dynamicsWorld->addRigidBody(rigidBodyCubeLeft,1, 1);


  //ground (now ceiling)
  shapeGround = new btStaticPlaneShape(btVector3(0, -1, 0), 1);
  shapeMotionStateGround = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,2,0)));
  shapeGround->calculateLocalInertia(mass,interia);
  btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCIGround(mass, shapeMotionStateGround, shapeGround, interia);
  rigidBodyGround = new btRigidBody(shapeRigidBodyCIGround);
  dynamicsWorld->addRigidBody(rigidBodyGround,1, 1);
  

  //walls
  shapeTopWall = new btStaticPlaneShape(btVector3(0, 0, 1), 1);
  shapeMotionStateTopWall = new btDefaultMotionState(btTransform(btQuaternion(0,0,-5.87,1),btVector3(0,0,-5.87)));
  shapeTopWall->calculateLocalInertia(mass,interia);
  btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCITopWall(mass, shapeMotionStateTopWall, shapeTopWall, interia);
  rigidBodyTopWall = new btRigidBody(shapeRigidBodyCITopWall);
  dynamicsWorld->addRigidBody(rigidBodyTopWall,1, 1);

  shapeLeftWall = new btStaticPlaneShape(btVector3(1, 0, 0), 1);
  shapeMotionStateLeftWall = new btDefaultMotionState(btTransform(btQuaternion(-4.05,0,0,1),btVector3(-4.05,0,0)));
  shapeLeftWall->calculateLocalInertia(mass,interia);
  btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCILeftWall(mass, shapeMotionStateLeftWall, shapeLeftWall, interia);
  rigidBodyLeftWall = new btRigidBody(shapeRigidBodyCILeftWall);
  dynamicsWorld->addRigidBody(rigidBodyLeftWall,1, 1);

  shapeRightWall = new btStaticPlaneShape(btVector3(-1, 0, 0), 1);
  shapeMotionStateRightWall = new btDefaultMotionState(btTransform(btQuaternion(4.05,0,0,1),btVector3(4.05,0,0)));
  shapeRightWall->calculateLocalInertia(mass,interia);
  btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCIRightWall(mass, shapeMotionStateRightWall, shapeRightWall, interia);
  rigidBodyRightWall = new btRigidBody(shapeRigidBodyCIRightWall);
  dynamicsWorld->addRigidBody(rigidBodyRightWall,1, 1);

  shapeBottomWall = new btStaticPlaneShape(btVector3(0, 0, -1), 1);
  shapeMotionStateBottomWall = new btDefaultMotionState(btTransform(btQuaternion(0,0,5.87,1),btVector3(0,0,6.87)));
  shapeBottomWall->calculateLocalInertia(mass,interia);
  btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCIBottomWall(mass, shapeMotionStateBottomWall, shapeBottomWall, interia);
  rigidBodyBottomWall = new btRigidBody(shapeRigidBodyCIBottomWall);
  dynamicsWorld->addRigidBody(rigidBodyBottomWall,1, 1);

  //initialize speed modifier
  gameSpeed = 1;


  //initialize cam position
  double vectorX = m_camera->focusX - m_camera->eyePosX;
  double vectorY = m_camera->focusY - m_camera->eyePosY;
  double vectorZ = m_camera->focusZ - m_camera->eyePosZ;

  for (int i = 0; i < 600; i++)
  {
    m_camera->eyePosX  = m_camera->eyePosX  + vectorX * (-0.1);
    m_camera->eyePosZ  = m_camera->eyePosZ  + vectorZ * (-0.1);
    m_camera->focusX = m_camera->focusX + vectorX * (-0.1);
    m_camera->focusZ = m_camera->focusZ + vectorZ * (-0.1);
  }

  for (int i = 0; i < 1200; i++)
  {
    m_camera->eyePosZ  = m_camera->eyePosZ  + vectorX * (-0.1);
    m_camera->eyePosX  = m_camera->eyePosX  + (-vectorZ) * (-0.1);
    
    m_camera->focusZ = m_camera->focusZ + vectorX * (-0.1);
    m_camera->focusX = m_camera->focusX + (-vectorZ) * (-0.1);
  }

  for (int i = 0; i < 10; i++)
  {
    m_camera->focusY -= 0.2;
  }

  for (int i = 0; i < 1000; i++)
  {
    m_camera->eyePosY += 0.1;
    m_camera->focusY += 0.1;
  }


  for (int i = 0; i < 480; i++)
  {
    m_camera->focusZ = (float)(m_camera->eyePosZ + sin(0.02)* vectorX + cos(0.02)* vectorZ);
    m_camera->focusX = (float)(m_camera->eyePosX + cos(0.02)* vectorX - sin(0.02)* vectorZ);
  }
  for (int i = 0; i < 480; i++)
  {
    m_camera->focusZ = (float)(m_camera->eyePosZ + sin(0.02)* vectorX + cos(0.02)* vectorZ);
    m_camera->focusX = (float)(m_camera->eyePosX + cos(0.02)* vectorX - sin(0.02)* vectorZ);
  }


  //initialize my paddle!
  btScalar scalarMatrix[16];
  btTransform transformThing;
  btQuaternion quaternionThing;

  rigidBodyPaddleLeft->getMotionState()->getWorldTransform(transformThing);
  quaternionThing.setEuler(leftBumperAngle, 0.0, 0.0);
	transformThing.setRotation(quaternionThing);
  transformThing.getOpenGLMatrix(scalarMatrix);
	rigidBodyPaddleLeft->getMotionState()->setWorldTransform(transformThing);
	rigidBodyPaddleLeft->setMotionState(rigidBodyPaddleLeft->getMotionState());

  rigidBodyPaddleLeft2->getMotionState()->getWorldTransform(transformThing);
  quaternionThing.setEuler(leftBumperAngle, 0.0, 0.0);
	transformThing.setRotation(quaternionThing);
  transformThing.getOpenGLMatrix(scalarMatrix);
	rigidBodyPaddleLeft2->getMotionState()->setWorldTransform(transformThing);
	rigidBodyPaddleLeft2->setMotionState(rigidBodyPaddleLeft2->getMotionState());

  rigidBodyPaddleRight->getMotionState()->getWorldTransform(transformThing);
  quaternionThing.setEuler(rightBumperAngle, 0.0, 0.0);
	transformThing.setRotation(quaternionThing);
  transformThing.getOpenGLMatrix(scalarMatrix);
	rigidBodyPaddleRight->getMotionState()->setWorldTransform(transformThing);
	rigidBodyPaddleRight->setMotionState(rigidBodyPaddleRight->getMotionState());

  rigidBodyPaddleRight2->getMotionState()->getWorldTransform(transformThing);
  quaternionThing.setEuler(rightBumperAngle, 0.0, 0.0);
	transformThing.setRotation(quaternionThing);
  transformThing.getOpenGLMatrix(scalarMatrix);
	rigidBodyPaddleRight2->getMotionState()->setWorldTransform(transformThing);
	rigidBodyPaddleRight2->setMotionState(rigidBodyPaddleRight2->getMotionState());


  // Set up the shaders
  m_shader = new Shader();
  if(!m_shader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!m_shader->AddShader(GL_VERTEX_SHADER, true))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_shader->AddShader(GL_FRAGMENT_SHADER, true))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the program
  if(!m_shader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  // Locate the projection matrix in the shader
  m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_modelMatrix not found\n");
    return false;
  }

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}


void Graphics::Update(unsigned int dt)
{

  
  // Update the objects with dt being multiplied by the speed set by user input to control the speed of the rotation/orbits 
  sphere->planet_update(dt, 0, rigidBodySphere, dynamicsWorld);

  cylinder->planet_update(dt, 0, rigidBodyCylinder, dynamicsWorld);
  cylinder2->planet_update(dt, 0, rigidBodyCylinder2, dynamicsWorld);
  cylinder3->planet_update(dt, 0, rigidBodyCylinder3, dynamicsWorld);


  btPoint2PointConstraint constraintLeft(*rigidBodyPaddleLeft , *rigidBodyCubeLeft, btVector3(-0.79,0.15,3.42), btVector3(-0.79, 0.15, 3.42));


  paddleLeft->planet_update(dt, 1, rigidBodyPaddleLeft, dynamicsWorld);
  paddleLeft2->planet_update(dt, 1, rigidBodyPaddleLeft2, dynamicsWorld);
  paddleRight->planet_update(dt, 1, rigidBodyPaddleRight, dynamicsWorld);
  paddleRight2->planet_update(dt, 1, rigidBodyPaddleRight2, dynamicsWorld);
  cube ->planet_update(dt, 0, rigidBodyCubeLeft, dynamicsWorld);

  //Bumper Stuff!
  if(leftHold == 1 && leftBumperAngle < 4.1f)
  {
    glm::mat4 paddleLeftTemp = paddleLeft->GetModel();
    glm::vec3 paddleLeftModel = glm::vec3(paddleLeftTemp[3]);


    btScalar scalarMatrix[16];
    btTransform transformThing;
    btQuaternion quaternionThing;
  
    leftBumperAngle += 0.2f;
    rigidBodyPaddleLeft->getMotionState()->getWorldTransform(transformThing);    
    quaternionThing.setEuler(leftBumperAngle, 0.0, 0.0);
		transformThing.setRotation(quaternionThing);
	  transformThing.getOpenGLMatrix(scalarMatrix);
	  rigidBodyPaddleLeft->getMotionState()->setWorldTransform(transformThing);
		rigidBodyPaddleLeft->setMotionState(rigidBodyPaddleLeft->getMotionState());

    rigidBodyPaddleLeft2->getMotionState()->getWorldTransform(transformThing);    
    quaternionThing.setEuler(leftBumperAngle, 0.0, 0.0);
		transformThing.setRotation(quaternionThing);
	  transformThing.getOpenGLMatrix(scalarMatrix);
	  rigidBodyPaddleLeft2->getMotionState()->setWorldTransform(transformThing);
		rigidBodyPaddleLeft2->setMotionState(rigidBodyPaddleLeft2->getMotionState());

  }

  if(rightHold == 1 && rightBumperAngle > -0.9f)
  {
    glm::mat4 paddleRightTemp = paddleRight->GetModel();
    glm::vec3 paddleRightModel = glm::vec3(paddleRightTemp[3]);

    btScalar scalarMatrix[16];
    btTransform transformThing;
    btQuaternion quaternionThing;
  
    rightBumperAngle -= 0.2f;
    rigidBodyPaddleRight->getMotionState()->getWorldTransform(transformThing);    
    quaternionThing.setEuler(rightBumperAngle, 0.0, 0.0);
		transformThing.setRotation(quaternionThing);
	  transformThing.getOpenGLMatrix(scalarMatrix);
	  rigidBodyPaddleRight->getMotionState()->setWorldTransform(transformThing);
		rigidBodyPaddleRight->setMotionState(rigidBodyPaddleRight->getMotionState());

    rigidBodyPaddleRight2->getMotionState()->getWorldTransform(transformThing);    
    quaternionThing.setEuler(rightBumperAngle, 0.0, 0.0);
		transformThing.setRotation(quaternionThing);
	  transformThing.getOpenGLMatrix(scalarMatrix);
	  rigidBodyPaddleRight2->getMotionState()->setWorldTransform(transformThing);
		rigidBodyPaddleRight2->setMotionState(rigidBodyPaddleRight2->getMotionState());

  }

  //Game Starter!
  if (startFrames > 0)
  {
    btScalar scalarMatrix[16];
    btTransform transformThing;
    btQuaternion quaternionThing;
    startFrames++;
    if (startFrames > 15)
    {
      dynamicsWorld->setGravity(btVector3(0, -9.81, 5));
      btVector3 theVector(0,0,-5);
      rigidBodySphere->setLinearVelocity(theVector);
      startFrames = 0;
      rigidBodySphere->clearForces();
      btVector3 zeroVector(0,0,2);
      rigidBodySphere->setLinearVelocity(zeroVector);
      rigidBodySphere->getMotionState()->getWorldTransform(transformThing);
      transformThing.setOrigin(btVector3(2.23145f, 1.122362f, -0.12489f));
	    rigidBodySphere->getMotionState()->setWorldTransform(transformThing);
		  rigidBodySphere->setMotionState(rigidBodySphere->getMotionState());
      dynamicsWorld->setGravity(btVector3(0, -9.81, 5));
    }
  }
  
  //Make gravity go back to normal after launching
  if (launchFrames > 0)
  {
    launchFrames++;
    //cout << launchFrames << endl;
    if (launchFrames > 25)
    {
      launchFrames = 0;
      dynamicsWorld->setGravity(btVector3(0, -9.81, 5));
    }
  }
  
  //Prevent accidental double deaths
  if (deathFrames > 0)
  {
    deathFrames++;
    if (deathFrames > 10)
    {
      deathFrames = 0;
    }
  }


  //Detect a loss
  glm::mat4 sphereTemp = sphere->GetModel();
  glm::vec3 sphereModel = glm::vec3(sphereTemp[3]);
  if (sphereModel.x < 2.1 && sphereModel.z > 5.0)
  {
    btTransform transformThing;
    if (livesRemaining < 1 && livesRemaining != -1000 && deathFrames == 0)
    {
        cout << "GAME OVER!!!" << endl;
        cout << "Your score for this game is: " << currentScore << endl;
        cout << "Press T to play again!" << endl << endl << endl << endl;
        livesRemaining = -1000;
        rigidBodySphere->clearForces();
        btVector3 zeroVector(0,0,0);
        rigidBodySphere->setLinearVelocity(zeroVector);
        rigidBodySphere->getMotionState()->getWorldTransform(transformThing);
        transformThing.setOrigin(btVector3(9999.0f, -9999.122362f, -999.12489f));
	      rigidBodySphere->getMotionState()->setWorldTransform(transformThing);
		    rigidBodySphere->setMotionState(rigidBodySphere->getMotionState());
        deathFrames = 1;
    }
    else if (deathFrames == 0)
    {
        cout << "YOU DIED!" << endl;
        cout << "Lives remaining: " << livesRemaining << endl << endl << endl;
        livesRemaining -= 1;
        rigidBodySphere->clearForces();
        btVector3 zeroVector(0,0,0);
        rigidBodySphere->setLinearVelocity(zeroVector);
        rigidBodySphere->getMotionState()->getWorldTransform(transformThing);
        transformThing.setOrigin(btVector3(2.23145f, 0.122362f, -0.12489f));
	      rigidBodySphere->getMotionState()->setWorldTransform(transformThing);
		    rigidBodySphere->setMotionState(rigidBodySphere->getMotionState());
        deathFrames = 1;
    }
    
  }



  //Detect the bumpers!
  glm::mat4 cylinderTemp = cylinder->GetModel();
  glm::mat4 cylinderTemp2 = cylinder2->GetModel();
  glm::mat4 cylinderTemp3 = cylinder3->GetModel();
  glm::vec3 cylinderModel = glm::vec3(cylinderTemp[3]);
  glm::vec3 cylinderModel2 = glm::vec3(cylinderTemp2[3]);
  glm::vec3 cylinderModel3 = glm::vec3(cylinderTemp3[3]);


  float xChecker = abs(sphereModel.x - cylinderModel.x);
  float zChecker = abs(sphereModel.z - cylinderModel.z);
  //Check for hit
  if ( xChecker < 0.2 && zChecker < 0.2)
  {
    currentScore += 500;
    cout << "Nice cylinder hit!" << endl;
    cout << "Your current score is: " << currentScore << endl << endl << endl << endl << endl;
  }

  xChecker = abs(sphereModel.x - cylinderModel2.x);
  zChecker = abs(sphereModel.z - cylinderModel2.z);
  //Check for hit
  if ( xChecker < 0.2 && zChecker < 0.2)
  {
    currentScore += 500;
    cout << "Nice cylinder hit!" << endl;
    cout << "Your current score is: " << currentScore << endl << endl << endl << endl << endl;
  }

  xChecker = abs(sphereModel.x - cylinderModel3.x);
  zChecker = abs(sphereModel.z - cylinderModel3.z);
  //Check for hit
  if ( xChecker < 0.2 && zChecker < 0.2)
  {
    currentScore += 500;
    cout << "Nice cylinder hit!" << endl;
    cout << "Your current score is: " << currentScore << endl << endl << endl << endl << endl;
  }


}


//Render the objects
void Graphics::Render(bool newInput, char input)
{
  //clear the screen
  glClearColor(0.0, 0.0, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Start the correct program
  m_shader->Enable();

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

 if(newInput){
    if(input == 'r'){
	if(shaderType1){
		tableAmbient = -0.2;
		shaderType1 = false;
	}else{
		tableAmbient = -0.9;
		shaderType1 = true;
	}
    }
    if(input == 'q'){
	tableAmbient += 0.1;
    }
    if(input == 'w'){
	tableAmbient -= 0.1;
    }
    if(input == 'a'){
	zval += 1;

	tableDiffuse += 0.1;
	tableSpecular += 0.1;
    }
    if(input == 's'){
	zval -= 1;

	tableDiffuse -= 0.1;
	tableSpecular -= 0.1;
    }
 }

  glUniform1f(m_shader->GetUniformLocation("ball"), 0.0);

  //sphere
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(sphere->GetModel()));
  glUniform4f(m_shader->GetUniformLocation("LightPosition"), 0,10,13,0);
  glUniform4f(m_shader->GetUniformLocation("spotLight"), 0,4,5,1);

  glUniform4f(m_shader->GetUniformLocation("AmbientProduct"), 0.5,0.5,0.5,1);
  glUniform4f(m_shader->GetUniformLocation("DiffuseProduct"), 0.5, 0.5, 0.5,1);
  glUniform4f(m_shader->GetUniformLocation("SpecularProduct"), 0, 0, 0,1);
  glUniform1f(m_shader->GetUniformLocation("Shininess"), .0005);
  sphere->Render(pTextureSphere);

  //table
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(table->GetModel()));
  glUniform4f(m_shader->GetUniformLocation("LightPosition"), 0,10,13,0);
  glUniform4f(m_shader->GetUniformLocation("spotLight"), 0,4,5,1);
  glUniform4f(m_shader->GetUniformLocation("secondLight"), 0,0,0,0);
  glUniform4f(m_shader->GetUniformLocation("AmbientProduct"), tableAmbient,tableAmbient,tableAmbient,1);
  glUniform4f(m_shader->GetUniformLocation("DiffuseProduct"), tableDiffuse,tableDiffuse,tableDiffuse,1);
  glUniform4f(m_shader->GetUniformLocation("SpecularProduct"), tableSpecular,tableSpecular,tableSpecular,1);
  glUniform1f(m_shader->GetUniformLocation("Shininess"), 1);
  table->Render(pTextureTable);

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(cylinder->GetModel()));
  glUniform4f(m_shader->GetUniformLocation("LightPosition"), 0,5,0,0);
  glUniform4f(m_shader->GetUniformLocation("AmbientProduct"), 0.5,0.5,0.5,1);
  glUniform4f(m_shader->GetUniformLocation("DiffuseProduct"), 1,1,1,1);
  glUniform4f(m_shader->GetUniformLocation("SpecularProduct"), 1,1,1,1);
  glUniform1f(m_shader->GetUniformLocation("Shininess"), 1);
  cylinder->Render(pTextureCylinder);

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(cylinder2->GetModel()));
  glUniform4f(m_shader->GetUniformLocation("LightPosition"), 0,5,0,0);
  glUniform4f(m_shader->GetUniformLocation("AmbientProduct"), 0.5,0.5,0.5,1);
  glUniform4f(m_shader->GetUniformLocation("DiffuseProduct"), 1,1,1,1);
  glUniform4f(m_shader->GetUniformLocation("SpecularProduct"), 1,1,1,1);
  glUniform1f(m_shader->GetUniformLocation("Shininess"), 1);
  cylinder2->Render(pTextureCylinder);

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(cylinder3->GetModel()));
  glUniform4f(m_shader->GetUniformLocation("LightPosition"), 0,5,0,0);
  glUniform4f(m_shader->GetUniformLocation("AmbientProduct"), 0.5,0.5,0.5,1);
  glUniform4f(m_shader->GetUniformLocation("DiffuseProduct"), 1,1,1,1);
  glUniform4f(m_shader->GetUniformLocation("SpecularProduct"), 1,1,1,1);
  glUniform1f(m_shader->GetUniformLocation("Shininess"), 1);
  cylinder3->Render(pTextureCylinder);

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(paddleLeft->GetModel()));
  glUniform4f(m_shader->GetUniformLocation("LightPosition"), 0,5,0,0);
  glUniform4f(m_shader->GetUniformLocation("AmbientProduct"), 0.5,0.5,0.5,1);
  glUniform4f(m_shader->GetUniformLocation("DiffuseProduct"), 1,1,1,1);
  glUniform4f(m_shader->GetUniformLocation("SpecularProduct"), 1,1,1,1);
  glUniform1f(m_shader->GetUniformLocation("Shininess"), 1);
  paddleLeft->Render(pTexturePaddle);


  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(paddleLeft2->GetModel()));
  glUniform4f(m_shader->GetUniformLocation("LightPosition"), 0,5,0,0);
  glUniform4f(m_shader->GetUniformLocation("AmbientProduct"), 0.5,0.5,0.5,1);
  glUniform4f(m_shader->GetUniformLocation("DiffuseProduct"), 1,1,1,1);
  glUniform4f(m_shader->GetUniformLocation("SpecularProduct"), 1,1,1,1);
  glUniform1f(m_shader->GetUniformLocation("Shininess"), 1);
  paddleLeft2->Render(pTexturePaddle);

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(paddleRight->GetModel()));
  glUniform4f(m_shader->GetUniformLocation("LightPosition"), 0,5,0,0);
  glUniform4f(m_shader->GetUniformLocation("AmbientProduct"), 0.5,0.5,0.5,1);
  glUniform4f(m_shader->GetUniformLocation("DiffuseProduct"), 1,1,1,1);
  glUniform4f(m_shader->GetUniformLocation("SpecularProduct"), 1,1,1,1);
  glUniform1f(m_shader->GetUniformLocation("Shininess"), 1);
  paddleLeft2->Render(pTexturePaddle);

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(paddleRight->GetModel()));
  paddleRight->Render(pTexturePaddle);


  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(paddleRight2->GetModel()));
  glUniform4f(m_shader->GetUniformLocation("LightPosition"), 0,5,0,0);
  glUniform4f(m_shader->GetUniformLocation("AmbientProduct"), 0.5,0.5,0.5,1);
  glUniform4f(m_shader->GetUniformLocation("DiffuseProduct"), 1,1,1,1);
  glUniform4f(m_shader->GetUniformLocation("SpecularProduct"), 1,1,1,1);
  glUniform1f(m_shader->GetUniformLocation("Shininess"), 1);
  paddleRight->Render(pTexturePaddle);


  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(paddleRight2->GetModel()));
  glUniform4f(m_shader->GetUniformLocation("LightPosition"), 0,5,0,0);
  glUniform4f(m_shader->GetUniformLocation("AmbientProduct"), 0.5,0.5,0.5,1);
  glUniform4f(m_shader->GetUniformLocation("DiffuseProduct"), 1,1,1,1);
  glUniform4f(m_shader->GetUniformLocation("SpecularProduct"), 1,1,1,1);
  glUniform1f(m_shader->GetUniformLocation("Shininess"), 1);
  paddleRight2->Render(pTexturePaddle);




  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }
}

//User inputs to control the camera
void Graphics::getUserInput(char input)
{
  //declare variables
  double movementX = 0;
  double movementY = 0;
  double movementZ = 0;
  double rotationX = 0;
  double rotationY = 0;

  //dynamicsWorld->setGravity(btVector3(0, -9.81, 15));

  double vectorX = m_camera->focusX - m_camera->eyePosX;
  double vectorY = m_camera->focusY - m_camera->eyePosY;
  double vectorZ = m_camera->focusZ - m_camera->eyePosZ;

  glm::mat4 sphereTemp = sphere->GetModel();
  glm::mat4 paddleLeftTemp = paddleLeft->GetModel();
  glm::mat4 paddleLeftTemp2 = paddleLeft2->GetModel();
  glm::mat4 paddleRightTemp = paddleRight->GetModel();
  glm::mat4 paddleRightTemp2 = paddleRight2->GetModel();

  glm::vec3 sphereModel = glm::vec3(sphereTemp[3]);
  glm::vec3 paddleLeftModel = glm::vec3(paddleLeftTemp[3]);
  glm::vec3 paddleLeftModel2 = glm::vec3(paddleLeftTemp2[3]);
  glm::vec3 paddleRightModel = glm::vec3(paddleRightTemp[3]);
  glm::vec3 paddleRightModel2 = glm::vec3(paddleRightTemp2[3]);

  btScalar scalarMatrix[16];
  btTransform transformThing;
  btQuaternion quaternionThing;

  //Left paddle!
  if (input == 'z')
  {
          float xChecker = abs(sphereModel.x - paddleLeftModel.x);
  			  float zChecker = abs(sphereModel.z - paddleLeftModel.z);
          //Check for hit
  				if ( xChecker < 0.5 && zChecker < 0.5)
          {
              btVector3 theVector(0,0,-15);
              rigidBodySphere->setLinearVelocity(theVector);
              currentScore += 200;
              cout << "Nice paddle hit!" << endl;
              cout << "Your current score is: " << currentScore << endl << endl << endl << endl << endl;
  			  }

          xChecker = abs(sphereModel.x - paddleLeftModel2.x);
  			  zChecker = abs(sphereModel.z - paddleLeftModel2.z);
          //Check for hit
  				if ( xChecker < 0.5 && zChecker < 0.5)
          {
              btVector3 theVector(0,0,-15);
              rigidBodySphere->setLinearVelocity(theVector);
              currentScore += 200;
              cout << "Nice paddle hit!" << endl;
              cout << "Your current score is: " << currentScore << endl << endl << endl << endl << endl;
  			  }

          leftHold = 1;
  }
  if (input == 'Z')
  {
    rigidBodyPaddleLeft->getMotionState()->getWorldTransform(transformThing);
    leftHold = 0;
    leftBumperAngle = 2.5f;
    quaternionThing.setEuler(leftBumperAngle, 0.0, 0.0);
		transformThing.setRotation(quaternionThing);
		transformThing.getOpenGLMatrix(scalarMatrix);
		rigidBodyPaddleLeft->getMotionState()->setWorldTransform(transformThing);
		rigidBodyPaddleLeft->setMotionState(rigidBodyPaddleLeft->getMotionState());

    rigidBodyPaddleLeft2->getMotionState()->getWorldTransform(transformThing);
    quaternionThing.setEuler(leftBumperAngle, 0.0, 0.0);
		transformThing.setRotation(quaternionThing);
		transformThing.getOpenGLMatrix(scalarMatrix);
		rigidBodyPaddleLeft2->getMotionState()->setWorldTransform(transformThing);
		rigidBodyPaddleLeft2->setMotionState(rigidBodyPaddleLeft2->getMotionState());



    //cout << sphereModel.x << ", " << sphereModel.y << ", " << sphereModel.z << endl;
  }



  //Right paddle!
  //Left paddle!
  if (input == '/')
  {
          float xChecker = abs(sphereModel.x - paddleRightModel.x);
  			  float zChecker = abs(sphereModel.z - paddleRightModel.z);
          //Check for hit
  				if ( xChecker < 0.5 && zChecker < 0.5)
          {
              btVector3 theVector(0,0,-15);
              rigidBodySphere->setLinearVelocity(theVector);
              currentScore += 200;
              cout << "Nice paddle hit!" << endl;
              cout << "Your current score is: " << currentScore << endl << endl << endl << endl << endl;
  			  }

          xChecker = abs(sphereModel.x - paddleRightModel2.x);
  			  zChecker = abs(sphereModel.z - paddleRightModel2.z);
          //Check for hit
  				if ( xChecker < 0.5 && zChecker < 0.5)
          {
              btVector3 theVector(0,0,-15);
              rigidBodySphere->setLinearVelocity(theVector);
              currentScore += 200;
              cout << "Nice paddle hit!" << endl;
              cout << "Your current score is: " << currentScore << endl << endl << endl << endl << endl;
  			  }

          rightHold = 1;
  }
  if (input == '?')
  {
    rigidBodyPaddleRight->getMotionState()->getWorldTransform(transformThing);
    rightHold = 0;
    rightBumperAngle = 0.6f;
    quaternionThing.setEuler(rightBumperAngle, 0.0, 0.0);
		transformThing.setRotation(quaternionThing);
		transformThing.getOpenGLMatrix(scalarMatrix);
		rigidBodyPaddleRight->getMotionState()->setWorldTransform(transformThing);
		rigidBodyPaddleRight->setMotionState(rigidBodyPaddleRight->getMotionState());

    rigidBodyPaddleRight2->getMotionState()->getWorldTransform(transformThing);
    quaternionThing.setEuler(rightBumperAngle, 0.0, 0.0);
		transformThing.setRotation(quaternionThing);
		transformThing.getOpenGLMatrix(scalarMatrix);
		rigidBodyPaddleRight2->getMotionState()->setWorldTransform(transformThing);
		rigidBodyPaddleRight2->setMotionState(rigidBodyPaddleRight2->getMotionState());
  }

  //Game resetter
  if (input == 't')
  {
    cout << "Resetting Game!" << endl;
    livesRemaining = 3;
    rigidBodySphere->clearForces();
    btVector3 zeroVector(0,0,2);
    rigidBodySphere->setLinearVelocity(zeroVector);
    rigidBodySphere->getMotionState()->getWorldTransform(transformThing);
    transformThing.setOrigin(btVector3(2.23145f, 1.122362f, -0.12489f));
	  rigidBodySphere->getMotionState()->setWorldTransform(transformThing);
		rigidBodySphere->setMotionState(rigidBodySphere->getMotionState());
    dynamicsWorld->setGravity(btVector3(0, -9.81, 5));
  }

  //Ball launcher!
  if (input == 'l')
  {
    //cout << sphereModel.x << ", " << sphereModel.y << ", " << sphereModel.z << endl;
    if (sphereModel.x > 2.2 && sphereModel.x < 2.3 && sphereModel.y > -0.1 && sphereModel.y < 0 && sphereModel.z > 5.6 && sphereModel.z < 5.7)
    {
        //rigidBodySphere->clearForces();
        //btVector3 zeroVector(0,0,-20);
        //rigidBodySphere->setLinearVelocity(zeroVector);
        launchFrames = 1;
        dynamicsWorld->setGravity(btVector3(0, -9.81, -25));
        currentScore += 100;
        cout << "Ball launch bonus!" << endl;
        cout << "Your current score is: " << currentScore << endl << endl << endl << endl;
    }
  }

  //speed modifier
  if (input == '-' && gameSpeed > 0)
  {
    gameSpeed -= 0.1;
  }
  if (input == '=')
  {
    gameSpeed += 0.1;
  }
  if (input == 'b')
  {
    gameSpeed = 1;
  }

  if(gameSpeed < 0.001)
  {
     gameSpeed = 0.001;
  }

  //detect input for moving horizontally
  if (input == 'a')
  {
    movementX = -0.1;
  }
  if (input == 'd')
  {
    movementX = 0.1;
  }

  //move the camera horizontally
  if (movementX != 0)
  {
    m_camera->eyePosZ  = m_camera->eyePosZ  + vectorX * movementX;
    m_camera->eyePosX  = m_camera->eyePosX  + (-vectorZ) * movementX;
    
    m_camera->focusZ = m_camera->focusZ + vectorX * movementX;
    m_camera->focusX = m_camera->focusX + (-vectorZ) * movementX;
  }

  //detect input for moving vertically
  if (input == 'h')
  {
    movementY = -0.3;
  }
  if (input == 'y')
  {
    movementY = 0.3;
  }

  //move the camera vertically
  if (movementY != 0)
  {
    m_camera->eyePosY += movementY;
    m_camera->focusY += movementY;
  }

  //detect input for moving forward or backward
  if (input == 'w')
  {
    movementZ = 0.1;
  }
  if (input == 's')
  {
    movementZ = -0.1;
  }

  //move the camera forward or backward
  if (movementZ != 0)
  {
    m_camera->eyePosX  = m_camera->eyePosX  + vectorX * movementZ;
    m_camera->eyePosZ  = m_camera->eyePosZ  + vectorZ * movementZ;
    m_camera->focusX = m_camera->focusX + vectorX * movementZ;
    m_camera->focusZ = m_camera->focusZ + vectorZ * movementZ;
  }

  //detect input for looking to the left or right
  if (input == '4')//left
  {
    dynamicsWorld->setGravity(btVector3(-15, -9.81, 10));
  }
  if (input == '6')//right
  {
    dynamicsWorld->setGravity(btVector3(15, -9.81, 10));
  }

  //detect input for looking up or down
  if (input == '8')//up
  {
    dynamicsWorld->setGravity(btVector3(0, -9.81, -5));
  }
  if (input == '2')//down
  {
    dynamicsWorld->setGravity(btVector3(0, -9.81, 5));
  }

  //look to the left or right
  if (rotationX != 0)
  {
    m_camera->focusZ = (float)(m_camera->eyePosZ + sin(rotationX)* vectorX + cos(rotationX)* vectorZ);
    m_camera->focusX = (float)(m_camera->eyePosX + cos(rotationX)* vectorX - sin(rotationX)* vectorZ);
  }

  //look up or down
  if (rotationY != 0)
  {
    m_camera->focusY -= rotationY;
  }      

}

//Displays errors encountered by OpenGL on the terminal
std::string Graphics::ErrorString(GLenum error)
{
  if(error == GL_INVALID_ENUM)
  {
    return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
  }

  else if(error == GL_INVALID_VALUE)
  {
    return "GL_INVALID_VALUE: A numeric argument is out of range.";
  }

  else if(error == GL_INVALID_OPERATION)
  {
    return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
  }

  else if(error == GL_INVALID_FRAMEBUFFER_OPERATION)
  {
    return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
  }

  else if(error == GL_OUT_OF_MEMORY)
  {
    return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
  }
  else
  {
    return "None";
  }
}

