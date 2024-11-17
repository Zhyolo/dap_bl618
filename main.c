#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tasks.h"
#include "winusb_cdc.h"

int main(void)
{
    board_init();

    shell_task_init();

    winusb_cdc_init(0, 0x20072000);
    printf("Hello, World!\n");
    vTaskStartScheduler();
    return 0;
}
