#pragma once
#include"Explodes.h"

#define M_PI 3.14159265358979323846

int checkCrash(const std::vector<Block>& blocks, const Block& mainBlock,const GameUI& gameUi, const std::vector<Block>& specailBlocks) {
	RECT checkRect;

	for (const Block& block : blocks) { // ���� ��� for ���� ���
		if (block.status && IntersectRect(&checkRect, &block.rect, &mainBlock.rect)) {
			return mainBlock.speed; // �浹�� �����Ǹ� mainBlock�� �ӵ���ŭ ��ȯ
		}
	}

	for (const Block& sBlock : specailBlocks) { //spcail�� �浹�˻�
		if (sBlock.status && IntersectRect(&checkRect, &sBlock.rect, &mainBlock.rect)) {
			return mainBlock.speed; // �浹�� �����Ǹ� mainBlock�� �ӵ���ŭ ��ȯ
		}
	}

	RECT changeGameBordRect = gameUi.gameBordRect;
	InflateRect(&changeGameBordRect, -(mainBlock.rect.right - mainBlock.rect.left), -(mainBlock.rect.bottom - mainBlock.rect.top));
	if (!IntersectRect(&checkRect, &changeGameBordRect, &mainBlock.rect)) {
		return mainBlock.speed;
	}

	return 0; // �浹�� ������ 0 ��ȯ
}

void resRet(HDC& mDC, const RECT rect, const CImage& mainResp) { // �簢������ �����ϴ� �κ�
	mainResp.Draw(mDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0, 0, mainResp.GetWidth(), mainResp.GetHeight());
}

void ExpandRect(RECT& rect) { // ������ Ȯ��
	int width = rect.right - rect.left;  // ������ �ʺ�
	int height = rect.bottom - rect.top;  // ������ ����

	// �߽��� ���
	int centerX = (rect.left + rect.right) / 2;
	int centerY = (rect.top + rect.bottom) / 2;

	// �߽����� �����ϸ鼭 �� ���� ������ �ʺ�� ������ ���ݸ�ŭ Ȯ��
	// �߽����� �����ϸ鼭 �� ���� ���� �ʺ�� ������ 1.5�踸ŭ Ȯ��
	rect.left = centerX - (int)(width * 1.5);
	rect.right = centerX + (int)(width * 1.5);
	rect.top = centerY - (int)(height * 1.5);
	rect.bottom = centerY + (int)(height * 1.5);
}


void blankMain(std::vector<Block>& blocks, const Block* block, std::vector<Block>& redBlocks) { // ���� ���� ������, ��� ��ĭ�����
	RECT blankSizeRect = block->rect;
	ExpandRect(blankSizeRect);

	RECT selectRect;

	for (int i = 0; i < blocks.size(); ++i) {
		if (IntersectRect(&selectRect, &blankSizeRect, &blocks[i].rect)) {
			blocks[i].status = false;
		}
		for (int i = 0; i < redBlocks.size(); ++i) {
			if (IntersectRect(&selectRect, &blankSizeRect, &redBlocks[i].rect)) {
				redBlocks.erase(redBlocks.begin() + i);
			}
		}
	}
}

bool moveRedBlock(std::vector<Block>& redBlocks, Block& mainBlock, HDC& mDC, std::vector<Explosion>& explodes,const Setting& setting) {

	for (Block& redBlock : redBlocks) { 
		if (redBlock.status) {
			// mainBlock�� �߽� ���
			int mainCenterX = (mainBlock.rect.left + mainBlock.rect.right) / 2;
			int mainCenterY = (mainBlock.rect.top + mainBlock.rect.bottom) / 2;

			// redBlock�� �߽� ���
			int redCenterX = (redBlock.rect.left + redBlock.rect.right) / 2;
			int redCenterY = (redBlock.rect.top + redBlock.rect.bottom) / 2;

			// mainBlock�� ���� �̵��� ��ǥ ���
			if (redCenterX < mainCenterX) {
				redBlock.rect.left += redBlock.speed;
				redBlock.rect.right += redBlock.speed;
				redBlock.aroundRect.left += redBlock.speed;
				redBlock.aroundRect.right += redBlock.speed;
			}
			else if (redCenterX > mainCenterX) {
				redBlock.rect.left -= redBlock.speed;
				redBlock.rect.right -= redBlock.speed;
				redBlock.aroundRect.left -= redBlock.speed;
				redBlock.aroundRect.right -= redBlock.speed;
			}

			if (redCenterY < mainCenterY) {
				redBlock.rect.top += redBlock.speed;
				redBlock.rect.bottom += redBlock.speed;
				redBlock.aroundRect.top += redBlock.speed;
				redBlock.aroundRect.bottom += redBlock.speed;
			}
			else if (redCenterY > mainCenterY) {
				redBlock.rect.top -= redBlock.speed;
				redBlock.rect.bottom -= redBlock.speed;
				redBlock.aroundRect.top -= redBlock.speed;
				redBlock.aroundRect.bottom -= redBlock.speed;
			}
		}
		RECT c_rect;
		// ���� �� + ���� �� �ε�����
		if (IntersectRect(&c_rect, &redBlock.rect, &mainBlock.rect)&& !setting.getInvincibility()) {
			if (redBlock.status) {
				Explosion b(mainBlock.rect, mainBlock.color);
				explodes.push_back(b);
				mainBlock.status = false;
				return true;
			}
		}
	}

	return false;
}

