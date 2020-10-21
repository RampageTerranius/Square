#pragma once

#include <SDL.h>
#include <vector>
#include <string>
#include "GameState.h"
#include "GameData.h"
#include "Renderer.h"
#include "EventHandle.h"

class GameEngine
{
public:
	GameEngine();
	// Startup and shutdown of engine.
	void Init();
	void Cleanup();

	// Loop management.
	void HandleInput();
	void HandleEvents();
	void Render();

	// State management.
	bool HasActiveState();
	GameState* State() { return states.back(); }
	void PushNewState(GameState* state);
	void PopLastState();

	// Global renderer for engine.
	Renderer GetRenderer() { return renderer; }

	bool running = false;

	int fps;

	// Game related variables.
	std::string SCREEN_NAME = "Square";
	int SCREEN_WIDTH = 1024;
	int SCREEN_HEIGHT = 768;
	Uint32 FRAME_RATE = 60;

	float playerMoveRate = 2;

	SDL_Rect camera;

	GameData gameData;

	class Controls
	{
	public:
		int up = SDLK_w;
		int down = SDLK_s;
		int left = SDLK_a;
		int right = SDLK_d;
		int respawn = SDLK_SPACE;
	}controls;

private:
	void LoadSettings();
	bool FULLSCREEN = false;
	bool VSYNC = false;

	Renderer renderer = Renderer();
	std::vector<GameState*> states;
};

extern GameEngine game;