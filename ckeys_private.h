/*
 * ckeys_private.h
 *
 * Stuff and things intended only for use by the implementation.
 * The user should not use any of this, even though the file has to
 * be included in the main CKeys header.
 */
#pragma once


void _mouse_update(void);


/* Mouse struct, used only by the implementation */
struct _Mouse {
    int    x;
    int    y;
    int    screen;
    size_t window;
};


struct _Mouse* _mouse_p = NULL;


void _perror_exit(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}


void _fprintf_exit(const char* msg) {
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}


FILE* _popen_err_checked(const char* cmd, const char* type) {
    FILE* fp = popen(cmd, type);

    if (fp == NULL)
        _perror_exit("popen");

    return fp;
}


void _pclose_err_checked(FILE* fp) {
    if (pclose(fp) == -1)
        _perror_exit("pclose");
}


void* _xcalloc(size_t nmemb, size_t size) {
    void* ptr = calloc(nmemb, size);

    if (ptr == NULL)
        _perror_exit("calloc");

    return ptr;
}


struct _Mouse* _mouse_get(void) {
    _mouse_update();
    return _mouse_p;
}

