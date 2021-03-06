#include "gamestage.h"
#include "../entitycollider.h"
#include "endstage.h"
#include "../bass.h"
#include <math.h>
#include "firstscreen.h"
#include "menustage.h"
float angle = 0;
Scene* scene = NULL;
Shader * shader = NULL;

int cam_position = 0;
int numcam = 0;

const Uint8* keystate = NULL;
PlayerController* pc = NULL;
GameStage* GameStage::instance = NULL;

int m, s;

//El handler para un sample
HSAMPLE hSample6, hSample7, hSample8, hSample12, hSample13;
SDL_Joystick *joyg = NULL;
//El handler para un canal
HCHANNEL hSampleChannel6, hSampleChannel7, hSampleChannel8, hSampleChannel12, hSampleChannel13;

GameStage::GameStage()
{
	instance = this;
	
	if (SDL_NumJoysticks()> 0) {
		joyg = SDL_JoystickOpen(0);
	}
}

void GameStage::init()
{
	floorok = false;
	ps = false;
	control_camera = false;
	s = 36; // 32s
	m = 3; // 3 min
	n = 1;
	game = Game::getInstance();
	//game->camera->lookAt(Vector3(0.f, 25.f, 25.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	//game->camera->setPerspective(70.f, Game::instance->window_width / (float)Game::instance->window_height, 0.1f, 100000.f); //set the projection, we want to be perspective
	//create our camera
	game->camera = new Camera();
	game->camera->lookAt(Vector3(0, 750, 0), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera
	game->camera->setPerspective(70.f, Game::instance->window_width / (float)Game::instance->window_height, 7.5f, 50000.f); //set the projection, we want to be perspective
			//std::cout << scene->plane->model << std::endl;
			//test = new EntityMesh();
		   //test->config("data/meshes/spitfire/spitfire_color_spec.tga", "data/meshes/spitfire/spitfire.ASE");
	scene = new Scene();
	scene->createScene();

	BulletManager::instance = new BulletManager();

	pc = new PlayerController();

	EndStage::instance->succes = false;

}
void GameStage::secondinit()
{
	floorok = false;
	ps = true;
	control_camera = false;
	game = Game::getInstance();
	m = 3;
	s = 36;
	n = 1;


	game->camera = new Camera();
	game->camera->lookAt(Vector3(0, 750, 0), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera
	game->camera->setPerspective(70.f, Game::instance->window_width / (float)Game::instance->window_height, 0.1f, 100000.f); //set the projection, we want to be perspective
	//std::cout << scene->plane->model << std::endl;
	//test = new EntityMesh();
	//test->config("data/meshes/spitfire/spitfire_color_spec.tga", "data/meshes/spitfire/spitfire.ASE");
	scene = new Scene();
	scene->createScene();

	BulletManager::instance = new BulletManager();

	pc = new PlayerController();

	EndStage::instance->succes = false;

	BASS_Init(1, 44100, 0, 0, NULL);
	
	//Cargamos un sample (memoria, filename, offset, length, max, flags)
	hSample6 = BASS_SampleLoad(false, "data/sounds/planesound.wav", 0, 0, 3, 0);
	hSample7 = BASS_SampleLoad(false, "data/sounds/tensionmid.wav", 0, 0, 3, 0);
	hSample8 = BASS_SampleLoad(false, "data/sounds/tensionlast.wav", 0, 0, 3, 0);
	hSample12 = BASS_SampleLoad(false, "data/sounds/alarm.wav", 0, 0, 3, 0);
	hSample13 = BASS_SampleLoad(false, "data/sounds/writer.wav", 0, 0, 3, 0);
	//Creamos un canal para el sample


	hSampleChannel6 = BASS_SampleGetChannel(hSample6, false);
	hSampleChannel7 = BASS_SampleGetChannel(hSample7, false);
	hSampleChannel8 = BASS_SampleGetChannel(hSample8, false);
	hSampleChannel12 = BASS_SampleGetChannel(hSample12, false);
	hSampleChannel13 = BASS_SampleGetChannel(hSample13, false);

	BASS_ChannelSetAttribute(hSampleChannel7, BASS_ATTRIB_VOL, 0.4);
	BASS_ChannelSetAttribute(hSampleChannel6, BASS_ATTRIB_VOL, 0.5);

	BASS_ChannelSetAttribute(hSampleChannel12, BASS_ATTRIB_VOL, 0.6);

	BASS_ChannelSetAttribute(hSampleChannel13, BASS_ATTRIB_VOL, 1.5);
}

void GameStage::render()
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//set the clear color (the background color)
	//glClearColor(61.0/256.0, 96.0/256.0, 147.0/256.0, 1.0);
	glClearColor(71.0 / 256.0, 101.0 / 256.0, 113.0 / 256.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Put the camera matrices on the stack of OpenGL (only for fixed rendering)
	game->camera->set();

	//Draw out world
	//drawGrid(500); //background grid
	
	glDisable(GL_DEPTH_TEST);
	scene->cielo->model.setIdentity();
	scene->cielo->model.traslate(game->camera->eye.x, game->camera->eye.y, game->camera->eye.z);
	scene->cielo->render();
	glEnable(GL_DEPTH_TEST);
	


	
	
	/*scene->water->model.setIdentity();
	scene->water->model.traslate(game->camera->eye.x, 0, game->camera->eye.z);
	scene->water->render(game->camera, shader);*/

	scene->root->render();

	if (ps) {
		BASS_ChannelPlay(hSampleChannel6, false);
		if (pc->spr) {
			BASS_ChannelSetAttribute(hSampleChannel6, BASS_ATTRIB_VOL, 0.8);
		}
		else
			BASS_ChannelSetAttribute(hSampleChannel6, BASS_ATTRIB_VOL, 0.5);
		if (Scene::instance->barco->musicon) {
			BASS_ChannelPlay(hSampleChannel8, false);
			BASS_ChannelStop(hSampleChannel7);
		}
		else {
			BASS_ChannelPlay(hSampleChannel7, false);
		}
	}

	//camera pos
	//std::cout << camera->eye.x << " - " << camera->eye.y << " - " << camera->eye.z << std::endl; 
	
	BulletManager::instance->render();

	if (pc->useGUI)
		renderGUI();

	std::stringstream vida;
	vida << "HP : " << scene->plane->hp << "%";
	drawText(game->window_width * 0.05, game->window_height * 0.9, vida.str(), Vector3(1, 0.01*scene->plane->hp, 0.01*scene->plane->hp), 2);



	std::stringstream coord;
	coord << "Coord : (" << game->camera->eye.x << "," << game->camera->eye.y << "," << game->camera->eye.z << ")";
	drawText(game->window_width * 0.7, game->window_height * 0.9, coord.str(), Vector3(1, 1, 1), 2);


	std::stringstream vidabarco;
	vidabarco << "Enemy boat : " << scene->barco->hp << "%";
	drawText(game->window_width * 0.05, game->window_height * 0.85, vidabarco.str(), Vector3(1, 0.0025*scene->barco->hp, 0.0025*scene->barco->hp), 2);




	

	
	if (s < 0.0) {
		s = 59.0;
		m -= 1.0;
	}
	//if (s == 9.0) s = 09.0; if (s == 8.0) s = 08.0; if (s == 7.0) s = 07.0; if (s == 6.0) s = 06.0; if (s == 5.0) s = 05.0; if (s == 4.0) s = 04.0; if (s == 3.0) s = 03.0; if (s == 2.0) s = 02.0; if (s == 1.0) s = 01.0; if (s == 0.0) s = 00.0;

	float elap = m * 60.0 + s;
	std::stringstream time;

	if (s >= 10.0)
		time << "Time remaining : 0"<<m << ":" << (int)s << " min";
	else

		time << "Time remaining : 0" << m << ":0" << (int)s << " min";
	drawText(game->window_width * 0.05, game->window_height * 0.1, time.str(), Vector3(1, 0.005*elap, 0.005*elap),2);

	std::stringstream score;
	score << "Score : " << scene->plane->getScore() << " points";
	drawText(game->window_width * 0.05, game->window_height * 0.05, score.str(), Vector3(1, 1, 1), 2);

	if (m >= 0.0 &&  m < 1.0 && s <= 10.0) {
		BASS_ChannelPlay(hSampleChannel12, false);
	}

	if (m >= 3.00 && s > 17.0) {

		BASS_ChannelPlay(hSampleChannel13, false);
		std::stringstream msg;
		msg << "Hey soldier ! Your mission today is to defend" << std::endl;
		msg << "us from terrorists !  There is one boat fully" << std::endl;
		msg << "charged of bombs which is coming to our carrier!"<< std::endl;
		msg << "Find it and destroy it as fast as you can! " << std::endl;
		msg << "Go carefully, there are a lot of enemies defending him !" << std::endl;
		msg << "You have less than 4 minutes ... GOOD LUCK" << std::endl;
		drawText(game->window_width * 0.25, game->window_height * 0.275, msg.str(), Vector3(1, 1, 1), 2);
	}
	else
		BASS_ChannelStop(hSampleChannel13);
}

void GameStage::renderGUI()
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND); //transparencia
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	Camera cam2D;
	cam2D.setOrthographic(0, Game::instance->window_width, Game::instance->window_height, 0, -1, 1);
	cam2D.set();
	
	Mesh quad;
	Texture* cross = Texture::Load("data/meshes/crosshair.TGA");
	cross->bind();
	quad.createQuad(Game::instance->window_width*0.5, Game::instance->window_height*0.5, 50, 50);
	quad.render(GL_TRIANGLES);
	cross->unbind();
	
}

