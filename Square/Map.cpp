#include "Map.h"
#include "Debug.h"
#include "GameEngine.h"

void Map::Cleanup()
{
	if (mapTexture.HasTexture())
		mapTexture.Clear();
}

void Map::SetMapWidthHeight(int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;

	// Setup the width and height for the map.
	mapData.resize(width);
	for (int i = 0; i < width; ++i)
		mapData[i].resize(height);	

	CreateTextureForMap(width, height);
}

void Map::DrawMap()
{
	mapTexture.Draw(game.GetRenderer().renderer, (game.GetRenderer().WindowWidth() / 2) - game.camera.x, (game.GetRenderer().WindowHeight() / 2) - game.camera.y);
}

void Map::DrawObjects()
{
	for (Object obj : objects)
		obj.Draw();
}

void Map::SetMapData(int x, int y, DataType setting)
{
	if ((x < width && x >= 0) && (y < height && y >= 0))
	{
		mapData[x][y] = setting;
		CreateTextureForMap(width, height);
	}
	else
		debug.Log("Map", "SetMapData", "Attempt to set data from location that is out of map range");
}

DataType Map::GetMapData(int x, int y)
{
	if ((x < width && x >= 0) && (y < height && y >= 0))
		return mapData[x][y];

	debug.Log("Map", "GetMapData", "Attempt to get data from location that is out of map range");
	return DataType::Null;
}

DataType Map::GetMapData(Player* player)
{
	int x = floor(player->x / 25);
	int y = floor(player->y / 25);

	if ((x < width && x >= 0) && (y < height && y >= 0))
		return mapData[x][y];

	debug.Log("Map", "GetMapData", "Attempt to get data from location that is out of map range");
	return DataType::Null;
}

void Map::DefaultMapRenderColour(int i, int n, SDL_Rect& rect)
{
	if ((i + n) % 2 == 0)
		SDL_SetRenderDrawColor(game.GetRenderer().renderer, firstColour.r, firstColour.g, firstColour.b, firstColour.a);
	else
		SDL_SetRenderDrawColor(game.GetRenderer().renderer, secondColour.r, secondColour.g, secondColour.b, secondColour.a);

	if (SDL_RenderFillRect(game.GetRenderer().renderer, &rect) < 0)
		debug.Log("Map", "CreateTextureForMap", "Failed to fill rect to map texture: " + (std::string)SDL_GetError());

	SDL_SetRenderDrawColor(game.GetRenderer().renderer, game.GetRenderer().renderColor.r, game.GetRenderer().renderColor.g, game.GetRenderer().renderColor.b, game.GetRenderer().renderColor.a);
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
			if (mapData[i][n] > DataType::Null)
			{
				switch (mapData[i][n])
				{	
				case DataType::Empty:
					DefaultMapRenderColour(i, n, rect);
					break;

				case DataType::Spawn:
					SDL_SetRenderDrawColor(game.GetRenderer().renderer, spawnColour.r, spawnColour.g, spawnColour.b, spawnColour.a);

					if (SDL_RenderFillRect(game.GetRenderer().renderer, &rect) < 0)
						debug.Log("Map", "CreateTextureForMap", "Failed to fill rect to map texture: " + (std::string)SDL_GetError());

					SDL_SetRenderDrawColor(game.GetRenderer().renderer, game.GetRenderer().renderColor.r, game.GetRenderer().renderColor.g, game.GetRenderer().renderColor.b, game.GetRenderer().renderColor.a);

					break;

				case DataType::Exit:
					SDL_SetRenderDrawColor(game.GetRenderer().renderer, exitColour.r, exitColour.g, exitColour.b, exitColour.a);

					if (SDL_RenderFillRect(game.GetRenderer().renderer, &rect) < 0)
						debug.Log("Map", "CreateTextureForMap", "Failed to fill rect to map texture: " + (std::string)SDL_GetError());

					SDL_SetRenderDrawColor(game.GetRenderer().renderer, game.GetRenderer().renderColor.r, game.GetRenderer().renderColor.g, game.GetRenderer().renderColor.b, game.GetRenderer().renderColor.a);

					break;

				case DataType::OneWayUp:
					DefaultMapRenderColour(i, n, rect);

					game.gameData.arrow->Draw(game.GetRenderer().renderer, 0, 25 * i, 25 * n);
					break;

				case DataType::OneWayDown:
					DefaultMapRenderColour(i, n, rect);

					game.gameData.arrow->Draw(game.GetRenderer().renderer, 180, (25 * i) + 1, (25 * n) + 1);
					break;

				case DataType::OneWayLeft:
					DefaultMapRenderColour(i, n, rect);

					game.gameData.arrow->Draw(game.GetRenderer().renderer, 270, 25 * i, (25 * n) + 1);
					break;

				case DataType::OneWayRight:
					DefaultMapRenderColour(i, n, rect);

					game.gameData.arrow->Draw(game.GetRenderer().renderer, 90, (25 * i) + 1, 25 * n);
					break;
				}	
			}
		}
	
	mapTexture.SetTexture(tex, "MapTexture");
	mapTexture.anchor = Anchor::TopLeft;

	SDL_SetRenderTarget(game.GetRenderer().renderer, NULL);
}

void Map::RunObjectEvents()
{
	for (Object& obj : objects)
		obj.MoveToCurrentTargetPoint(true);
}