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
	allTextures.CreateTexture(GetEXEPath() + "\\Images\\Bullet.png", "bullet");
	allTextures.CreateTexture(GetEXEPath() + "\\Images\\asteroid 10x10.png", "asteroid 10");
	allTextures.CreateTexture(GetEXEPath() + "\\Images\\asteroid 15x15.png", "asteroid 15");
	allTextures.CreateTexture(GetEXEPath() + "\\Images\\asteroid 20x20.png", "asteroid 20");
	allTextures.CreateTexture(GetEXEPath() + "\\Images\\asteroid 25x25.png", "asteroid 25");

	background = allTextures.CreateTexture(GetEXEPath() + "\\Images\\background.jpg", "background");
	background->anchor = Anchor::TopLeft;
	background->SetWidthHeight(game.SCREEN_WIDTH, game.SCREEN_HEIGHT);

	allSounds.CreateChunk(GetEXEPath() + "\\Sounds\\8-bit-explosion2.aiff", "explosion");
	allSounds.CreateChunk(GetEXEPath() + "\\Sounds\\8-bit-bump.aiff", "hit");
	allSounds.CreateChunk(GetEXEPath() + "\\Sounds\\8-bit-laser1.aiff", "shot");

	// Setup player varaibles.	
	player.tex = allTextures.CreateTexture(GetEXEPath() + "\\images\\player.png", "player");
	player.x = static_cast <float> (game.SCREEN_WIDTH) / 2.0f;
	player.y = static_cast <float> (game.SCREEN_HEIGHT) / 2.0f;

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
		// Check for other input.
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

	background->Draw(game.GetRenderer().renderer, 0, 0);

	// TODO: draw the map here.

	player.Draw();

	// TODO: draw all objects here.

	allTTF.RenderAll();

	SDL_RenderPresent(game.GetRenderer().renderer);
}