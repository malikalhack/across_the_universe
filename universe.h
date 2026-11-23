#ifndef UNIVERSE_H
#define UNIVERSE_H

#include <SDL.h>
#include <stdint.h>

#define STAR_COUNT (1000u)

typedef struct SPoint3d {
    int x;
    int y;
    int z;
}SPoint3d_t;

typedef struct SEllipse {
    int left;
    int top;
    int right;
    int bottom;
}SEllipse_t;


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

    void _MoveStars(SEllipse_t* const el, uint16_t star_number);
};

#endif // !UNIVERSE_H
