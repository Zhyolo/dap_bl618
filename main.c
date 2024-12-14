#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tasks.h"
#include "winusb_cdc.h"
#include "cmsis_dap.h"

#include <stdio.h>

int main(void)
{
    board_init();

    extern int filesystem_init(void);
    if(0 != filesystem_init())
    {
        printf("filesystem_init fail\n");
    }

    shell_task_init();
    cmsis_dap_init();

    winusb_cdc_init(0, 0x20072000);
    vTaskStartScheduler();
    return 0;
}
