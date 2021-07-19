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

	HDC hDC = nullptr;
	hDC = GetImageHDC();
	HICON hIcon = nullptr;

	if (hDC)
	{
		hIcon = reinterpret_cast<HICON>(LoadImage(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT));
	}

	HDC memDC = nullptr;

	if (hIcon)
	{
		memDC = CreateCompatibleDC(nullptr);
	}

	ICONINFO iconInfo = {};

	if (memDC)
	{
		GetIconInfo(hIcon, &iconInfo);
	}
	else
	{
		return;
	}

	HBITMAP hBitmap = nullptr;
	hBitmap = iconInfo.hbmColor;

	BITMAP bmp = {};
	BITMAPINFOHEADER bmpInfo = {};
	GetObject(hBitmap, sizeof(BITMAP), &bmp);

	bmpInfo.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.biWidth = bmp.bmWidth;
	bmpInfo.biHeight = bmp.bmHeight;
	bmpInfo.biPlanes = bmp.bmPlanes;
	bmpInfo.biBitCount = bmp.bmBitsPixel;
	bmpInfo.biCompression = BI_RGB;
	bmpInfo.biSizeImage = ((bmp.bmWidth * bmp.bmBitsPixel + 31) / 32) * 4 * bmp.bmHeight;
	bmpInfo.biClrImportant = 0;

	LPBYTE lpBits = nullptr;
	lpBits = reinterpret_cast<LPBYTE>(GlobalAlloc(GMEM_FIXED, bmpInfo.biSizeImage));

	if (!lpBits)
	{
		return;
	}

	GetDIBits(memDC, hBitmap, 0, bmp.bmHeight, lpBits, reinterpret_cast<BITMAPINFO*>(&bmpInfo), DIB_RGB_COLORS);

	IMAGE bkIMG(bmp.bmWidth, bmp.bmHeight);
	bkBUF = GetImageBuffer(&bkIMG);
	map<BYTE, int> m;

	wofstream ofs(L"Mask.log");
	ofs << L"hIcon: " << (hIcon ? L'Y' : L'N') << L'\n';
	ofs << bmp.bmWidth << L" x " << bmp.bmHeight << L" = " << bmp.bmPlanes << L", " << bmp.bmBitsPixel << L'\n';
	ofs << L"biSizeImage: " << bmpInfo.biSizeImage << L'\n';
	ofs << L"lpBits: " << (lpBits ? L'Y' : L'N') << L'\n';

	for (int i = 0; i < bmpInfo.biSizeImage; i += 4)
	{
		/*if (m.find(lpBits[i]) == m.end())
		{
			m[lpBits[i]] = 1;
		}
		else
		{
			m[lpBits[i]] += 1;
		}

		if ((lpBits[i] & 0xFF000000) != 0xFF000000)
		{
			bkBUF[i] = BLACK;
		}
		else
		{
			bkBUF[i] = WHITE;
		}*/
	}

	if (lpBits)
	{
		GlobalFree(lpBits);
		lpBits = nullptr;
	}

	if (hIcon)
	{
		DestroyIcon(hIcon);
		hIcon = nullptr;
	}

	if (memDC)
	{
		DeleteDC(memDC);
		memDC = nullptr;
	}

	if (hDC)
	{
		hDC = nullptr;
	}

	vector<pair<BYTE, int>> list;

	for (const auto& it : m) {
		list.push_back(it);
	}

	sort(list.begin(), list.end(), [](const pair<BYTE, int>& a, const pair<BYTE, int>& b) {
		return a.second > b.second;
	});

	for (const auto& it : list)
	{
		ofs << hex << it.first << L": " << dec << it.second << L'\n';
	}

	ofs.close();
	saveimage(L"Mask.png", &bkIMG);
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

		for (int j = 0; j < 4; j++)
		{
			for (int i = 0; i < 7; i++)
			{
				putimage(i * 140, j * 160, 130, 150, &avatar, *(imgGridX + j * 5 + i), *(imgGridY + j * 5 + i));
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
