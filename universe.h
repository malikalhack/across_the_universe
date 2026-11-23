/**
 * @file    universe.h
 * @version 1.0.0
 * @authors Anton Chernov
 * @date    2026-11-24
 * @date    @showdate "%Y-%m-%d"
 */

#ifndef UNIVERSE_H
#define UNIVERSE_H
/******************************** Included files ******************************/
#include <SDL.h>
#include <stdint.h>

/********************************* Definitions ********************************/
#define STAR_COUNT (600u)

typedef struct SPoint3d {
    int x;
    int y;
    int z;
    uint8_t fade;
}SPoint3d_t;

/****************************** Class declaration *****************************/

class Universe {
public:
    Universe(int32_t width, int32_t height);
    
    bool Draw(SDL_Renderer* renderer);

private:
    const int32_t _client_width;
    const int32_t _client_height;

    const int32_t _D;
    const int32_t _oX;
    const int32_t _oY;

    SPoint3d_t point3d[STAR_COUNT];

    void _ResetStar(uint16_t star_number);
    bool _MoveStar(
        SDL_FPoint * const previous_point,
        SDL_FPoint * const screen_point,
        uint16_t star_number
    );
};
/******************************************************************************/
#endif // !UNIVERSE_H
