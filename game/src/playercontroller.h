#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "includes.h"


class PlayerController
{
public:
	PlayerController();
	~PlayerController();

	bool spr;

	bool useGUI;
	void render(void);
	void update(double seconds_elapsed, int numc);
	void onKeyPressed(SDL_KeyboardEvent event);

};

#endif 