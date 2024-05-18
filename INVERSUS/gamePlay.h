#pragma once
#include"Explodes.h"

#define M_PI 3.14159265358979323846

int checkCrash(const std::vector<Block>& blocks, const Block& mainBlock,const GameUI& gameUi, const std::vector<Block>& specailBlocks) {
	RECT checkRect;

	for (const Block& block : blocks) { // 범위 기반 for 루프 사용
		if (block.status && IntersectRect(&checkRect, &block.rect, &mainBlock.rect)) {
			return mainBlock.speed; // 충돌이 감지되면 mainBlock의 속도만큼 반환
		}
	}

	for (const Block& sBlock : specailBlocks) { //spcail블럭 충돌검사
		if (sBlock.status && IntersectRect(&checkRect, &sBlock.rect, &mainBlock.rect)) {
			return mainBlock.speed; // 충돌이 감지되면 mainBlock의 속도만큼 반환
		}
	}

	RECT changeGameBordRect = gameUi.gameBordRect;
	InflateRect(&changeGameBordRect, -(mainBlock.rect.right - mainBlock.rect.left), -(mainBlock.rect.bottom - mainBlock.rect.top));
	if (!IntersectRect(&checkRect, &changeGameBordRect, &mainBlock.rect)) {
		return mainBlock.speed;
	}

	return 0; // 충돌이 없으면 0 반환
}

void resRet(HDC& mDC, const RECT rect, const CImage& mainResp) { // 사각형으로 생성하는 부분
	mainResp.Draw(mDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0, 0, mainResp.GetWidth(), mainResp.GetHeight());
}

void ExpandRect(RECT& rect) { // 사이즈 확대
	int width = rect.right - rect.left;  // 원래의 너비
	int height = rect.bottom - rect.top;  // 원래의 높이

	// 중심점 계산
	int centerX = (rect.left + rect.right) / 2;
	int centerY = (rect.top + rect.bottom) / 2;

	// 중심점을 유지하면서 각 변을 원래의 너비와 높이의 절반만큼 확장
	// 중심점을 유지하면서 각 변을 원래 너비와 높이의 1.5배만큼 확장
	rect.left = centerX - (int)(width * 1.5);
	rect.right = centerX + (int)(width * 1.5);
	rect.top = centerY - (int)(height * 1.5);
	rect.bottom = centerY + (int)(height * 1.5);
}


void blankMain(std::vector<Block>& blocks, const Block* block, std::vector<Block>& redBlocks) { // 메인 에셋 생성시, 가운데 빈칸만들기
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
			// mainBlock의 중심 계산
			int mainCenterX = (mainBlock.rect.left + mainBlock.rect.right) / 2;
			int mainCenterY = (mainBlock.rect.top + mainBlock.rect.bottom) / 2;

			// redBlock의 중심 계산
			int redCenterX = (redBlock.rect.left + redBlock.rect.right) / 2;
			int redCenterY = (redBlock.rect.top + redBlock.rect.bottom) / 2;

			// mainBlock을 향해 이동할 좌표 계산
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
		// 메인 블럭 + 레드 블럭 부딪히면
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
	// mainBlock.rect의 중앙을 기준으로 크기를 60%로 줄여서 b->rect에 할당
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

void aroundBroken(std::vector<Block>& redBlocks, Block& redBlock, std::vector<Explosion>& explodes, std::vector<Block>& blocks, int& combo) { //연쇄폭발.
	RECT ch_rect;
	combo = 0;
	for (int i = redBlocks.size() - 1; i >= 0; --i) {
		if (IntersectRect(&ch_rect, &redBlocks[i].rect, &redBlock.aroundRect)) {
			combo++;
			// 폭발
			Explosion b(redBlocks[i].rect, redBlocks[i].color);
			explodes.push_back(b);
			RECT r_ch = redBlocks[i].rect; // 없어질 검은 부분
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
			// 폭발
			Explosion b(redBlocks[i].rect, redBlocks[i].color);
			explodes.push_back(b);
			RECT r_ch = redBlocks[i].rect; // 없어질 검은 부분
			InflateRect(&r_ch, gameUi.cellSize, gameUi.cellSize);
			for (auto& block : blocks) {
				if (IntersectRect(&ch_rect, &r_ch, &block.rect)) {
					block.status = false;
				}
			}

			// game score 및 주변 터짐
			//aroundBroken(redBlocks, redBlocks[i], explodes, blocks, combo);

			redBlocks.erase(redBlocks.begin() + i);

			if (!bullet.through) {
				return true;
			}
		}
	}
	return false;
}

void print_score(HDC& mDC, const GameUI& gameUi) { // const 제거
    // 폰트 생성
    HFONT hFont = CreateFont(
        36,                        // 글자 높이
        0,                         // 글자 폭
        0,                         // 글자 각도
        0,                         // 기준선 각도
        FW_BOLD,                   // 글자 굵기
        FALSE,                     // 기울임 글자
        FALSE,                     // 밑줄
        FALSE,                     // 취소선
        DEFAULT_CHARSET,           // 문자 집합
        OUT_DEFAULT_PRECIS,        // 출력 정확도
        CLIP_DEFAULT_PRECIS,       // 클립 정확도
        DEFAULT_QUALITY,           // 출력 품질
        DEFAULT_PITCH | FF_SWISS,  // 글꼴 및 글꼴 패밀리
        L"Arial");                 // 글꼴 이름

    // 폰트 선택
    HFONT hOldFont = (HFONT)SelectObject(mDC, hFont);

    // 스코어 출력
    int score = gameUi.getScore();
    std::wstring scoreText = L"Score: " + std::to_wstring(score);
    RECT scoreRect = { 10, 10, 200, 50 };
    DrawText(mDC, scoreText.c_str(), -1, &scoreRect, DT_LEFT | DT_TOP | DT_SINGLELINE);

    // 이전 폰트 복원 및 새 폰트 삭제
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
		block.originalRect = block.rect;  // 각 블록의 원래 위치 저장
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
			int radius = 15; // 중심으로부터의 거리
			int x = static_cast<int>(radius * cos(currentRadians));
			int y = static_cast<int>(radius * sin(currentRadians));

			bullets[i].rect.left = center.x + x - width / 2;
			bullets[i].rect.top = center.y + y - height / 2;
			bullets[i].rect.right = bullets[i].rect.left + width;
			bullets[i].rect.bottom = bullets[i].rect.top + height;
		}
	}
}