void GameStage::update(double seconds_elapsed)
{
	keystate = Game::instance->keystate;
	float speed = seconds_elapsed * 100; //the speed is defined by the seconds_elapsed so it goes constant

										 //mouse input to rotate the cam
	if ((Game::instance->mouse_state & SDL_BUTTON_LEFT) || Game::instance->mouse_locked) //is left button pressed?
	{
		game->camera->rotate(Game::instance->mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
		game->camera->rotate(Game::instance->mouse_delta.y * 0.005f, game->camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
	}

	//async input to move the camera around
	if (control_camera)
	{
		if (keystate[SDL_SCANCODE_LSHIFT]) speed *= 100; //move faster with left shift
		if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) game->camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
		if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) game->camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
		if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT]) game->camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
		if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT]) game->camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);
	}
	else
	{
		//Vector3 lastpos = scene->plane->model * Vector3(1, 1, 1);

		BulletManager::instance->update(seconds_elapsed);
		pc->update(seconds_elapsed, numcam);

		//scene->plane->update(seconds_elapsed);

		
	}

	scene->plane->model.traslateLocal(0, 0, seconds_elapsed * 10);

	scene->root->update(seconds_elapsed);
	/*scene->plane->update(seconds_elapsed);
	scene->p38->update(seconds_elapsed);
	scene->p38a->update(seconds_elapsed);
	scene->bomber->update(seconds_elapsed);
	scene->barco->update(seconds_elapsed);*/

	//to navigate with the mouse fixed in the middle
	if (Game::instance->mouse_locked)
	{
		int center_x = (int)floor(Game::instance->window_width*0.5f);

		int center_y = (int)floor(Game::instance->window_height*0.5f);
		//center_x = center_y = 50;
		SDL_WarpMouseInWindow(Game::instance->window, center_x, center_y); //put the mouse back in the middle of the screen
																		   //SDL_WarpMouseGlobal(center_x, center_y); //put the mouse back in the middle of the screen

		Game::instance->mouse_position.x = (float)center_x;
		Game::instance->mouse_position.y = (float)center_y;
	}


	angle += (float)seconds_elapsed * 10.0f;


	/***MOVIMIENTO AVIONES***/
	/*
	scene->p38->model.traslateLocal(0, 0, seconds_elapsed * 10 * speed);
	scene->p38a->model.traslateLocal(0, 0, seconds_elapsed * 10 * speed);
	scene->bomber->model.traslateLocal(0, 0, seconds_elapsed * 10 * speed);
	*/
	

	s -= seconds_elapsed;
	
	if (joyg) {
		JoystickState jst = getJoystickState(joyg);

		if (jst.button[BACK_BUTTON]) {
			EntityCollider::deleteAllColliders();
			Entity::toDestroy.push_back(Scene::instance->root);
			Entity::toDestroy.push_back(Scene::instance->cielo);
			scene->plane->deleteEntity();
			GameStage::instance->repeat = true;
			Stage::instance->current->onChange("menustate");
			stopMusic();
		}
		if (jst.button[Y_BUTTON]) {
			numcam += 1;
		}
	}

}
void GameStage::stopMusic()
{
	BASS_ChannelStop(hSampleChannel6);
	BASS_ChannelStop(hSampleChannel7);
	BASS_ChannelStop(hSampleChannel8);
	BASS_ChannelStop(hSampleChannel12);
	BASS_ChannelStop(hSampleChannel13);
}
void GameStage::onKeyPressed(SDL_KeyboardEvent event)
{
	switch (event.keysym.sym)
	{
	case SDLK_z: {numcam += 1; break; } //change camera player mode
	case SDLK_ESCAPE: {EntityCollider::deleteAllColliders();
		Entity::toDestroy.push_back(Scene::instance->root);
		Entity::toDestroy.push_back(Scene::instance->cielo);
		scene->plane->deleteEntity();
		GameStage::instance->repeat = true;
		Stage::instance->current->onChange("menustate");
		stopMusic();
		MenuStage::instance->secondInit(); break;}
					  //ESC key, kill the app
	case SDLK_TAB: { control_camera = !control_camera; break;}
				   //case SDLK_SPACE: bm->shoot(camera->eye, Vector3(10,10,10), 1.00 ,5.00, scene->plane, 1);
	 /*case SDLK_p: {scene->plane->hp = scene->plane->hp - 5;}*/
			   
	}
}
void GameStage::onMouseButton(SDL_MouseButtonEvent event)
{

}
