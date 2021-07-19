#pragma once

#include "actorBehavior.h"
#include "actor.h"

#define AVATAR_TILE_PATH	L"assets\\avatars\\pnghut_maplestory-2-video-game-sprite-monster-inc.png"

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

	IMAGE avatar, mask;
	IMAGE mkIMG[2];
	IMAGE bkIMG[2];

	int imgGridX[25] = {
		  0, 130, 265, 408, 544,
		  0, 130, 265, 408, 544,
		  0, 130, 265, 408, 544,
		  0, 130, 265, 408, 544,
		  0, 144, 296, 436, 582
	};

	int imgGridY[25] = {
		 -6,  -6,  -6,  -6,  -6,
		120, 120, 120, 120, 120,
		260, 260, 260, 260, 260,
		400, 400, 400, 400, 400,
		550, 550, 550, 550, 550
	};

	int imgGridW[25] = {
		 30,  30,  30,  30,  30,
		140, 140, 140, 140, 140,
		160, 160, 160, 160, 160,
		160, 160, 160, 160, 160,
		160, 160, 160, 160, 160
	};

	int imgGridZ[25] = {
		126, 126, 126, 126, 126,
		143, 143, 143, 143, 143,
		140, 140, 140, 140, 140,
		153, 153, 153, 153, 153,
		146, 146, 146, 146, 146
	};
};
