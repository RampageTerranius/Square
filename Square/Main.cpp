#include "GameEngine.h"
#include "Timer.h"
#include "Debug.h"

int main(int argc, char* argv[])
{
	Timer perFrametimer;
	Timer lastTickTimer;

	if (!game.HasActiveState())
	{
		debug.Log("Main", "main", "Game engine has no active state!");
		game.running = false;
	}

	lastTickTimer.Start();

	Uint32 lastTickTime = lastTickTimer.GetTicks();

	int framesCounted = 0;

	while (game.running)
	{
		perFrametimer.Start();

		game.HandleInput();
		game.HandleEvents();
		game.Render();

		Uint32 timerFps = perFrametimer.GetTicks();
		framesCounted++;

		if (timerFps < (1000u / game.FRAME_RATE))
			SDL_Delay((1000u / game.FRAME_RATE) - timerFps);

		if (lastTickTimer.GetTicks() - lastTickTime >= 1000)
		{
			game.fps = framesCounted;
			framesCounted = 0;
			lastTickTime = lastTickTimer.GetTicks();
		}
	}

	game.Cleanup();

	return 0;
}