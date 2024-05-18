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
	//int capacity; //�Ѿ� ��
	RECT rect;
	COLORREF color = RGB(0, 0, 0);
	COLORREF borderColor = RGB(0, 0, 0);
	int speed = 7;
	int max_speed = 15;
	
	int capacity = 0; //�뷮

	bool status = true; // ���� ����
	bool special = false;
	bool bullet_move_status = false; // �Ѿ� �̵��ϴ��� üũ

	bool through = false; // ���� ����

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
		int radius = 20; // �߽����κ����� �Ÿ�
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
		// ���� ���� �� �ܰ� �� ����
		int tailLength = 100;
		int steps = 10;
		int stepLength = tailLength / steps;

		// ���� ����� �� ���� ����
		COLORREF startColor = bullet.color;
		COLORREF endColor = RGB(255, 255, 255); // ���

		// ���� �׸���
		for (int i = 0; i < steps; ++i) {
			double t = static_cast<double>(i) / steps;
			COLORREF currentColor = interpolateColor(startColor, endColor, t);

			HBRUSH hBrush = CreateSolidBrush(currentColor);
			HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush);
			HPEN hPen = (HPEN)SelectObject(mDC, GetStockObject(NULL_PEN));

			RECT tailRect = bullet.rect;

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

			// ���� �귯�ÿ� ������ ����
			SelectObject(mDC, oldBrush);
			SelectObject(mDC, hPen);
			DeleteObject(hBrush);
		}

		// �Ѿ��� �Ӹ� �κ��� �ձ۰� �׸���
		HBRUSH hBrush = CreateSolidBrush(bullet.color);
		HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush);
		HPEN hPen = (HPEN)SelectObject(mDC, GetStockObject(NULL_PEN));

		if (bullet.right) {
			RECT headRect = bullet.rect;
			headRect.right += 10; // �ձ� �κ��� ���� �ణ Ȯ��
			Rectangle(mDC, bullet.rect.left, bullet.rect.top, bullet.rect.right, bullet.rect.bottom);
			Ellipse(mDC, bullet.rect.right - 10, bullet.rect.top, bullet.rect.right + 5, bullet.rect.bottom);
		}
		else if (bullet.left) {
			RECT headRect = bullet.rect;
			headRect.left -= 10; // �ձ� �κ��� ���� �ణ Ȯ��
			Rectangle(mDC, bullet.rect.left, bullet.rect.top, bullet.rect.right, bullet.rect.bottom);
			Ellipse(mDC, bullet.rect.left - 5, bullet.rect.top, bullet.rect.left + 10, bullet.rect.bottom);
		}
		else if (bullet.up) {
			RECT headRect = bullet.rect;
			headRect.top -= 10; // �ձ� �κ��� ���� �ణ Ȯ��
			Rectangle(mDC, bullet.rect.left, bullet.rect.top, bullet.rect.right, bullet.rect.bottom);
			Ellipse(mDC, bullet.rect.left, bullet.rect.top - 5, bullet.rect.right, bullet.rect.top + 10);
		}
		else if (bullet.down) {
			RECT headRect = bullet.rect;
			headRect.bottom += 10; // �ձ� �κ��� ���� �ణ Ȯ��
			Rectangle(mDC, bullet.rect.left, bullet.rect.top, bullet.rect.right, bullet.rect.bottom);
			Ellipse(mDC, bullet.rect.left, bullet.rect.bottom - 10, bullet.rect.right, bullet.rect.bottom + 5);
		}

		// ���� �귯�ÿ� ������ ����
		SelectObject(mDC, oldBrush);
		SelectObject(mDC, hPen);
		DeleteObject(hBrush);
	}

};