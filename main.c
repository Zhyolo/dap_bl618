#include "bflb_mtimer.h"
#include "board.h"

#define DBG_TAG "MAIN"
#include "log.h"

int main(void)
{
    board_init();

    printf("hello world\r\n");

    extern void winusbv2_init(uint8_t busid, uintptr_t reg_base);
    winusbv2_init(0, 0x20072000);
    while (1) {
    }
}
