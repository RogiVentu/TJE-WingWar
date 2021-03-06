#include "menustage.h"
#include "../bass.h"
#include "gamestage.h"

MenuStage* MenuStage::instance = NULL;
//El handler para un sample
HSAMPLE hSample2;

SDL_Joystick *joym = NULL;

//El handler para un canal
HCHANNEL hSampleChannel2;

MenuStage::MenuStage()
{
	instance = this;

	if (SDL_NumJoysticks()> 0) {
		joym = SDL_JoystickOpen(0);
	}
}
MenuStage::~MenuStage()
{

}
void MenuStage::init()
{

	game = Game::getInstance();
	
	text = Texture::Load("data/menu/menustart.tga");


	quad.createQuad(game->window_width*0.5, game->window_height*0.5, game->window_width, game->window_height, true);
	camera2D.setOrthographic(0.0 , game->window_width , game->window_height , 0.0 , -1.0,1.0);

	BASS_Init(1, 44100, 0, 0, NULL);
	//Cargamos un sample (memoria, filename, offset, length, max, flags)
	hSample2 = BASS_SampleLoad(false, "data/sounds/musicmenu.wav", 0, 0, 3, 0);
	//Creamos un canal para el sample
	hSampleChannel2 = BASS_SampleGetChannel(hSample2, false);
	
	
	
}

void MenuStage::secondInit()
{
	game = Game::getInstance();

	text = Texture::Load("data/menu/menustart.tga");

	camera2D.setOrthographic(0.0, game->window_width, game->window_height, 0.0, -1.0, 1.0);
	quad.createQuad(game->window_width*0.5, game->window_height*0.5, game->window_width, game->window_height, true);

	BASS_Init(1, 44100, 0, 0, NULL);
	//Cargamos un sample (memoria, filename, offset, length, max, flags)
	hSample2 = BASS_SampleLoad(false, "data/sounds/musicmenu.wav", 0, 0, 3, 0);
	//Creamos un canal para el sample
	hSampleChannel2 = BASS_SampleGetChannel(hSample2, false);



}
void MenuStage::render()
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND); //transparencia
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//set the clear color (the background color)
	glClearColor(0, 0, 0.5, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//game->camera->set();

	camera2D.set();

	glColor4f(1.f, 1.f, 1.f, 1.f);

	text->bind();
	quad.render(GL_TRIANGLES);
	text->unbind();

	BASS_ChannelPlay(hSampleChannel2, false);


}
void MenuStage::update(double dt)
{
	if (joym) {
		JoystickState jst = getJoystickState(joym);

		if (jst.button[BACK_BUTTON] && jst.button[B_BUTTON]) {
			exit(0);
		}
		if (jst.button[A_BUTTON]) {
			BASS_ChannelStop(hSampleChannel2);
			if (GameStage::instance->repeat)
				GameStage::instance->secondinit();
			Stage::instance->current->onChange("gamestage");
		}
		if (jst.button[X_BUTTON]) {
			Stage::instance->current->onChange("controlstage");
		}
	}
}

void MenuStage::onKeyPressed(SDL_KeyboardEvent event)
{
	switch (event.keysym.sym)
	{
	case SDLK_1: {BASS_ChannelStop(hSampleChannel2);
		if(GameStage::instance->repeat)
		GameStage::instance->secondinit();
		Stage::instance->current->onChange("gamestage"); break; 
	}

	case SDLK_2: { Stage::instance->current->onChange("controlstage"); break;
	}
	case SDLK_ESCAPE || SDLK_3: exit(0); break;

	
	}
}
void MenuStage::onMouseButton(SDL_MouseButtonEvent event)
{

	BASS_ChannelStop(hSampleChannel2);
	if (GameStage::instance->repeat)
		GameStage::instance->secondinit();

	std::cout << "kul"<<std::endl;
	Stage::instance->current->onChange("gamestage");
	std::cout << "yeoaas" << std::endl;
}