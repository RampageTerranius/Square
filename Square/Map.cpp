#include "Map.h"
#include "Debug.h"

void Map::Cleanup()
{
	if (mapTexture.HasTexture())
		mapTexture.Clear();
}

void Map::SetMapWidthHeight(int width, int height)
{
	// Setup the width and height for the map.
	mapData.resize(width);
	for (int i = 0; i < width; ++i)
		mapData[i].resize(height);

	CreateTextureForMap(width, height);
}

void Map::DrawMap(Player* player)
{
	mapTexture.Draw(game.GetRenderer().renderer, static_cast<int> (game.GetRenderer().WindowWidth() - round(game.camera.x)), static_cast<int> (game.GetRenderer().WindowHeight() - round(game.camera.y)));
}

void Map::DrawMap()
{
	mapTexture.Draw(game.GetRenderer().renderer, game.GetRenderer().WindowWidth() / 2, game.GetRenderer().WindowHeight() / 2);
}

void Map::CreateTextureForMap(int width, int height)
{
	SDL_Texture* tex = SDL_CreateTexture(game.GetRenderer().renderer, NULL, SDL_TEXTUREACCESS_TARGET, 25 * width, 25 * height);	

	if (SDL_SetRenderTarget(game.GetRenderer().renderer, tex) < 0)
	{
		debug.Log("Map", "CreateTextureForMap", "Failed to set render target: " + (std::string)SDL_GetError());
		return;
	}

	for (int i = 0; i < width; i++)
		for (int n = 0; n < height; n++)
		{
			// Set rect location.
			SDL_Rect rect = { 25 * i, 25 * n, 25, 25 };

			// Set colour.
			if ((i + n) % 2 == 0)
				SDL_SetRenderDrawColor(game.GetRenderer().renderer, firstColour.r, firstColour.g, firstColour.b, firstColour.a);
			else
				SDL_SetRenderDrawColor(game.GetRenderer().renderer, secondColour.r, secondColour.g, secondColour.b, secondColour.a);

			if (SDL_RenderFillRect(game.GetRenderer().renderer, &rect) < 0)
				debug.Log("Map", "CreateTextureForMap", "Failed to fill rect to map texture: " + (std::string)SDL_GetError());			

			SDL_SetRenderDrawColor(game.GetRenderer().renderer, game.GetRenderer().renderColor.r, game.GetRenderer().renderColor.g, game.GetRenderer().renderColor.b, game.GetRenderer().renderColor.a);
		}

	
	mapTexture.SetTexture(tex, "MapTexture");

	SDL_SetRenderTarget(game.GetRenderer().renderer, NULL);
}
