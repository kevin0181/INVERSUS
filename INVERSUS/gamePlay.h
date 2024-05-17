#pragma once

//int checkCrash(const std::vector<Block> blocks, const Block mainBlock) { // 메인 에셋이랑 검은돌이랑 충돌 체크
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

	for (const Block& block : blocks) { // 범위 기반 for 루프 사용
		if (block.status && IntersectRect(&checkRect, &block.rect, &mainBlock.rect)) {
			return mainBlock.speed; // 충돌이 감지되면 mainBlock의 속도만큼 반환
		}
	}
	return 0; // 충돌이 없으면 0 반환
}

void resRet(HDC &mDC, RECT rect, CImage& mainResp) { // 사각형으로 생성하는 부분
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

void moveRedBlock(std::vector<Block>& redBlocks, Block& mainBlock) {

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
	}

}