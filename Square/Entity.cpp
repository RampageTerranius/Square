#include "Entity.h"
#include "GameEngine.h"
#include "Debug.h"
#include "Misc Functions.h"

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

void Player::Move(Direction dir)
{
	int originalX = x;
	int originalY = y;

	DataType tempData = game.gameData.map.GetMapData(this);

	switch (dir)
	{
	case Direction::Up:
		if (tempData == DataType::OneWayLeft || tempData == DataType::OneWayDown || tempData == DataType::OneWayRight)
			return;

		y -= moveRate;
		if (y < 0)		
			y = 0;
		else if (tempData <= DataType::Null)
			y = floor(originalY / (double)25) * 25;
		break;

	case Direction::Down:
		if (tempData == DataType::OneWayLeft || tempData == DataType::OneWayUp || tempData == DataType::OneWayRight)
			return;

		y += moveRate;
		if (y > ((game.gameData.map.Height() * 25) - 1))
			y = (game.gameData.map.Height() * 25) - 1;
		else if (tempData <= DataType::Null)
			y = (ceil(originalY / (double)25) * 25) - 1;
		break;

	case Direction::Left:
		if (tempData == DataType::OneWayUp || tempData == DataType::OneWayDown || tempData == DataType::OneWayRight)
			return;

		x -= moveRate;
		if (x < 0)
			x = 0;
		else if (tempData <= DataType::Null)
			x = floor(originalX / (double)25) * 25;
		break;

	case Direction::Right:			
		if (tempData == DataType::OneWayLeft || tempData == DataType::OneWayDown || tempData == DataType::OneWayUp)
			return;

		x += moveRate;

		if (x > ((game.gameData.map.Width() * 25) - 1))
			x = (game.gameData.map.Width() * 25) - 1;
		else if (tempData <= DataType::Null)
			x = (ceil(originalX / (double)25) * 25) - 1;
		break;
	}
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