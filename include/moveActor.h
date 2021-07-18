#pragma once

#include "actorBehavior.h"
#include "actor.h"

class MoveActor : public ActorBehavior
{
public:
	MoveActor(int maxFrameNum, const wchar_t* mkIMGURL[], const wchar_t* bkIMGURL[]);

	void setGroundZero(int y);
	void setDirection(int dir);
	void drawScene(int width, int height, int miniWidth, int miniHeight, int& x, int& y, int dir, bool speedUp, const IMAGE& miniMap, const IMAGE& background);

protected:
	int groundZero, jumpHeight;
	int moveDir, maxFrameNum;

	bool isJumping, isFalling;

	IMAGE avatar;
	IMAGE mkIMG[2];
	IMAGE bkIMG[2];

	int imgGridX[25] = {
		  0, 130, 260, 390, 520,
		  0, 130, 265, 408, 544,
		  0, 130, 260, 390, 520,
		  0, 130, 260, 390, 520,
		  0, 144, 296, 436, 582
	};

	int imgGridY[25] = {
		-10,   0,   0,   0,   0,
		130, 130, 130, 127, 126,
		266, 260, 260, 260, 260,
		400, 390, 390, 390, 390,
		566, 560, 560, 557, 560
	};
};
