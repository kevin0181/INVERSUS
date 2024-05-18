#pragma once
#include <Windows.h>
#define M_PI 3.14159265358979323846

class Bullet {
private:
	COLORREF interpolateColor(COLORREF start, COLORREF end, double t) {
		BYTE startR = GetRValue(start);
		BYTE startG = GetGValue(start);
		BYTE startB = GetBValue(start);

		BYTE endR = GetRValue(end);
		BYTE endG = GetGValue(end);
		BYTE endB = GetBValue(end);

		BYTE resultR = static_cast<BYTE>(startR + t * (endR - startR));
		BYTE resultG = static_cast<BYTE>(startG + t * (endG - startG));
		BYTE resultB = static_cast<BYTE>(startB + t * (endB - startB));

		return RGB(resultR, resultG, resultB);
	}
public:
	//int capacity; //총알 수
	RECT rect;
    RECT rect_l;
    RECT rect_r;

	COLORREF color = RGB(0, 0, 0);
	COLORREF borderColor = RGB(0, 0, 0);
	int speed = 7;
	int max_speed = 15;
	
	int capacity = 0; //용량

	bool status = true; // 장전 상태
	bool special = false;
	bool bullet_move_status = false; // 총알 이동하는지 체크

	bool through = false; // 관통 상태
	bool upgradeBullet = false;

	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;

	void bullet_default_print(HDC& mDC, Bullet& bullet) {
		HBRUSH hBrush = CreateSolidBrush(bullet.color);
		HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush);
		Ellipse(mDC, bullet.rect.left, bullet.rect.top, bullet.rect.right, bullet.rect.bottom);
		SelectObject(mDC, oldBrush);
		DeleteObject(hBrush);
	}

	void bullet_drop_print(HDC& mDC, Bullet& bullet, double angle) {
		int numBullets = bullet.capacity;
		double angleStep = 360.0 / numBullets;
		int radius = 20; // 중심으로부터의 거리
		int width = bullet.rect.right - bullet.rect.left;
		int height = bullet.rect.bottom - bullet.rect.top;
		double radians = angle * (M_PI / 180.0);

		for (int i = 0; i < numBullets; ++i) {
			double currentAngle = i * angleStep + angle;
			double currentRadians = currentAngle * (M_PI / 180.0);
			int x = static_cast<int>(radius * cos(currentRadians));
			int y = static_cast<int>(radius * sin(currentRadians));

			int left = bullet.rect.left + x - width / 2;
			int top = bullet.rect.top + y - height / 2;
			int right = left + width;
			int bottom = top + height;

			HBRUSH hBrush = CreateSolidBrush(bullet.color);
			HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush);

			HPEN hPen = CreatePen(PS_SOLID, 1, bullet.borderColor);
			HPEN oldPen = (HPEN)SelectObject(mDC, hPen);

			Ellipse(mDC, left, top, right, bottom);

			SelectObject(mDC, oldPen);
			SelectObject(mDC, oldBrush);
			DeleteObject(hBrush);
			DeleteObject(hPen);
		}
	}

    void print(HDC& mDC, Bullet& bullet) {
        // 꼬리 길이 및 단계 수 설정
        int tailLength = 100;
        int steps = 10;
        int stepLength = tailLength / steps;

        // 시작 색상과 끝 색상 설정
        COLORREF startColor = bullet.color;
        COLORREF endColor = RGB(255, 255, 255); // 흰색

        auto drawBullet = [&](RECT rect, COLORREF color) {
            for (int i = 0; i < steps; ++i) {
                double t = static_cast<double>(i) / steps;
                COLORREF currentColor = interpolateColor(startColor, endColor, t);

                HBRUSH hBrush = CreateSolidBrush(currentColor);
                HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush);
                HPEN hPen = (HPEN)SelectObject(mDC, GetStockObject(NULL_PEN));

                RECT tailRect = rect;

                if (bullet.right) {
                    tailRect.left -= stepLength * i;
                    tailRect.right -= stepLength * i;
                }
                else if (bullet.left) {
                    tailRect.left += stepLength * i;
                    tailRect.right += stepLength * i;
                }
                else if (bullet.up) {
                    tailRect.top += stepLength * i;
                    tailRect.bottom += stepLength * i;
                }
                else if (bullet.down) {
                    tailRect.top -= stepLength * i;
                    tailRect.bottom -= stepLength * i;
                }

                Rectangle(mDC, tailRect.left, tailRect.top, tailRect.right, tailRect.bottom);

                // 원래 브러시와 펜으로 복구
                SelectObject(mDC, oldBrush);
                SelectObject(mDC, hPen);
                DeleteObject(hBrush);
            }

            // 총알의 머리 부분을 둥글게 그린다
            HBRUSH hBrush = CreateSolidBrush(bullet.color);
            HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush);
            HPEN hPen = (HPEN)SelectObject(mDC, GetStockObject(NULL_PEN));

            if (bullet.right) {
                RECT headRect = rect;
                headRect.right += 10; // 둥근 부분을 위해 약간 확장
                Rectangle(mDC, rect.left, rect.top, rect.right, rect.bottom);
                Ellipse(mDC, rect.right - 10, rect.top, rect.right + 5, rect.bottom);
            }
            else if (bullet.left) {
                RECT headRect = rect;
                headRect.left -= 10; // 둥근 부분을 위해 약간 확장
                Rectangle(mDC, rect.left, rect.top, rect.right, rect.bottom);
                Ellipse(mDC, rect.left - 5, rect.top, rect.left + 10, rect.bottom);
            }
            else if (bullet.up) {
                RECT headRect = rect;
                headRect.top -= 10; // 둥근 부분을 위해 약간 확장
                Rectangle(mDC, rect.left, rect.top, rect.right, rect.bottom);
                Ellipse(mDC, rect.left, rect.top - 5, rect.right, rect.top + 10);
            }
            else if (bullet.down) {
                RECT headRect = rect;
                headRect.bottom += 10; // 둥근 부분을 위해 약간 확장
                Rectangle(mDC, rect.left, rect.top, rect.right, rect.bottom);
                Ellipse(mDC, rect.left, rect.bottom - 10, rect.right, rect.bottom + 5);
            }

            // 원래 브러시와 펜으로 복구
            SelectObject(mDC, oldBrush);
            SelectObject(mDC, hPen);
            DeleteObject(hBrush);
        };

        // 기본 총알 그리기
        drawBullet(bullet.rect, bullet.color);

        // 업그레이드된 총알 그리기
        if (bullet.upgradeBullet) {
            bullet.rect_l = bullet.rect;
            bullet.rect_r = bullet.rect;

            if (bullet.left || bullet.right) {
                bullet.rect_l.top -= 30; // 위쪽으로 20 픽셀 이동
                bullet.rect_l.bottom -= 30;
                bullet.rect_r.top += 30; // 아래쪽으로 20 픽셀 이동
                bullet.rect_r.bottom += 30;
            }
            else if (bullet.up || bullet.down) {
                bullet.rect_l.left -= 30; // 왼쪽으로 20 픽셀 이동
                bullet.rect_l.right -= 30;
                bullet.rect_r.left += 30; // 오른쪽으로 20 픽셀 이동
                bullet.rect_r.right += 30;
            }


            drawBullet(bullet.rect_l, bullet.color);
            drawBullet(bullet.rect_r, bullet.color);
        }
    }

};