void moveChangeBackgroundBlack(const std::vector<Block>& redBlocks, std::vector<Block>& blocks) {
	RECT c_rect;
	for (auto& block : blocks) {
		if (!block.status) {
			for (auto& redBlock : redBlocks) {
				if (redBlock.status) {
					if (IntersectRect(&c_rect, &block.rect, &redBlock.rect)) {
						block.status = true;
					}
				}
			}
		}
	}
}

bool findFalseBullet(std::vector<Bullet>& mainBullets, Bullet*& b) {
	if (mainBullets.size() > 0) {
		for (int i = 0; i < mainBullets.size(); ++i) {
			if (!mainBullets[i].bullet_move_status) {
				b = &mainBullets[i];
				return true;
			}
		}
	}
	return false;
}

void bulletScaleDown(Bullet*& b, const Block& mainBlock) {
	// mainBlock.rect�� �߾��� �������� ũ�⸦ 60%�� �ٿ��� b->rect�� �Ҵ�
	int width = mainBlock.rect.right - mainBlock.rect.left;
	int height = mainBlock.rect.bottom - mainBlock.rect.top;

	int newWidth = static_cast<int>(width * 0.3);
	int newHeight = static_cast<int>(height * 0.3);

	int centerX = mainBlock.rect.left + width / 2;
	int centerY = mainBlock.rect.top + height / 2;

	b->rect.left = centerX - newWidth / 2;
	b->rect.top = centerY - newHeight / 2;
	b->rect.right = centerX + newWidth / 2;
	b->rect.bottom = centerY + newHeight / 2;
}

void checkBulletBlock(const Bullet& bullet, std::vector<Block>& blocks) {
	RECT ch_rect;
	for (auto& block : blocks) {
		if (block.status) {
			if (IntersectRect(&ch_rect, &block.rect, &bullet.rect)) {
				block.status = false;
			}
		}
	}
}

void aroundBroken(std::vector<Block>& redBlocks, Block& redBlock, std::vector<Explosion>& explodes, std::vector<Block>& blocks, int& combo) { //��������.
	RECT ch_rect;
	combo = 0;
	for (int i = redBlocks.size() - 1; i >= 0; --i) {
		if (IntersectRect(&ch_rect, &redBlocks[i].rect, &redBlock.aroundRect)) {
			combo++;
			// ����
			Explosion b(redBlocks[i].rect, redBlocks[i].color);
			explodes.push_back(b);
			RECT r_ch = redBlocks[i].rect; // ������ ���� �κ�
			InflateRect(&r_ch, gameUi.cellSize, gameUi.cellSize);
			for (auto& block : blocks) {
				if (IntersectRect(&ch_rect, &r_ch, &block.rect)) {
					block.status = false;
				}
			}

			redBlocks.erase(redBlocks.begin() + i);
			--i;
		}
	}
}

