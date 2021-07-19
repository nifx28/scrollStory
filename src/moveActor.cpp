#include "stdafx.h"
#include "moveActor.h"

using namespace std;

MoveActor::MoveActor(int maxFrameNum, const wchar_t* mkIMGURL[], const wchar_t* bkIMGURL[]) :
	groundZero(0), jumpHeight(20),
	moveDir(0), maxFrameNum(maxFrameNum),
	isJumping(false), isFalling(false)
{
	for (int i = 0; i < 2; i++)
	{
		loadimage(mkIMG + i, mkIMGURL[i]);
		loadimage(bkIMG + i, bkIMGURL[i]);
	}

	loadimage(&avatar, AVATAR_TILE_PATH, 833, 1345);

	DWORD* mkBUF = nullptr, * bkBUF = nullptr;
	mkBUF = GetImageBuffer(&avatar);

	mask = IMAGE(833, 1345);
	bkBUF = GetImageBuffer(&mask);
	map<DWORD, int> m;

	wofstream ofs(L"Mask.log");

	for (int i = 0; i < mask.getwidth() * mask.getheight(); i++)
	{
		if (m.find(mkBUF[i]) == m.end())
		{
			m[mkBUF[i]] = 1;
		}
		else
		{
			m[mkBUF[i]] += 1;
		}

		if ((mkBUF[i] & 0xFF000000) != 0xFF000000)
		{
			mkBUF[i] = BLACK;
			bkBUF[i] = WHITE;
		}
		else
		{
			bkBUF[i] = BLACK;
		}
	}

	vector<pair<DWORD, int>> list;

	for (const auto& it : m) {
		list.push_back(it);
	}

	sort(list.begin(), list.end(), [](const pair<DWORD, int>& a, const pair<DWORD, int>& b) {
		return a.second > b.second;
	});

	for (const auto& it : list)
	{
		ofs << hex << it.first << L": " << dec << it.second << L'\n';
	}

	ofs.close();
	saveimage(L"Avatar.png", &avatar);
	saveimage(L"Mask.png", &mask);
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
		//cleardevice();
		putimage(0, 0, width, height, &background, 0, 0);

		for (int j = 0, y = 0; j < 5; j++)
		{
			y += *(imgGridW + j * 5);

			for (int i = 0, x = 40; i < 5; i++)
			{
				rectangle(x, y, x + 130, y + *(imgGridZ + j * 5));
				putimage(x, y, 130, *(imgGridZ + j * 5), &mask, *(imgGridX + j * 5 + i), *(imgGridY + j * 5 + i), SRCAND);
				putimage(x, y, 130, *(imgGridZ + j * 5), &avatar, *(imgGridX + j * 5 + i), *(imgGridY + j * 5 + i), SRCINVERT);
				x += 140;
			}
		}

		putimage(width - miniWidth - 10, 10, &miniMap);
		//putimage(140, 160, 130, 150, &mask, 408, 127, SRCAND);
		//putimage(140, 160, 130, 150, &avatar, 408, 127, SRCINVERT);
		putimage(x, y, 50, 80, bkIMG + moveDir, frame * 50, 0, SRCAND);
		putimage(x, y, 50, 80, mkIMG + moveDir, frame * 50, 0, SRCPAINT);
		frame++;
		FlushBatchDraw();
		Sleep(100);
		EndBatchDraw();
	}
}
