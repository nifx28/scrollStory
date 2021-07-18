#pragma once

#include "actorBehavior.h"

class Actor
{
public:
	enum DIRECTION { None = -1, Left, Right, Up, Down };

	Actor(int width, int height, int miniWidth, int miniHeight, int x, int y, int dir = DIRECTION::Right, bool speedUp = false);
	~Actor();

	void setMiniMap(IMAGE img);
	void setBackground(IMAGE img);
	void setActor(ActorBehavior* pActor);

	void playScene(int moveDir = -1, bool speedUp = false);
	int getDirection();

protected:
	int width, height;
	int miniWidth, miniHeight;
	int x, y;
	int moveDir;
	bool speedUp;

	IMAGE miniMap, background;
	ActorBehavior* pActor;

	void setMoveDir(int moveDir, bool speedUp);

	std::atomic<bool> ready{ true };
	void playSceneAsync();
};
