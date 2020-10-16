#pragma once
#include <list>

#include "Textures.h"
#include "Audio.h"

class Entity
{
public:
	Entity();
	virtual void Draw();
	virtual bool Update() = 0;
	void MoveCameraToThisEntity();

	Texture* tex;
	float x, y;
	float rotation;
};

class Player : public Entity
{
public:
	Player();
	bool Update();
	void Respawn();
	void Draw();

	float moveRate;
};