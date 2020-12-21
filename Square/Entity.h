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
	Effect(float* xVar, float* yVar) { x = xVar; y = yVar; }
	float* x;
	float* y;
	virtual void Run(Player* player);
};

class Effect_Kill : public Effect
{
public:
	Effect_Kill(float* xVar, float* yVar) : Effect(xVar, yVar) {};
	void Run(Player* player);
};

class Effect_Repel : Effect
{
public:
	Effect_Repel(float* xVar, float* yVar) : Effect(xVar, yVar) {};
	void Run(Player* player);
	float repelStrength;
	float repelDistance;
	float distanceDivisor;
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
	Effect* effect;
};