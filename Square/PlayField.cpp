#include <SDL.h>

#include "PlayField.h"
#include "EventHandle.h"
#include "GameEngine.h"
#include "Misc Functions.h"
#include "Debug.h"

GameState_PlayField::GameState_PlayField()
{
	Init();
}

void GameState_PlayField::Init()
{
	deaths = 0;

	// Load all the textures the playfield requires.
	background = allTextures.CreateTexture(GetEXEPath() + "\\Images\\background.jpg", "background");
	if (background != nullptr)
	{
		background->anchor = Anchor::TopLeft;
		background->SetWidthHeight(game.SCREEN_WIDTH, game.SCREEN_HEIGHT);
	}

	// Setup player varaibles.	
	player.tex = allTextures.CreateTexture(GetEXEPath() + "\\images\\player.png", "player");
	player.x = 12;
	player.y = 12;

	game.gameData.arrow = allTextures.CreateTexture(GetEXEPath() + "\\images\\arrow.png", "arrow");
	game.gameData.arrow->anchor = Anchor::TopLeft;

	// Setup key bindings.
	iManager.Bind(game.controls.up, commandMoveUp);
	iManager.Bind(game.controls.down, commandMoveDown);
	iManager.Bind(game.controls.left, commandMoveLeft);
	iManager.Bind(game.controls.right, commandMoveRight);
	fps = allTTF.CreateTTF(game.GetRenderer().renderer, "fps");
	fps->SetFont(GetEXEPath() + "\\Fonts\\pxl.ttf", 30);
	fps->SetAnchor(Anchor::TopLeft);
	fps->x = 10;
	fps->y = 10;

	deaths = allTTF.CreateTTF(game.GetRenderer().renderer, "deaths");
	deaths->SetFont(GetEXEPath() + "\\Fonts\\pxl.ttf", 30);
	deaths->SetAnchor(Anchor::TopRight);
	deaths->y = 10;
	deaths->x = game.SCREEN_WIDTH - 10;

	game.gameData.map.SetMapWidthHeight(25, 10);

	for (int i = 0; i < 25; i++)
		for (int n = 0; n < 10; n++)
			game.gameData.map.SetMapData(i, n, DataType::Empty);

	game.gameData.map.SetMapData(5, 6, DataType::None);
	game.gameData.map.SetMapData(5, 5, DataType::None);
	game.gameData.map.SetMapData(5, 4, DataType::None);

	game.gameData.map.SetMapData(0, 0, DataType::Spawn);
	game.gameData.map.SetMapData(20, 8, DataType::Exit);
	game.gameData.map.SetMapData(16, 6, DataType::OneWayUp);
	game.gameData.map.SetMapData(17, 8, DataType::OneWayRight);
	game.gameData.map.SetMapData(16, 9, DataType::OneWayDown);
	game.gameData.map.SetMapData(15, 8, DataType::OneWayLeft);
}

void GameState_PlayField::Cleanup()
{
	allTextures.Cleanup();
	iManager.ClearAll();
	allSounds.Cleanup();

	allTTF.ClearAll();

	delete commandMoveUp;
	delete commandMoveDown;
	delete commandMoveLeft;
	delete commandMoveRight;
}

bool GameState_PlayField::HandleInput()
{
	bool running = iManager.GenerateInputAndDispatchCommands();

	// Process the command list as necessary.
	if (!iManager.ProcessCommandList(&player))
		running = false;
	else
	{
		// Check for other input (non command input).
		// TODO: other input?
	}

	// If the user has signaled to exit we instead want to pop the current state and revert to the last state (generally this means going back to the menu)
	if (!running)
		game.PopLastState();

	return true;
}

void GameState_PlayField::CheckForCollisons()
{
	// TODO: Check collison with objects that kill here.
}

void GameState_PlayField::HandleEvents()
{
	// Handle players events.
	player.Update();

	// TODO: handle all objects here

	CheckForCollisons();
}

void GameState_PlayField::Render()
{
	SDL_RenderClear(game.GetRenderer().renderer);

	fps->SetText(std::to_string(game.fps));
	deaths->SetText(std::to_string(currentDeaths));

	if (background != nullptr)
		background->Draw(game.GetRenderer().renderer, 0, 0);

	game.gameData.map.DrawMap();

	player.Draw();

	// TODO: draw all objects here.

	allTTF.RenderAll();

	SDL_RenderPresent(game.GetRenderer().renderer);
}