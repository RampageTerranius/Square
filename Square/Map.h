#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>

#include "Entity.h"
#include "Textures.h"

enum class DataType
{
	None,
	Null,// Used where its not possible to return the value of a square, like if out of range for example.
	Empty,
	Spawn,
	Exit,
	OneWayLeft,
	OneWayRight,
	OneWayUp,
	OneWayDown
};

class Map
{
public:
	SDL_Color firstColour{ 255, 255, 255, 255 };
	SDL_Color secondColour{ 200, 255, 200, 255 };
	SDL_Color spawnColour{ 100, 100, 200, 255 };
	SDL_Color exitColour{ 0, 100, 200, 255 };

	void SetMapWidthHeight(int newWidth, int newHeight);
	void DrawMap();
	DataType GetMapData(int x, int y);
	DataType GetMapData(Player* player);
	void SetMapData(int x, int y, DataType setting);
	//void RestartMap();
	//void LoadMap(std::string mapName);

	int Width() { return width; }
	int Height() { return height; }

	void Cleanup();

private:
	std::vector<std::vector<DataType>> mapData;
	std::string currentMap;
	void DefaultMapRenderColour(int i, int n);
	void CreateTextureForMap(int width, int height);
	Texture mapTexture;
	int width, height;
};