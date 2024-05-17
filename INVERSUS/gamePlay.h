#pragma once
#include"Explodes.h"

int checkCrash(const std::vector<Block>& blocks, const Block& mainBlock) {
	RECT checkRect;

	for (const Block& block : blocks) { // ���� ��� for ���� ���
		if (block.status && IntersectRect(&checkRect, &block.rect, &mainBlock.rect)) {
			return mainBlock.speed; // �浹�� �����Ǹ� mainBlock�� �ӵ���ŭ ��ȯ
		}
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

bool moveRedBlock(std::vector<Block>& redBlocks, Block& mainBlock, HDC& mDC, std::vector<Explosion>& explodes) {

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
		if (IntersectRect(&c_rect, &redBlock.rect, &mainBlock.rect)) {
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
	//for (int i = mainBullets.size()-1; i >= 0; --i) {
	for (int i = 0; i < mainBullets.size(); ++i) {
		if (!mainBullets[i].bullet_move_status) {
			b = &mainBullets[i];
			return true;
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

void aroundBroken(std::vector<Block>& redBlocks, Block& redBlock, std::vector<Explosion>& explodes, std::vector<Block>& blocks) { //��������.
	RECT ch_rect;
	for (int i = 0; i < redBlocks.size(); ++i) {
		if (IntersectRect(&ch_rect, &redBlocks[i].rect, &redBlock.aroundRect)) {
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
		}
	}
}

bool checkRedBlockBullet(Bullet& bullet, std::vector<Block>& redBlocks, std::vector<Block>& blocks, HDC& mDC, const GameUI& gameUi, std::vector<Explosion>& explodes) {
	
	RECT ch_rect;
	for (int i = 0; i < redBlocks.size(); ++i) {
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

			//game score �� �ֺ� ����
			aroundBroken(redBlocks, redBlocks[i], explodes, blocks);

			//redBlocks.erase(redBlocks.begin() + i);

			if (!bullet.through) {
				return true;
			}
		}
	}
	return false;
}
