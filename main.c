#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tasks.h"
#include "winusb_cdc.h"

int main(void)
{
    board_init();
    
    printf("Hello, World!\n");

    extern int filesystem_init(void);
    if(0 != filesystem_init())
    {
        printf("filesystem_init fail\n");
    }

    shell_task_init();

    winusb_cdc_init(0, 0x20072000);
    vTaskStartScheduler();
    return 0;
}
