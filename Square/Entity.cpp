#include "Entity.h"
#include "GameEngine.h"
#include "Debug.h"
#include "Misc Functions.h"
#include <math.h>

Entity::Entity()
{
	tex = nullptr;
	x = y = 0.0f;
	rotation = 0.0f;
}

void Entity::Draw()
{
	if (tex != nullptr)
		tex->Draw(game.GetRenderer().renderer, rotation, static_cast <int> (round(x)), static_cast <int> (round(y)));
}

Player::Player()
{
	moveRate = game.playerMoveRate;
}

bool Player::Update()
{
	if (x < 0)
		x = static_cast <float> (game.SCREEN_WIDTH - 1);
	else if (x > game.SCREEN_WIDTH)
		x = 0;

	if (y < 0)
		y = static_cast <float> (game.SCREEN_HEIGHT - 1);
	else if (y > game.SCREEN_HEIGHT)
		y = 0;

	return true;
}

void Player::Respawn()
{
	// TODO: respawn player at maps starting point.
}