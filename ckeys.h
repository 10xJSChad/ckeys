/*
 * ckeys.h
 *
 * Main CKeys header, includes all other headers and contains
 * all of the platform agnostic public functions.
 */
#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "ckeys_private.h"


#ifdef CKEYS_X11
#include "ckeys_x11.h"
#endif


#ifdef CKEYS_WINDOWS
#include "ckeys_windows.h"
#endif


#ifdef CKEYS_STUB
#include "ckeys_stub.h"
#endif


#define LERP(a, b, t) ((a) + (t) * ((b) - (a)))
#define RAND_RANGE(min, max) ((rand() % ((max) - (min)) + (min)))


struct Vec2 {
    int x;
    int y;
};


bool Vec2_IsWithinTolerance(struct Vec2 a, struct Vec2 b, double tolerance) {
    /* lazy divide by zero check, feels like doing this may have
       consequences one day but I'll take the gamble. */
    if (a.x == 0 || a.y == 0 || b.x == 0 || b.y == 0)
        return false;

    return (abs(a.x - b.x) / ((a.x + b.x) / 2) * 100 <= tolerance)
        && (abs(a.y - b.y) / ((a.y + b.y) / 2) * 100 <= tolerance);
}


struct Vec2 Vec2_Lerp(struct Vec2 a, struct Vec2 b, float t) {
    struct Vec2 result = {LERP(a.x, b.x, t), LERP(a.y, b.y, t)};
    return result;
}


int Mouse_GetX(void) {
    return _mouse_get()->x;
}


int Mouse_GetY(void) {
    return _mouse_get()->y;
}


void Mouse_GetPos(int* x, int* y) {
    if (x != NULL) *x = Mouse_GetX();
    if (y != NULL) *y = Mouse_GetY();
}


struct Vec2 Mouse_GetPos_Vec2(void) {
    struct Vec2 pos = {Mouse_GetX(), Mouse_GetY()};
    return pos;
}


int Mouse_GetScreen(void) {
    return _mouse_get()->screen;
}


size_t Mouse_GetWindow(void) {
    return _mouse_get()->window;
}


void Mouse_SetPos_Vec2(struct Vec2 pos) {
    Mouse_SetPos(pos.x, pos.y);
}


void Mouse_SetX(int x) {
    Mouse_SetPos_Vec2((struct Vec2) {x, Mouse_GetY()});
}


void Mouse_SetY(int y) {
    Mouse_SetPos_Vec2((struct Vec2) {Mouse_GetX(), y});
}


void Mouse_PrintPos(void) {
    printf("Mouse position: (%d, %d)\n", Mouse_GetX(), Mouse_GetY());
}


/*
 * Moves the mouse to the specified position, immediately stops and returns
 * false if the mouse was moved outside the tolerance.
 *
 * Does not adjust movement speed to account for inconsistent scheduling.
 * Use Mouse_LerpTo if consistent movement speed from point A to B is desired.
 */
bool Mouse_MoveTo(int x, int y, int speed, double tolerance) {
    struct Vec2 start_pos = Mouse_GetPos_Vec2();
    struct Vec2 new_pos = start_pos;

    int dist = sqrt(pow(x - start_pos.x, 2) + pow(y - start_pos.y, 2));
    int steps = dist / speed;

    for (int i = 0; i < steps; i++) {
        if (!Vec2_IsWithinTolerance(new_pos, Mouse_GetPos_Vec2(), tolerance))
            return false;

        new_pos = Vec2_Lerp(start_pos, (struct Vec2) {x, y}, (float) i / steps);
        Mouse_SetPos_Vec2(new_pos);
    }

    Mouse_SetPos(x, y);
    return true;
}


/* Moves the mouse to the specified position, using linear interpolation,
   returns false if the mouse was moved outside the tolerance */
bool Mouse_LerpTo(int x, int y, int speed, double tolerance) {
    double alpha = 0;
    struct Vec2 start_pos = Mouse_GetPos_Vec2();
    struct Vec2 new_pos = start_pos;
    struct Vec2 dest_pos = {x, y};

    CKeys_TimeElapsed();

    while (alpha < 1) {
        if (!Vec2_IsWithinTolerance(new_pos, Mouse_GetPos_Vec2(), tolerance))
            return false;

        new_pos = Vec2_Lerp(start_pos, dest_pos, alpha);
        Mouse_SetPos_Vec2(new_pos);
        alpha += speed * CKeys_TimeElapsed();
    }

    Mouse_SetPos_Vec2(dest_pos);
    return true;
}


void Pixel_ColorAtMouse(char* buf) {
    return Pixel_ColorAtPos(Mouse_GetX(), Mouse_GetY(), buf);
}


bool Pixel_ColorAtPosEquals(int x, int y, const char* color) {
    char buf[8];
    Pixel_ColorAtPos(x, y, buf);
    return strcmp(buf, color) == 0;
}


bool Pixel_ColorAtMouseEquals(const char* color) {
    return Pixel_ColorAtPosEquals(Mouse_GetX(), Mouse_GetY(), color);
}


void Key_Press(int keycode, int delay) {
    Key_Down(keycode);
    SLEEP_MS(delay);
    Key_Up(keycode);
}


/* I honestly see no reason to have this function at all,
   but let's keep it for funsies */
void CKeys_Free(void) {
    free(_mouse_p);
    _mouse_p = NULL;
}
