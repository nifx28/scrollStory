#include "stdafx.h"
#include "moveActor.h"

using namespace std;

MoveActor::MoveActor(int maxFrameNum, const wchar_t* mkIMGURL[], const wchar_t* bkIMGURL[]) :
	groundZero(0), jumpHeight(20),
	moveDir(0), maxFrameNum(maxFrameNum),
	isJumping(false), isFalling(false)
{
	loadimage(&avatar, L"assets\\avatars\\pnghut_maplestory-2-video-game-sprite-monster-inc.png", 833, 1345);

	for (int i = 0; i < 2; i++)
	{
		loadimage(mkIMG + i, mkIMGURL[i]);
		loadimage(bkIMG + i, bkIMGURL[i]);

		if (i < 1)
		{
			continue;
		}

		DWORD* mkBUF = nullptr, * bkBUF = nullptr;
		mkBUF = GetImageBuffer(bkIMG + i);
		
		IMAGE bkIMG(250, 80);
		bkBUF = GetImageBuffer(&bkIMG);
		COLORREF bkMask = 0xFFFFFFFF;
		//bkMask = 0xFF000009;
		map<DWORD, int> m;

		for (int j = 0; j < 20000; j++)
		{
			if (m.find(mkBUF[j]) == m.end())
			{
				m[mkBUF[j]] = 1;
			}
			else
			{
				m[mkBUF[j]] = m[mkBUF[j]] + 1;
			}

			if (mkBUF[j] != bkMask)
			{
				bkBUF[j] = mkBUF[j];
			}
			else
			{
				bkBUF[j] = WHITE;
			}
		}

		wofstream ofs(L"Mask.log");

		vector<pair<DWORD, int>> list;

		for (const auto& it : m) {
			list.push_back(it);
		}

		sort(list.begin(), list.end(), [](const pair<DWORD, int>& a, const pair<DWORD, int>& b) {
			return a.second < b.second;
		});

		for (const auto& it : list)
		{
			ofs << hex << it.first << L": " << dec << it.second << L'\n';
		}

		ofs.close();
		saveimage(L"Mask.png", &bkIMG);
	}
}

void MoveActor::setGroundZero(int y)
{
	this->groundZero = y;
}

void MoveActor::setDirection(int dir)
{
	this->moveDir = dir;
}

void MoveActor::drawScene(int width, int height, int miniWidth, int miniHeight, int& x, int& y, int dir, bool speedUp, const IMAGE& miniMap, const IMAGE& background)
{
	int moveDir = dir, speed = 8 + (speedUp ? 5 : 0);
	int frame = 0;

	while (frame < maxFrameNum)
	{
		switch (dir)
		{
		case Actor::DIRECTION::None:
			{
				moveDir = this->moveDir;
				break;
			}
		case Actor::DIRECTION::Left:
			{
				x -= speed;
				break;
			}
		case Actor::DIRECTION::Right:
			{
				x += speed;
				break;
			}
		case Actor::DIRECTION::Up:
			{
				if (!this->isJumping)
				{
					this->isJumping = true;
				}

				break;
			}
		case Actor::DIRECTION::Down:
			{
				y += speed;
				break;
			}
		}

		if (this->isJumping)
		{
			if (!this->isFalling)
			{
				y -= speed;
			}
			else
			{
				y += speed;
			}

			if (y < this->jumpHeight)
			{
				this->isFalling = true;
			}
		}

		if (y > this->groundZero)
		{
			this->isJumping = false;
			this->isFalling = false;
			y = this->groundZero;
		}

		BeginBatchDraw();
		putimage(0, 0, width, height, &background, 0, 0);

		for (int j = 0; j < 5; j++)
		{
			for (int i = 0; i < 5; i++)
			{
				putimage(i * 140, j * 140, 130, 130, &avatar, *(imgGridX + j * 5 + i), *(imgGridY + j * 5 + i));
			}
		}

		putimage(width - miniWidth - 10, 10, &miniMap);
		putimage(x, y, 50, 80, bkIMG + moveDir, frame * 50, 0, SRCAND);
		putimage(x, y, 50, 80, mkIMG + moveDir, frame * 50, 0, SRCPAINT);
		frame++;
		FlushBatchDraw();
		Sleep(100);
		EndBatchDraw();
	}
}
