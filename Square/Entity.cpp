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

void Entity::MoveCameraToThisEntity()
{
	game.camera.x = x;
	game.camera.y = y;
}

Player::Player()
{
	moveRate = game.playerMoveRate;
}

void Player::Draw()
{
	if (tex != nullptr)
		tex->Draw(game.GetRenderer().renderer, rotation, game.GetRenderer().WindowWidth() / 2, game.GetRenderer().WindowHeight() / 2);
}

bool Player::Update()
{
	if (x < 0)
		x = 0;
	else if (x > game.SCREEN_WIDTH - 1)
		x = static_cast <float> (game.SCREEN_WIDTH - 1);

	if (y < 0)
		y = 0;
	else if (y > game.SCREEN_HEIGHT - 1)
		y = static_cast <float> (game.SCREEN_HEIGHT - 1);

	MoveCameraToThisEntity();

	return true;
}

void Player::Respawn()
{
	// TODO: respawn player at maps starting point.
}