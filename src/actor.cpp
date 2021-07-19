#include "stdafx.h"
#include "actor.h"

using namespace std;

Actor::Actor(int width, int height, int miniWidth, int miniHeight, int x, int y, int dir, bool speedUp) :
	width(width), height(height),
	miniWidth(miniWidth), miniHeight(miniHeight),
	x(x), y(y),
	moveDir(dir),
	speedUp(speedUp),
	pActor(nullptr)
{
}

Actor::~Actor()
{
	if (pActor)
	{
		delete pActor;
	}
}

void Actor::setMiniMap(IMAGE img)
{
	miniMap = img;
}

void Actor::setBackground(IMAGE img)
{
	background = img;
}

void Actor::setActor(ActorBehavior* pActor)
{
	this->pActor = pActor;
	this->pActor->setGroundZero(y);
	this->pActor->setDirection(moveDir);
}

void Actor::setMoveDir(int moveDir, bool speedUp)
{
	this->moveDir = moveDir;
	this->speedUp = speedUp;
}

void Actor::playSceneAsync()
{
	pActor->drawScene(width, height, miniWidth, miniHeight, x, y, moveDir, speedUp, miniMap, background);
}

void Actor::playScene(int moveDir, bool speedUp)
{
	setMoveDir(moveDir, speedUp);

	if (ready.load(std::memory_order_acquire))
	{
		ready.store(false, memory_order_release);

		[[maybe_unused]] auto a1 = async(launch::async,
			[&]() {
				playSceneAsync();
				ready.store(true, memory_order_release);
			}
		);
	}
}

int Actor::getDirection()
{
	return moveDir;
}
