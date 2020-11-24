#pragma once
#include <vector>

#include "Textures.h"
#include "Audio.h"

enum class Direction
{
	None,
	Up,
	Down,
	Left,
	Right
};

class Entity
{
public:
	Entity();
	virtual void Draw();
	virtual bool Update() = 0;
	virtual void Move(Direction dir);
	void MoveCameraToThisEntity();

	Texture* tex;
	float x, y;
	float rotation;
	float moveRate;
};

class Player : public Entity
{
public:
	Player();
	bool Update();
	void Respawn();
	void Draw();
	void Move(Direction dir);	
};

class MovePoint
{
public:
	SDL_Point point;
	float speed;
};

class Effect
{
public:
	virtual void Run(Player* player);
};

class Effect_Kill : Effect
{
	void Run(Player* player);
};

class Object : public Entity
{
public:
	bool Update();
	void Move();
	void Draw();
	void MoveToCurrentTargetPoint(bool changeToNextPointOnArrival);
	void AffectPlayer(Player* player);
	std::vector<MovePoint> movePoints;

private:	
	int currentTargetPoint = 0;
	Effect effect;
};

