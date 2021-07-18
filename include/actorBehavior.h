#pragma once

#include <graphics.h>

class ActorBehavior
{
public:
	virtual ~ActorBehavior() = 0;

	virtual void setGroundZero(int y) = 0;
	virtual void setDirection(int dir) = 0;
	virtual void drawScene(int width, int height, int miniWidth, int miniHeight, int& x, int& y, int dir, bool speedUp, const IMAGE& miniMap, const IMAGE& background) = 0;
};
