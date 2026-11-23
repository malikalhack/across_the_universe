/**
 * @file    universe.c
 * @version 1.0.0
 * @authors Anton Chernov
 * @date    2026-11-24
 * @date    @showdate "%Y-%m-%d"
 */

/******************************** Included files ******************************/

#include <cmath>
#include <ctime>
#include <stdexcept>

#include "universe.h"

/********************************* Definitions ********************************/

constexpr int SPEED = 8;
constexpr float MAX_TRAIL_LENGTH = 6.f;
constexpr uint8_t STAR_FADE_FRAMES = 4;

static bool DrawAntialiasedPoint(
    SDL_Renderer *renderer,
    const SDL_FPoint &point,
    uint8_t brightness
) {
    const int x = static_cast<int>(std::floor(point.x));
    const int y = static_cast<int>(std::floor(point.y));
    const float fraction_x = point.x - x;
    const float fraction_y = point.y - y;
    const float coverage[] = {
        (1.f - fraction_x) * (1.f - fraction_y),
        fraction_x * (1.f - fraction_y),
        (1.f - fraction_x) * fraction_y,
        fraction_x * fraction_y
    };
    const int point_x[] = { x, x + 1, x, x + 1 };
    const int point_y[] = { y, y, y + 1, y + 1 };

    for (int i = 0; i < 4; i++) {
        const uint8_t point_brightness = static_cast<uint8_t>(
            std::lround(brightness * coverage[i])
        );
        if (point_brightness == 0) {
            continue;
        }
        if (SDL_SetRenderDrawColor(
                renderer,
                point_brightness,
                point_brightness,
                point_brightness,
                255
            ) != 0 ||
            SDL_RenderDrawPoint(renderer, point_x[i], point_y[i]) != 0) {
            return false;
        }
    }

    return true;
}

/******************************* Class definition *****************************/

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
    for (uint16_t i = 0; i < STAR_COUNT; i++) {
        _ResetStar(i);
        point3d[i].fade = STAR_FADE_FRAMES;
    }
}
/*----------------------------------------------------------------------------*/

void Universe::_ResetStar(uint16_t star_number) {
    const int min_depth = _D / 2 > 0 ? _D / 2 : 1;
    const int depth_range = _D - min_depth + 1;
    const int depth = min_depth + std::rand() % depth_range;
    const int screen_x = std::rand() % _client_width - _oX;
    const int screen_y = std::rand() % _client_height - _oY;

    point3d[star_number].x =
        static_cast<int>((static_cast<int64_t>(screen_x) * depth) / _D);
    point3d[star_number].y =
        static_cast<int>((static_cast<int64_t>(screen_y) * depth) / _D);
    if (point3d[star_number].x == 0 && point3d[star_number].y == 0) {
        point3d[star_number].x = 1;
    }
    point3d[star_number].z = depth;
    point3d[star_number].fade = 0;
}
/*----------------------------------------------------------------------------*/

bool Universe::_MoveStar(
    SDL_FPoint * const previous_point,
    SDL_FPoint * const screen_point,
    uint16_t star_number
) {
    const float previous_scale =
        static_cast<float>(_D) / point3d[star_number].z;
    previous_point->x = point3d[star_number].x * previous_scale + _oX;
    previous_point->y = point3d[star_number].y * previous_scale + _oY;

    point3d[star_number].z -= SPEED;
    if (point3d[star_number].z <= 0) {
        _ResetStar(star_number);
        screen_point->x = point3d[star_number].x + _oX;
        screen_point->y = point3d[star_number].y + _oY;
        return false;
    }
    if (point3d[star_number].fade < STAR_FADE_FRAMES) {
        point3d[star_number].fade++;
    }

    const float scale = static_cast<float>(_D) / point3d[star_number].z;

    screen_point->x = point3d[star_number].x * scale + _oX;
    screen_point->y = point3d[star_number].y * scale + _oY;

    return true;
}
/*----------------------------------------------------------------------------*/

bool Universe::Draw(SDL_Renderer *renderer) {
    if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255) != 0 ||
        SDL_RenderClear(renderer) != 0 ||
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255) != 0) {
        return false;
    }

    SDL_FPoint previous_point;
    SDL_FPoint screen_point;
    for (uint16_t i = 0; i < STAR_COUNT; i++) {
        const bool continued = _MoveStar(&previous_point, &screen_point, i);
        if (screen_point.x < 0.f || screen_point.x >= _client_width ||
            screen_point.y < 0.f || screen_point.y >= _client_height) {
            continue;
        }

        const uint8_t brightness = static_cast<uint8_t>(
            255 * point3d[i].fade / STAR_FADE_FRAMES
        );
        if (continued) {
            const float dx = screen_point.x - previous_point.x;
            const float dy = screen_point.y - previous_point.y;
            const float length = std::hypot(dx, dy);
            if (length < 2.f) {
                if (!DrawAntialiasedPoint(renderer, screen_point, brightness)) {
                    return false;
                }
                continue;
            }
            if (length > MAX_TRAIL_LENGTH) {
                previous_point.x = screen_point.x - dx * MAX_TRAIL_LENGTH / length;
                previous_point.y = screen_point.y - dy * MAX_TRAIL_LENGTH / length;
            }
        } else {
            previous_point = screen_point;
            if (!DrawAntialiasedPoint(renderer, screen_point, brightness)) {
                return false;
            }
            continue;
        }

        if (SDL_SetRenderDrawColor(renderer, brightness, brightness, brightness, 255) != 0) {
            return false;
        }

        if (SDL_RenderDrawLine(
                renderer,
                static_cast<int>(std::lround(previous_point.x)),
                static_cast<int>(std::lround(previous_point.y)),
                static_cast<int>(std::lround(screen_point.x)),
                static_cast<int>(std::lround(screen_point.y))
            ) != 0) {
            return false;
        }
    }

    return true;
}
/******************************************************************************/
