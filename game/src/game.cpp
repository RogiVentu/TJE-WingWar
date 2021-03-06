#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "rendertotexture.h"
#include "shader.h"
#include "entitymesh.h"
#include "scene.h"
#include <math.h>
#include <cmath>
#include "utils.h"
#include "bulletmanager.h"
#include "bass.h"
#include "stages/gamestage.h"
#include "stages/menustage.h"
#include "stages/firstscreen.h"
#include "stages/endstage.h"
#include "stages/controlstage.h"

//some globals


std::map<std::string, Stage*> Stage::s_Stages;
Stage* Stage::current = NULL;
Game* Game::instance = NULL;

Camera* camera = NULL;
//Camera* camera = NULL;

Game::Game(SDL_Window* window)
{
	this->window = window;
	instance = this;


	// initialize attributes
	// Warning: DO NOT CREATE STUFF HERE, USE THE INIT 
	// things create here cannot access opengl
	SDL_GetWindowSize( window, &window_width, &window_height );
	std::cout << " * Window size: " << window_width << " x " << window_height << std::endl;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;

	keystate = NULL;
	mouse_locked = false;

}

//Here we have already GL working, so we can create meshes and textures
void Game::init(void)
{
    std::cout << " * Path: " << getPath() << std::endl;
    
    //SDL_SetWindowSize(window, 50,50);

	
	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	FirstScreen* firstscreen = new FirstScreen();
	Stage::s_Stages.insert(std::pair<std::string, Stage* >("firstscreen", firstscreen));
	MenuStage* menu = new MenuStage();
	Stage::s_Stages.insert(std::pair<std::string, Stage* >("menustate", menu));
	GameStage* gamestage = new GameStage();
	Stage::s_Stages.insert(std::pair<std::string, Stage* >("gamestage", gamestage));
	EndStage* endstage = new EndStage();
	Stage::s_Stages.insert(std::pair<std::string, Stage* >("endstage", endstage));
	ControlStage* controlstage = new ControlStage();
	Stage::s_Stages.insert(std::pair<std::string, Stage* >("controlstage", controlstage));

	gamestage->init();
	menu->init();
	endstage->init();
	controlstage->init();
	firstscreen->init();

	Stage::instance->current = Stage::s_Stages["firstscreen"];

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
	
}

//what to do when the image has to be draw
void Game::render(void)
{
	
	//camera->set();

	//example to render the FPS every 10 frames
	//drawText(2, 2, std::to_string(fps), Vector3(1, 1, 1), 2);

	Stage::instance->current->render();
	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);

	
}

void Game::update(double seconds_elapsed)
{
	Stage::instance->current->update(seconds_elapsed);
}

//Keyboard event handler (sync input)
void Game::onKeyPressed( SDL_KeyboardEvent event )
{
	Stage::instance->current->onKeyPressed(event);
	/*switch (event.keysym.sym)
	{
	case SDLK_ESCAPE: exit(0); //ESC key, kill the app
	}*/
}


void Game::onMouseButton( SDL_MouseButtonEvent event )
{
	Stage::instance->current->onMouseButton(event);

	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}

void Game::setWindowSize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
    
	/*
    Uint32 flags = SDL_GetWindowFlags(window);
    if(flags & SDL_WINDOW_ALLOW_HIGHDPI)
    {
        width *= 2;
        height *= 2;
    }
	*/

	glViewport( 0,0, width, height );
	camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}