bool checkRedBlockBullet(Bullet& bullet, std::vector<Block>& redBlocks, std::vector<Block>& blocks, HDC& mDC,const GameUI& gameUi, std::vector<Explosion>& explodes, int& combo) {
	RECT ch_rect;
	for (int i = redBlocks.size() - 1; i >= 0; --i) {
		if (IntersectRect(&ch_rect, &redBlocks[i].rect, &bullet.rect) && redBlocks[i].status) {
			// ����
			Explosion b(redBlocks[i].rect, redBlocks[i].color);
			explodes.push_back(b);
			RECT r_ch = redBlocks[i].rect; // ������ ���� �κ�
			InflateRect(&r_ch, gameUi.cellSize, gameUi.cellSize);
			for (auto& block : blocks) {
				if (IntersectRect(&ch_rect, &r_ch, &block.rect)) {
					block.status = false;
				}
			}

			// game score �� �ֺ� ����
			//aroundBroken(redBlocks, redBlocks[i], explodes, blocks, combo);

			redBlocks.erase(redBlocks.begin() + i);

			if (!bullet.through) {
				return true;
			}
		}
	}
	return false;
}

void print_score(HDC& mDC, const GameUI& gameUi) { // const ����
    // ��Ʈ ����
    HFONT hFont = CreateFont(
        36,                        // ���� ����
        0,                         // ���� ��
        0,                         // ���� ����
        0,                         // ���ؼ� ����
        FW_BOLD,                   // ���� ����
        FALSE,                     // ����� ����
        FALSE,                     // ����
        FALSE,                     // ��Ҽ�
        DEFAULT_CHARSET,           // ���� ����
        OUT_DEFAULT_PRECIS,        // ��� ��Ȯ��
        CLIP_DEFAULT_PRECIS,       // Ŭ�� ��Ȯ��
        DEFAULT_QUALITY,           // ��� ǰ��
        DEFAULT_PITCH | FF_SWISS,  // �۲� �� �۲� �йи�
        L"Arial");                 // �۲� �̸�

    // ��Ʈ ����
    HFONT hOldFont = (HFONT)SelectObject(mDC, hFont);

    // ���ھ� ���
    int score = gameUi.getScore();
    std::wstring scoreText = L"Score: " + std::to_wstring(score);
    RECT scoreRect = { 10, 10, 200, 50 };
    DrawText(mDC, scoreText.c_str(), -1, &scoreRect, DT_LEFT | DT_TOP | DT_SINGLELINE);

    // ���� ��Ʈ ���� �� �� ��Ʈ ����
    SelectObject(mDC, hOldFont);
    DeleteObject(hFont);
}

void print_combo(HDC& mDC, const int& combo, RECT rect) {
	std::wstring scoreText = L"combo X " + std::to_wstring(combo);
	RECT scoreRect = rect;
	InflateRect(&scoreRect, 10, 0);
	DrawText(mDC, scoreText.c_str(), -1, &scoreRect, DT_LEFT | DT_TOP | DT_SINGLELINE);
}

#define SHAKE_TIMER 7
#define SHAKE_DURATION 100 // duration in milliseconds
#define SHAKE_INTERVAL 10 // interval in milliseconds
#define SHAKE_MAGNITUDE 2 // shake intensity

void moveRect(GameUI& gameUi, std::vector<Block>& blocks, int& move_cnt, HWND hWnd) {
	gameUi.originalGameBordRect = gameUi.gameBordRect;  // Save the original position
	for (auto& block : blocks) {
		block.originalRect = block.rect;  // �� ����� ���� ��ġ ����
	}
	move_cnt = SHAKE_DURATION / SHAKE_INTERVAL;
	SetTimer(hWnd, SHAKE_TIMER, SHAKE_INTERVAL, NULL); // Start the shake timer
}

void rotateBullets(std::vector<Bullet>& bullets, const POINT& center, double angle) {
	double radians = angle * (M_PI / 180.0);
	int numBullets = bullets.size();
	double angleStep = 360.0 / numBullets;
	for (int i = 0; i < numBullets; ++i) {
		if (!bullets[i].bullet_move_status) {
			double currentAngle = i * angleStep + angle;
			double currentRadians = currentAngle * (M_PI / 180.0);
			int width = bullets[i].rect.right - bullets[i].rect.left;
			int height = bullets[i].rect.bottom - bullets[i].rect.top;
			int radius = 15; // �߽����κ����� �Ÿ�
			int x = static_cast<int>(radius * cos(currentRadians));
			int y = static_cast<int>(radius * sin(currentRadians));

			bullets[i].rect.left = center.x + x - width / 2;
			bullets[i].rect.top = center.y + y - height / 2;
			bullets[i].rect.right = bullets[i].rect.left + width;
			bullets[i].rect.bottom = bullets[i].rect.top + height;
		}
	}
}