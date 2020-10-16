#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>

#include "Entity.h"
#include "Textures.h"
#include "GameEngine.h"

class Map
{
public:
	std::vector<std::vector<bool>> mapData;
	std::string currentMap;

	SDL_Color firstColour { 255, 255, 255, 255 };
	SDL_Color secondColour { 200, 255, 200, 255 };

	void SetMapWidthHeight(int width, int height);

	void DrawMap(Player* player); // Draw overtop of player.
	void DrawMap(); // Draw centered.
	//void RestartMap();
	//void LoadMap(std::string mapName);

	void Cleanup();

private:
	void CreateTextureForMap(int width, int height);
	Texture mapTexture;
};
