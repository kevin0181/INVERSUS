#pragma once
#include <Windows.h>

class Explosion {
public:
    RECT rect;
    COLORREF color;
    int frame;

    Explosion(RECT r, COLORREF c) : rect(r), color(c), frame(0) {}

    bool update() {
        ++frame;
        return frame < 5; // 프레임
    }

    void draw(HDC& mDC,const Explosion& ex) const {
        HBRUSH brush = CreateSolidBrush(ex.color);
        SelectObject(mDC, brush);

        // 난수 생성기 초기화
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distX(ex.rect.left, ex.rect.right);
        std::uniform_int_distribution<int> distY(ex.rect.top, ex.rect.bottom);
        std::uniform_int_distribution<int> distSize(5, 50); // 원의 크기 범위

        // 여러 개의 원을 그려 폭발 효과 생성
        for (int i = 0; i < 10; ++i) { // 폭발 조각의 개수
            int x = distX(gen);
            int y = distY(gen);
            int size = distSize(gen);
            Ellipse(mDC, x - size, y - size, x + size, y + size);
        }

        DeleteObject(brush);
    }
};