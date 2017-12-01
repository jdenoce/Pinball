#ifndef ENGINE_H
#define ENGINE_H

#include <sys/time.h>
#include <assert.h>
#include <iostream>
#include "window.h"
#include "graphics.h"

//Engine is responsible for running everything in the program for every frame
class Engine
{
  public:
    Engine(string name, int width, int height);                 //Engine constructor
    Engine(string name);                                        //Alternative engine constructor
    ~Engine();                                                  //Engine destructor
    bool Initialize(std::string config_file);                   //Initializes all of the values for the engine
    void Run();                                                 //Runs a loop that does everything that lasts for the entirety of the program
    void Keyboard();                                            //Checks for keyboard inputs and performs the functions
    unsigned int getDT();                                       //Returns the current delta time
    long long GetCurrentTimeMillis();                           //Returns the time

    //public variables for holding inputs
    bool holdingA;
    bool holdingD;
    bool holdingW;
    bool holdingS;
    bool holdingLeft;
    bool holdingRight;
    bool holdingUp;
    bool holdingDown;
    bool holdingAlt;
    int holdingZ;
    int holdingSlash;
    int holdingSpace;
    int holdingT;

    bool state = true;
    bool newInput;
    char input;
  
  private:
    // Window related variables
    Window *m_window;    
    string m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;
    SDL_Event m_event;

    Graphics *m_graphics;
    unsigned int m_DT;
    long long m_currentTimeMillis;
    bool m_running;
};

#endif // ENGINE_H
