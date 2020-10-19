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
	SDL_Color firstColour { 255, 255, 255, 255 };
	SDL_Color secondColour { 200, 255, 200, 255 };

	void SetMapWidthHeight(int newWidth, int newHeight);
	void DrawMap();
	bool GetMapData(int x, int y);
	void SetMapData(int x, int y, bool setting);
	//void RestartMap();
	//void LoadMap(std::string mapName);

	void Cleanup();

private:
	std::vector<std::vector<bool>> mapData;
	std::string currentMap;
	void CreateTextureForMap(int width, int height);
	Texture mapTexture;
	int width, height;
};
