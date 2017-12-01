
#include "engine.h"

//Engine constructor
Engine::Engine(string name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
  glClearColor(1.0f, 1.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

//Alternative engine constructor
Engine::Engine(string name)
{
  m_WINDOW_NAME = name;
  m_WINDOW_HEIGHT = 0;
  m_WINDOW_WIDTH = 0;
  m_FULLSCREEN = true;
}

//Engine destructor
Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
}

//Initializes all of the values for the engine
bool Engine::Initialize(std::string config_file)
{
  cout << "Press spacebar immediately to run the pinball program" << endl;
  cout << "Staying on the paddle for too long will cause the pinball to become inactive" << endl;
  cout << "If this is the case, please restart the program" << endl;

  // Start a window
  m_window = new Window();
  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("The window failed to initialize.\n");
    return false;
  }

  // Start the graphics
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, config_file))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();

  //Initialize the holding input variables
  holdingA = 0;
  holdingD = 0;
  holdingW = 0;
  holdingS = 0;
  holdingLeft = 0;
  holdingRight = 0;
  holdingDown = 0;
  holdingUp = 0; 
  holdingSpace = 0;
  holdingAlt = 0;
  holdingZ = 0;
  holdingSlash = 0;
  holdingT = 0;

  // No errors
  return true;
}

//Runs a loop that lasts for the entirety of the program
void Engine::Run()
{
  m_running = true;

  while(m_running)
  {
    // Update the DT
    m_DT = getDT();

newInput = false;
    // Check the keyboard input
    while(SDL_PollEvent(&m_event) != 0)
    {
      Keyboard();
    }

    //Perform functions repeatedly if an input is being held
    if(holdingD == 1)
	  {
	        m_graphics->getUserInput('d');
	  }
	  if(holdingA == 1)
	  {
	        m_graphics->getUserInput('a');
	  }
	  if(holdingW == 1)
	  {
	        m_graphics->getUserInput('w');
	  }
	  if(holdingS == 1)
	  {
	        m_graphics->getUserInput('s');
	  }
	  if(holdingLeft == 1)
	  {
	        m_graphics->getUserInput('4');
	  }
	  if(holdingRight == 1)
	  {
	        m_graphics->getUserInput('6');
	  }
	  if(holdingUp == 1)
	  {
	        m_graphics->getUserInput('8');
	  }
	  if(holdingDown == 1)
	  {
	        m_graphics->getUserInput('2');
	  }
	  if(holdingSpace == 1)
	  {
		  m_graphics->getUserInput('y');
	  }
	  if(holdingAlt == 1)
	  {
		  m_graphics->getUserInput('h');
	  }
    if(holdingZ == 1)
	  {
		  m_graphics->getUserInput('z');
      holdingZ = 2;
	  }
	  if(holdingSlash == 1)
	  {
		  m_graphics->getUserInput('/');
      holdingSlash = 2;
	  }
    if(holdingSpace == 1)
	  {
		  m_graphics->getUserInput('l');
      holdingSpace = 2;
	  }
	  if(holdingT == 1)
	  {
		  m_graphics->getUserInput('t');
      holdingT = 2;
	  }

    // Update and render the graphics
    m_graphics->Update(m_DT);
    m_graphics->Render(newInput, input);

    // Swap to the Window
    m_window->Swap();
  }

  //run functions if an input is being held
  
}

