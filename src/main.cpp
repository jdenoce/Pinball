#include <iostream>
#include "engine.h"


#include <string>

//The main function is the first function the program will run
int main(void)
{
  //Check if user has inputted a file name
  std::string filePath = "ignore";

  // Start an engine and run it then cleanup after
  Engine *engine = new Engine("Tutorial Window Name", 500, 720);

  //Check if the file name is valid
  if(!engine->Initialize(filePath))
  {
    printf("The engine failed to start.\n");
    delete engine;
    engine = NULL;
    return 1;
  }

  //begin the program!
  engine->Run();

  //delete everything when the engine gets stopped by the user
  delete engine;
  engine = NULL;


  return 0;
}

