#pragma once

int checkCrash(std::vector<Block> blocks, Block mainBlock) { // ���� �����̶� �������̶� �浹 üũ
	RECT checkRect;

	for (int i = 0; i < blocks.size(); ++i) {
		if (IntersectRect(&checkRect, &blocks[i].rect, &mainBlock.rect) && blocks[i].status) {
			return mainBlock.speed;
		}
	}
	return 0;
}

void resRet(HDC &mDC, RECT rect, CImage& mainResp) { // �簢������ �����ϴ� �κ�
	mainResp.Draw(mDC, rect.left, rect.top, rect.right, rect.bottom, 0, 0, mainResp.GetWidth(), mainResp.GetHeight());
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