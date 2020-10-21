#pragma once
#include "GameState.h"
#include "Entity.h"
#include "Command.h"

class CommandMoveLeft : public Command
{
public:
	CommandMoveLeft() { allowContinuousExecution = true; }
	bool Execute(Player* player)
	{
		player->Move(Direction::Left);
		return true;
	}
};

class CommandMoveRight : public Command
{
public:
	CommandMoveRight() { allowContinuousExecution = true; }
	bool Execute(Player* player)
	{
		player->Move(Direction::Right);
		return true;
	}
};

class CommandMoveUp : public Command
{
public:
	CommandMoveUp() { allowContinuousExecution = true; }
	bool Execute(Player* player)
	{
		player->Move(Direction::Up);
		return true;
	}
};

class CommandMoveDown : public Command
{
public:
	CommandMoveDown() { allowContinuousExecution = true; }
	bool Execute(Player* player)
	{
		player->Move(Direction::Down);
		return true;
	}
};

class GameState_PlayField : public GameState
{
public:
	GameState_PlayField();

	void Init();
	void Cleanup();

	bool HandleInput();
	void HandleEvents();
	void Render();

	int currentDeaths;	

private:
	Player player;
	TTFs allTTF;

	void CheckForCollisons();

	Texture* background;

	TTF* fps;
	TTF* deaths;

	Command* commandMoveUp = new CommandMoveUp();
	Command* commandMoveDown = new CommandMoveDown();
	Command* commandMoveLeft = new CommandMoveLeft();
	Command* commandMoveRight = new CommandMoveRight();
};