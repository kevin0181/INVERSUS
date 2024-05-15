#pragma once

int checkCrash(std::vector<Block> blocks, Block mainBlock) { // 메인 에셋이랑 검은돌이랑 충돌 체크
	RECT checkRect;

	for (int i = 0; i < blocks.size(); ++i) {
		if (IntersectRect(&checkRect, &blocks[i].rect, &mainBlock.rect) && blocks[i].status) {
			return mainBlock.speed;
		}
	}
	return 0;
}

void resRet(HDC &mDC, RECT rect, CImage& mainResp) { // 사각형으로 생성하는 부분
	mainResp.Draw(mDC, rect.left, rect.top, rect.right, rect.bottom, 0, 0, mainResp.GetWidth(), mainResp.GetHeight());
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


void blankMain(std::vector<Block>& blocks, Block* block) { // 메인 에셋 생성시, 가운데 빈칸만들기
	RECT blankSizeRect = block->rect;
	ExpandRect(blankSizeRect);

	RECT selectRect;

	for (int i = 0; i < blocks.size(); ++i) {
		if (IntersectRect(&selectRect, &blankSizeRect, &blocks[i].rect)) {
			blocks[i].status = false;
		}
	}
}