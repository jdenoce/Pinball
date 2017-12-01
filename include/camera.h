#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

//Camera class is what controls the view for the program, in which it's controlled by the user through graphics.cpp
class Camera
{
  public:
    Camera();                             //Camera constructor
    ~Camera();                            //Camera destructor
    bool Initialize(int w, int h);        //Initialize the camera
    void Update();                        //Update the camera's position and angle from user input
    glm::mat4 GetProjection();            //return the projection
    glm::mat4 GetView();                  //return the view

    //public variables
    double eyePosX;
    double eyePosY;
    double eyePosZ;
    double focusX;
    double focusY;
    double focusZ;
  
  private:
    glm::mat4 projection;
    glm::mat4 view;
};

#endif /* CAMERA_H */
