#include "universe.h"

#define SPEED (10u)


Universe::Universe()
    :_client_width(GetSystemMetrics(SM_CXSCREEN)),
     _client_height(GetSystemMetrics(SM_CYSCREEN)),
     _D((_client_width + _client_height) / 2),
     _oX(_client_width / 2),
     _oY(_client_height / 2)
{
    for (int i = 0; i < STAR_COUNT; i++) {
        point3d[i].x = (rand() % _client_width)  - _oX;
        point3d[i].y = (rand() % _client_height) - _oY;
        point3d[i].z = (rand() % _client_width)  - _oX;
    }
}

Universe ::~Universe() = default;

void Universe::_MoveStars(SEllipse_t * const el, const uint16_t star_number) {
    int x, y, k, rPoint;

    point3d[star_number].z = point3d[star_number].z - SPEED;

    if (point3d[star_number].z <= -_D || (point3d[star_number].z + _D) == 0) {
        point3d[star_number].z = 0;
    }

    k = _D / (point3d[star_number].z + _D);

    x = _oX + point3d[star_number].x * k;
    y = _oY + point3d[star_number].y * k;
    rPoint = point3d[star_number].z / 200;

    el->left = x - rPoint;
    el->top = y - rPoint;
    el->right = x + rPoint;
    el->bottom = y + rPoint;

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
    for (int i = 0; i < STAR_COUNT; i++) {
        _MoveStars(&el, i);
        Ellipse(compdc, el.left, el.top, el.right, el.bottom);
    }

    // Draw the buffer to the screen
    BitBlt(hdc, 0, 0, _client_width, _client_height, compdc, 0, 0, SRCCOPY);

    EndPaint(hWnd, &ps);
}



