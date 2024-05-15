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