#pragma once

//int checkCrash(const std::vector<Block> blocks, const Block mainBlock) { // ���� �����̶� �������̶� �浹 üũ
//	RECT checkRect;
//
//	for (int i = 0; i < blocks.size(); ++i) {
//		if (IntersectRect(&checkRect, &blocks[i].rect, &mainBlock.rect) && blocks[i].status) {
//			return mainBlock.speed;
//		}
//	}
//	return 0;
//}

int checkCrash(const std::vector<Block>& blocks, const Block& mainBlock) {
	RECT checkRect;

	for (const Block& block : blocks) { // ���� ��� for ���� ���
		if (block.status && IntersectRect(&checkRect, &block.rect, &mainBlock.rect)) {
			return mainBlock.speed; // �浹�� �����Ǹ� mainBlock�� �ӵ���ŭ ��ȯ
		}
	}
	return 0; // �浹�� ������ 0 ��ȯ
}

void resRet(HDC &mDC, RECT rect, CImage& mainResp) { // �簢������ �����ϴ� �κ�
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


void blankMain(std::vector<Block>& blocks, Block* block) { // ���� ���� ������, ��� ��ĭ�����
	RECT blankSizeRect = block->rect;
	ExpandRect(blankSizeRect);

	RECT selectRect;

	for (int i = 0; i < blocks.size(); ++i) {
		if (IntersectRect(&selectRect, &blankSizeRect, &blocks[i].rect)) {
			blocks[i].status = false;
		}
	}
}

void moveRedBlock(std::vector<Block>& redBlocks, Block& mainBlock) {

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
	}

}