#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "shell.h"
#include "cdc_trans.h"

static Shell shell;
static SemaphoreHandle_t shell_mutex;
static char shell_buffer[512];

static signed short shell_read(char *buf, unsigned short len)
{
    cdc_trans_wait_readable();
    return cdc_trans_read_nonblock((uint8_t *)buf, len);
}

signed short shell_write(char *buf, unsigned short len)
{
    return cdc_trans_send((uint8_t *)buf, len);
}

static int shell_lock(Shell *shell)
{
    xSemaphoreTakeRecursive(shell_mutex, portMAX_DELAY);
    return 0;
}

static int shell_unlock(Shell *shell)
{
    xSemaphoreGiveRecursive(shell_mutex);
    return 0;
}

static void shell_task(void *pvParameters)
{
    shell_mutex = xSemaphoreCreateMutex();
    shell.read = shell_read;
    shell.write = shell_write;
    shell.lock = shell_lock;
    shell.unlock = shell_unlock;

    cdc_trans_wait_readable();
    shellInit(&shell, shell_buffer, sizeof(shell_buffer));
    while (1)
    {
        shellTask(&shell);
    }
}

void shell_task_init(void)
{
    cdc_trans_init();
    xTaskCreate(shell_task, "shell_task", 2048, NULL, 5, NULL);
}
