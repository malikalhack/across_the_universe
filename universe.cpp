#include "universe.h"
#include <ctime>
#include <math.h>

#define SPEED       (15.f)

Universe::Universe()
    :_client_width(GetSystemMetrics(SM_CXSCREEN)),
     _client_height(GetSystemMetrics(SM_CYSCREEN)),
     _D((_client_width + _client_height) / 2),
     _oX(_client_width / 2),
     _oY(_client_height / 2)
{
    srand(static_cast<uint32_t>(std::time(nullptr)));
    for (int i = 0; i < STAR_COUNT; i++) {
        point3d[i].x = 1.0f * (rand() % _client_width)  - _oX;
        point3d[i].y = 1.0f * (rand() % _client_height) - _oY;
        point3d[i].z = 1.0f * (rand() % _client_width)  - _oX;
    }
}

Universe ::~Universe() = default;

void Universe::_MoveStars(SEllipse_t * const el, const uint16_t star_number) {
    float x, y, k, star_radius;

    point3d[star_number].z = point3d[star_number].z - SPEED;

    if (!(point3d[star_number].z + _D)) {
        point3d[star_number].z = 0.1f;
    }

    if (point3d[star_number].z <= -_D) {
        point3d[star_number].z = 0.f;
    }

    k = _D /(point3d[star_number].z + _D);

    x = point3d[star_number].x * k + _oX;
    y = point3d[star_number].y * k + _oY;

    star_radius = point3d[star_number].z / 200.f;

    el->left = static_cast<int>(floor(x - star_radius));
    el->top = static_cast<int>(floor(y - star_radius));
    el->right = static_cast<int>(floor(x + star_radius));
    el->bottom = static_cast<int>(floor(y + star_radius));

}

int32_t Universe::GetWidth() {
    return _client_width;
}

int32_t Universe::GetHeigth() {
    return _client_height;
}

void Universe::DrawingMainWindow(HWND hWnd, HDC compdc) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);

    HPEN hpen = (HPEN)GetStockObject(BLACK_PEN);
    HBRUSH hbrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
    SelectObject(compdc, hpen);
    SelectObject(compdc, hbrush);
    Rectangle(compdc, 0, 0, _client_width, _client_height);

    // Draw The Stars In White
    hpen = (HPEN)GetStockObject(WHITE_PEN);
    hbrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    SelectObject(compdc, hpen);
    SelectObject(compdc, hbrush);

    SEllipse_t el;
    for (uint16_t i = 0; i < STAR_COUNT; i++) {
        _MoveStars(&el, i);
        Ellipse(compdc, el.left, el.top, el.right, el.bottom);
    }

    // Draw the buffer to the screen
    BitBlt(hdc, 0, 0, _client_width, _client_height, compdc, 0, 0, SRCCOPY);

    EndPaint(hWnd, &ps);
}



