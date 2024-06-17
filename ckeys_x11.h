/*
 * ckeys_x11.h
 *
 * X11 specific CKeys functions
 */
#pragma once


#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>


#define SLEEP_MS(ms) usleep(1000 * ms)
#define XDOTOOL_PATH "/usr/bin/xdotool"
#define GRABC_PATH "/usr/bin/grabc"


size_t Mouse_GetWindow(void);
void _mouse_init(void);


/* X11 display */
Display* _display_p = NULL;


void Mouse_SetPos(int x, int y) {
    char cmd[64];
    snprintf(cmd, sizeof cmd, XDOTOOL_PATH " mousemove %d %d", x, y);
    system(cmd);
}


void Mouse_LeftClick(void) {
    system(XDOTOOL_PATH " click 1");
}


void Mouse_RightClick(void) {
    system(XDOTOOL_PATH " click 3");
}


void Pixel_ColorAtPos(int x, int y, char* buf) {
    char cmd[128];
    snprintf(cmd, sizeof cmd, GRABC_PATH " -l +%d+%d -w 0x%lx", x, y, Mouse_GetWindow());

    FILE* fp = _popen_err_checked(cmd, "r");

    if (fgets(buf, 8, fp) == NULL)
        _fprintf_exit("fgets");

    _pclose_err_checked(fp);
}


void Key_Down(int keycode) {
    char cmd[64];
    snprintf(cmd, sizeof cmd, XDOTOOL_PATH " keydown 0x%x", keycode);
    system(cmd);
}


void Key_Up(int keycode) {
    char cmd[64];
    snprintf(cmd, sizeof cmd, XDOTOOL_PATH " keyup 0x%x", keycode);
    system(cmd);
}


void Key_WriteString(const char* str) {
    char* cmd = _xcalloc(1, 64 + strlen(str));
    snprintf(cmd, 64 + strlen(str), XDOTOOL_PATH " type %s", str);
    system(cmd);
    free(cmd);
}


bool Key_IsPressed(int keycode) {
    char keys_return[32];
    XQueryKeymap(_display_p, keys_return);
    KeyCode kc2 = XKeysymToKeycode(_display_p, keycode);
    return (keys_return[kc2>>3] & (1<<(kc2&7)));
}


void _mouse_update(void) {
    if (_mouse_p == NULL)
        _fprintf_exit("Mouse struct not initialized");

    FILE* fp = _popen_err_checked(XDOTOOL_PATH " getmouselocation", "r");
    fscanf(fp, "x: %d y: %d screen: %d window: %lu", &_mouse_p->x, &_mouse_p->y, &_mouse_p->screen, &_mouse_p->window);
    _pclose_err_checked(fp);
}


void _mouse_init(void) {
    _mouse_p = _xcalloc(1, sizeof *_mouse_p);
    _mouse_update();
}


/*
 * Returns the time elapsed since the last call,
 * requires an initialization call to return any meaningful value.
 *
 * This implementation is not X11 specific, but it's included here until
 * support for Wayland is added.
 */
double CKeys_TimeElapsed(void) {
    static struct timespec start = {0};
    struct timespec now;

    if (clock_gettime(CLOCK_MONOTONIC, &now) == -1)
        _perror_exit("clock_gettime");

    double elapsed = (now.tv_sec - start.tv_sec) + (now.tv_nsec - start.tv_nsec) / 1e9;
    start = now;
    return elapsed;
}


/* Initializes the CKeys library, must be called before any other function */
void CKeys_Init(void) {
    _mouse_init();
    _display_p = XOpenDisplay(NULL);

    if (_display_p == NULL)
        _fprintf_exit("XOpenDisplay failed");
}

