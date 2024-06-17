/*
 * ckeys_stub.h
 * 
 * Empty CKeys functions, for test compilation on non-Linux systems.
 * Also serves as a quick overview of the platform specific functions.
 */
#pragma once
#pragma GCC diagnostic ignored "-Wunused-parameter"


#define SLEEP_MS(ms)


void Mouse_SetPos(int x, int y) {}
void Mouse_LeftClick(void) {}
void Mouse_RightClick(void) {}


/* Gets the color of the pixel at the specified position and
   stores it in buf as a string in the format "RRGGBB" */
void Pixel_ColorAtPos(int x, int y, char* buf) {}


void Key_Down(int keycode) {}
void Key_Up(int keycode) {}
void Key_WriteString(const char* str) {}


bool Key_IsPressed(int keycode) {
    return false;
}


/* Updates _mouse_p with current information */
void _mouse_update(void) {
    _mouse_p->screen = 0;
    _mouse_p->window = 0;
    _mouse_p->x = 0;
    _mouse_p->y = 0;
}


void _mouse_init(void) {
    _mouse_p = _xcalloc(1, sizeof *_mouse_p);
    _mouse_update();
}


/*
 * Returns the time elapsed since the last call,
 * requires an initialization call to return any meaningful value.
 *
 * Basically just a typical delta time function.
 */
double CKeys_TimeElapsed(void) {
    return 0;
}


/* Initializes everything needed for the library */
void CKeys_Init(void) {
    _mouse_init();
}