//Checks for keyboard inputs and performs the functions
void Engine::Keyboard()
{
  //check if the program has ended and if so, don't perform any of the inputs
  if(m_event.type == SDL_QUIT)
  {
    m_running = false;
  }
  if (m_event.type == SDL_KEYDOWN)
  {
    //Set variables for holding and make them loop their functions on Run()
    if (m_event.key.keysym.sym == SDLK_a)
    {
        holdingA = 1;
    }
    if (m_event.key.keysym.sym == SDLK_d)
    {
        holdingD = 1;
    }
    if (m_event.key.keysym.sym == SDLK_w)
    {
        holdingW = 1;
    }
    if (m_event.key.keysym.sym == SDLK_s)
    {
        holdingS = 1;
    }
    if (m_event.key.keysym.sym == SDLK_LEFT)
    {
        holdingLeft = 1;
    }
    if (m_event.key.keysym.sym == SDLK_RIGHT)
    {
        holdingRight = 1;
    }
    if (m_event.key.keysym.sym == SDLK_UP)
    {
        holdingUp = 1;
    }
    if (m_event.key.keysym.sym == SDLK_DOWN)
    {
        holdingDown = 1;
    }
    if (m_event.key.keysym.sym == SDLK_LALT)
    {
        holdingAlt = 1;
    }
    if (m_event.key.keysym.sym == SDLK_RALT)
    {
        holdingAlt = 1;
    }
    if (m_event.key.keysym.sym == SDLK_z && holdingZ == 0)
    {
        holdingZ = 1;
    }
    if (m_event.key.keysym.sym == SDLK_SLASH && holdingSlash == 0)
    {
        holdingSlash = 1;
    }
    if (m_event.key.keysym.sym == SDLK_SPACE && holdingSpace == 0)
    {
        holdingSpace = 1;
    }
    if (m_event.key.keysym.sym == SDLK_t && holdingT == 0)
    {
        holdingT = 1;
    }

    //Normal input presses that happens once per press
    if (m_event.key.keysym.sym == SDLK_MINUS)
    {
        m_graphics->getUserInput('-');
    }
    if (m_event.key.keysym.sym == SDLK_EQUALS)
    {
        m_graphics->getUserInput('=');
    }
    if (m_event.key.keysym.sym == SDLK_BACKSPACE)
    {
        m_graphics->getUserInput('b');
    }

    //Quit the program
    if (m_event.key.keysym.sym == SDLK_ESCAPE)
    {
      m_running = false;
    }
    //adjust tableAmbient
    if (m_event.key.keysym.sym == SDLK_q)
    {
      newInput = true;
      input = 'q';
    }
    if (m_event.key.keysym.sym == SDLK_w)
    {
      newInput = true;
      input = 'w';
    }
    if (m_event.key.keysym.sym == SDLK_a)
    {
      newInput = true;
      input = 'a';
    }
    if (m_event.key.keysym.sym == SDLK_s)
    {
      newInput = true;
      input = 's';
    }
    //changing shader types
    if(m_event.key.keysym.sym == SDLK_RETURN)
    {
	newInput = true;
	input = 'r';

	if(state == false)
	{
		m_graphics->m_shader->Initialize();
		     
		m_graphics->m_shader->AddShader(GL_VERTEX_SHADER, true);
		m_graphics->m_shader->AddShader(GL_FRAGMENT_SHADER, true);
		      
		m_graphics->m_shader->Finalize();
		m_graphics->m_shader->Enable();
		
		state = true;
	}else{
		m_graphics->m_shader->Initialize();
		     
		m_graphics->m_shader->AddShader(GL_VERTEX_SHADER, false);
		m_graphics->m_shader->AddShader(GL_FRAGMENT_SHADER, false);
		      
		m_graphics->m_shader->Finalize();
		m_graphics->m_shader->Enable();
		
		state = false;
	}
    }
 
    
  }

  //check if inputs are no longer being held
  if (m_event.type == SDL_KEYUP)
  {
    if (m_event.key.keysym.sym == SDLK_a)
    {
            holdingA = 0;
    }
    if (m_event.key.keysym.sym == SDLK_d)
    {
            holdingD = 0;
    }
    if (m_event.key.keysym.sym == SDLK_w)
    {
            holdingW = 0;
    }
    if (m_event.key.keysym.sym == SDLK_s)
    {
            holdingS = 0;
    }
    if (m_event.key.keysym.sym == SDLK_LEFT)
    {
            holdingLeft = 0;
    }
    if (m_event.key.keysym.sym == SDLK_RIGHT)
    {
            holdingRight = 0;
    }
    if (m_event.key.keysym.sym == SDLK_UP)
    {
            holdingUp = 0;
    }
    if (m_event.key.keysym.sym == SDLK_DOWN)
    {
            holdingDown = 0;
    }
    if (m_event.key.keysym.sym == SDLK_SPACE)
    {
        holdingSpace = 0;
        m_graphics->getUserInput('L');
    }
    if (m_event.key.keysym.sym == SDLK_LALT)
    {
        holdingAlt = 0;
    }
    if (m_event.key.keysym.sym == SDLK_RALT)
    {
        holdingAlt = 0;
    }
    if (m_event.key.keysym.sym == SDLK_z)
    {
        holdingZ = 0;
        m_graphics->getUserInput('Z');
    }
    if (m_event.key.keysym.sym == SDLK_SLASH)
    {
        holdingSlash = 0;
        m_graphics->getUserInput('?');
    }
    if (m_event.key.keysym.sym == SDLK_t)
    {
        holdingT = 0;
        m_graphics->getUserInput('T');
    }
  }
}

//Returns the current delta time
unsigned int Engine::getDT()
{
  long long TimeNowMillis = GetCurrentTimeMillis();
  assert(TimeNowMillis >= m_currentTimeMillis);
  unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  m_currentTimeMillis = TimeNowMillis;
  return DeltaTimeMillis;
}

//Returns the time
long long Engine::GetCurrentTimeMillis()
{
  timeval t;
  gettimeofday(&t, NULL);
  long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  return ret;
}
