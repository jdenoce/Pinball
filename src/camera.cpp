#include "camera.h"

//Camera constructor
Camera::Camera()
{

}

//Camera destructor
Camera::~Camera()
{

}

bool Camera::Initialize(int w, int h)
{
  //--Init the view and projection matrices
  //  if you will be having a moving camera the view matrix will need to more dynamic
  //  ...Like you should update it before you render more dynamic 
  //  for this project having them static will be fine
  view = glm::lookAt( glm::vec3(0.0, 7, 8.5), //Eye Position 0, 12, 2.7
                      glm::vec3(0.0, 0.0, 0.0), //Focus point
                      glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 100000.0f); //Distance to the far plane, 
  eyePosX = 0.0;
  eyePosY = 7;
  eyePosZ = 10;
  focusX = 0.0;
  focusY = 0.0;
  focusZ = 0.0;


  return true;
}

//return the projection
glm::mat4 Camera::GetProjection()
{
  return projection;
}

//return the view
glm::mat4 Camera::GetView()
{
  return view;
}

//Update the camera for every frame
void Camera::Update()
{
        view = glm::lookAt(     glm::vec3(eyePosX, eyePosY, eyePosZ), //Eye Position
                                glm::vec3(focusX, focusY, focusZ), //Focus point
                                glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up
}

