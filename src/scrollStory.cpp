#include "stdafx.h"
#include "scrollStory.h"
#include "actor.h"
#include "moveActor.h"

using namespace std;

int main()
{
	int width = 1440, height = width / 16 * 9;
	int bgWidth = 900, bgHeight = 271;
	int miniWidth = bgWidth / 2, miniHeight = bgHeight / 2;

	HWND hWnd = nullptr;
	hWnd = initgraph(width, height); // , EW_SHOWCONSOLE

	if (hWnd)
	{
		wostringstream woss;
		woss << L"EasyX 遊戲開發 (" << width << L" x " << height << L")";
		//SetConsoleTitle(woss.str().data());
		SetWindowText(hWnd, woss.str().data());
	}

	IMAGE miniMap, background;
	loadimage(&miniMap, BACKGROUND_PATH, miniWidth, miniHeight);
	loadimage(&background, BACKGROUND_PATH, width, height);

	Actor* pActor = new Actor(width, height, miniWidth, miniHeight, 1000, 605);
	pActor->setMiniMap(miniMap);
	pActor->setBackground(background);

	const wchar_t* mkIMGURL[2] = { AVATAR_LEFT_PATH, AVATAR_RIGHT_PATH };
	const wchar_t* bkIMGURL[2] = { AVATAR_LEFT_MASK_PATH, AVATAR_RIGHT_MASK_PATH };

	pActor->setActor(new MoveActor(3, mkIMGURL, bkIMGURL));

	while (true)
	{
		int moveDir = Actor::DIRECTION::None;
		bool speedUp = false;

		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			moveDir = Actor::DIRECTION::Left;
		}

		if (GetAsyncKeyState('A') & 0x8000)
		{
			moveDir = Actor::DIRECTION::Left;
		}

		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			moveDir = Actor::DIRECTION::Right;
		}

		if (GetAsyncKeyState('D') & 0x8000)
		{
			moveDir = Actor::DIRECTION::Right;
		}

		if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		{
			speedUp = true;
		}

		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			moveDir = Actor::DIRECTION::Up;
		}

		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			moveDir = Actor::DIRECTION::Up;
		}

		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			moveDir = Actor::DIRECTION::Down;
		}

		if (GetAsyncKeyState('C') & 0x8000)
		{
			moveDir = Actor::DIRECTION::Down;
		}

		pActor->playScene(moveDir, speedUp);
	}
}
