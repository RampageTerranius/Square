#pragma once

#include "Map.h"
#include "Textures.h"
#include "Entity.h"

class GameData
{
public:
	Map map;	
	Texture* arrowTex;
	Texture* playerTex;
	SDL_Rect camera;
};