#include "Entity.h"
#include "GameEngine.h"
#include "Debug.h"
#include "Vector2D.h"
#include "Misc Functions.h"

Entity::Entity()
{
	tex = nullptr;
	x = y = 0.0f;
	rotation = 0.0f;
	moveRate = 0.0f;
}

void Entity::Draw()
{
	if (tex != nullptr)
		tex->Draw(game.GetRenderer().renderer, rotation, static_cast <int> (round(x)), static_cast <int> (round(y)));
}

void Entity::Move(Direction dir)
{
	switch (dir)
	{
	case Direction::Up:
		y -= moveRate;
		break;

	case Direction::Down:
		y += moveRate;
		break;

	case Direction::Left:
		x -= moveRate;
		break;

	case Direction::Right:
		x += moveRate;
		break;
	}
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
		// Check if were already in a oneway square.
		if (tempData == DataType::OneWayLeft || tempData == DataType::OneWayDown || tempData == DataType::OneWayRight)
			return;

		y -= moveRate;
		tempData = game.gameData.map.GetMapData(this);

		if (y < 0)		
			y = 0;
		else if (tempData <= DataType::Null || (tempData == DataType::OneWayLeft || tempData == DataType::OneWayDown || tempData == DataType::OneWayRight))
			y = floor(originalY / (double)25) * 25;
		break;

	case Direction::Down:
		// Check if were already in a oneway square.
		if (tempData == DataType::OneWayLeft || tempData == DataType::OneWayUp || tempData == DataType::OneWayRight)
			return;

		y += moveRate;
		tempData = game.gameData.map.GetMapData(this);

		if (y > ((game.gameData.map.Height() * 25) - 1))
			y = (game.gameData.map.Height() * 25) - 1;
		else if (tempData <= DataType::Null || (tempData == DataType::OneWayLeft || tempData == DataType::OneWayUp || tempData == DataType::OneWayRight))
			y = (ceil(originalY / (double)25) * 25) - 1;
		break;

	case Direction::Left:
		// Check if were already in a oneway square.
		if (tempData == DataType::OneWayRight || tempData == DataType::OneWayDown || tempData == DataType::OneWayUp)
			return;

		x -= moveRate;
		tempData = game.gameData.map.GetMapData(this);

		if (x < 0)
			x = 0;
		else if (tempData <= DataType::Null || (tempData == DataType::OneWayRight || tempData == DataType::OneWayDown || tempData == DataType::OneWayUp))
			x = floor(originalX / (double)25) * 25;
		break;

	case Direction::Right:
		// Check if were already in a oneway square.
		if (tempData == DataType::OneWayLeft || tempData == DataType::OneWayDown || tempData == DataType::OneWayUp)
			return;

		x += moveRate;
		tempData = game.gameData.map.GetMapData(this);

		if (x > ((game.gameData.map.Width() * 25) - 1))
			x = (game.gameData.map.Width() * 25) - 1;
		else if (tempData <= DataType::Null || (tempData == DataType::OneWayLeft || tempData == DataType::OneWayDown || tempData == DataType::OneWayUp))
			x = (ceil(originalX / (double)25) * 25) - 1;
		break;
	}

	if (x < 0)
		x = 0;
	else if (x > game.SCREEN_WIDTH - 1)
		x = static_cast <float> (game.SCREEN_WIDTH - 1);

	if (y < 0)
		y = 0;
	else if (y > game.SCREEN_HEIGHT - 1)
		y = static_cast <float> (game.SCREEN_HEIGHT - 1);
}

void Player::Draw()
{
	if (tex != nullptr)
		tex->Draw(game.GetRenderer().renderer, rotation, game.GetRenderer().WindowWidth() / 2, game.GetRenderer().WindowHeight() / 2);
}

bool Player::Update()
{
	MoveCameraToThisEntity();

	//for (Object obj : game.gameData.map.objects)
		

	return true;
}

void Player::Respawn()
{
	if (game.gameData.map.spawnPoint.x != -1 && game.gameData.map.spawnPoint.y != -1)
	{
		x = (game.gameData.map.spawnPoint.x * 25) / 2;
		y = (game.gameData.map.spawnPoint.y * 25) / 2;
	}
	else
	{
		x = 0;
		y = 0;
	}
}

void Object::MoveToCurrentTargetPoint(bool changeToNextPointOnArrival)
{
	if (movePoints.size() > 0)
	{
		// Get current distance for future calculations.
		float distance = sqrt(pow(movePoints[currentTargetPoint].point.x - x, 2) + pow(movePoints[currentTargetPoint].point.y - y, 2));
		float newDistance = sqrt(pow(movePoints[currentTargetPoint].point.x - x, 2) + pow(movePoints[currentTargetPoint].point.y - y, 2));
		if (distance != 0)
		{		
			// Calculate direction and move object.
			float oldXLoc = x;
			float oldYLoc = y;

			Vector2D diffVec(movePoints[currentTargetPoint].point.x - x, movePoints[currentTargetPoint].point.y - y);
			diffVec.Normalize();
			diffVec.Multiply(movePoints[currentTargetPoint].speed);

			x += diffVec.x;
			y += diffVec.y;

			newDistance = sqrt(pow(movePoints[currentTargetPoint].point.x - x, 2) + pow(movePoints[currentTargetPoint].point.y - y, 2));
		}
		// Determine if object has traveled past the target point.
		if (newDistance > distance || distance == 0)
		{
			x = movePoints[currentTargetPoint].point.x;
			y = movePoints[currentTargetPoint].point.y;

			if (changeToNextPointOnArrival)
			{
				currentTargetPoint++;
				if (currentTargetPoint >= movePoints.size())
					currentTargetPoint = 0;
			}
		}
	}
}

void Object::Move()
{
	MoveToCurrentTargetPoint(true);
}

bool Object::Update()
{
	return true;
}

void Object::Draw()
{
	if (tex != nullptr)
	{
		SDL_Point point = GetScreenCoordFromMapPoint({ static_cast<int> (x), static_cast<int> (y) });

		tex->Draw(game.GetRenderer().renderer, rotation, point.x, point.y);
	}
}

void Object::AffectPlayer(Player* player)
{
	effect.Run(player);
}

void Effect::Run(Player* player)
{

}

void Effect_Kill::Run(Player* player)
{
	player->Respawn();
}