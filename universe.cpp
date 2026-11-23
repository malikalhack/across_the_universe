#include "universe.h"

#include <ctime>
#include <cmath>
#include <stdexcept>

constexpr int SPEED = 15;

Universe::Universe(int32_t width, int32_t height)
    :_client_width(width),
     _client_height(height),
     _D((_client_width + _client_height) / 2),
     _oX(_client_width / 2),
     _oY(_client_height / 2)
{
    if (_client_width <= 0 || _client_height <= 0) {
        throw std::invalid_argument("Universe dimensions must be positive");
    }

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (int i = 0; i < STAR_COUNT; i++) {
        point3d[i].x = std::rand() % _client_width - _oX;
        point3d[i].y = std::rand() % _client_height - _oY;
        point3d[i].z = std::rand() % _client_width - _oX;
    }
}

void Universe::_MoveStars(SEllipse_t* const el, uint16_t star_number) {
    float x, y, k, star_radius;

    point3d[star_number].z = point3d[star_number].z - SPEED;

    if (!(point3d[star_number].z + _D)) {
        point3d[star_number].z = 0;
    }

    if (point3d[star_number].z <= -_D) {
        point3d[star_number].z = 0;
    }

    k = static_cast<float>(_D) / (point3d[star_number].z + _D);

    x = point3d[star_number].x * k + _oX;
    y = point3d[star_number].y * k + _oY;

    star_radius = point3d[star_number].z / 200.f;

    el->left = static_cast<int>(std::floor(x - star_radius));
    el->top = static_cast<int>(std::floor(y - star_radius));
    el->right = static_cast<int>(std::floor(x + star_radius));
    el->bottom = static_cast<int>(std::floor(y + star_radius));

}

bool Universe::Draw(SDL_Renderer* renderer) {
    if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255) != 0 ||
        SDL_RenderClear(renderer) != 0 ||
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255) != 0) {
        return false;
    }

    SEllipse_t el;
    for (uint16_t i = 0; i < STAR_COUNT; i++) {
        _MoveStars(&el, i);
        if (el.right <= el.left || el.bottom <= el.top) {
            continue;
        }

        SDL_Rect rect{
            el.left,
            el.top,
            el.right - el.left,
            el.bottom - el.top
        };
        if (SDL_RenderFillRect(renderer, &rect) != 0) {
            return false;
        }
    }

    return true;
}



