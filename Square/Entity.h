#pragma once
#include <list>

#include "Textures.h"
#include "Audio.h"

class Entity
{
public:
	Entity();
	void Draw();
	virtual bool Update() = 0;

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

	float moveRate;
};