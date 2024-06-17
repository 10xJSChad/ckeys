#define CKEYS_STUB
#include "ckeys.h"


int main(void) {
    CKeys_Init();
    Mouse_SetPos(100, 100);
    Mouse_PrintPos();
    return 0;
}
