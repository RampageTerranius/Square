#include "Misc Functions.h"
#include "GameEngine.h"

#include <Windows.h>
#include <PathCch.h>
#include <SDL.h>

// Gets the full path of the EXE file (minus the exe name).
std::string GetEXEPath()
{
	char* exePath;

	exePath = SDL_GetBasePath();
	std::string str = exePath;
	SDL_free(exePath);

	return str;
}

float GetDistance(float x1, float y1, float x2, float y2)
{
	return static_cast <float> (sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)));
}

// Gets the X/Y coordinate on the screen where the given point would be.
// This does not factor for the coord potentially being off the screen, it is up to the user to determine this.
SDL_Point GetScreenCoordFromMapPoint(SDL_Point point)
{
	SDL_Point mapCoord = { 0, 0 };

	// Convert mouse and camera into the location on the map we are pointing at.
	mapCoord.x = (point.x - game.camera.x) + (game.GetRenderer().WindowWidth() / 2);
	mapCoord.y = (point.y - game.camera.y) + (game.GetRenderer().WindowHeight() / 2);

	return mapCoord;
}