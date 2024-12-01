#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"
#include "shell.h"
#include "usbd_core.h"
#include "winusb_cdc.h"
#include "ring_buffer.h"

static Shell shell;
static SemaphoreHandle_t shell_mutex;
static char shell_buffer[512];
static ring_buf_t cdc_rb;
static uint8_t cdc_rb_buf[512];

USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t cdc_tx_buf[512];

static EventGroupHandle_t cdc_evt_handle;
static StaticEventGroup_t cdc_evt_group;

#define CDC_EVT_TX_DONE    (0x01 << 0)
#define CDC_EVT_RX_DONE    (0x01 << 1)

static void cdc_rx_cb(uint8_t *data, uint32_t len)
{
    ring_buf_put(&cdc_rb, data, len);

    BaseType_t xHigherPriorityTaskWoken, xResult;
    xHigherPriorityTaskWoken = pdFALSE;

    xResult = xEventGroupSetBitsFromISR(
                                cdc_evt_handle,
                                CDC_EVT_RX_DONE,
                                &xHigherPriorityTaskWoken );
    if( xResult != pdFAIL )
    {
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
}

static void tx_done_cd(void)
{
    BaseType_t xHigherPriorityTaskWoken, xResult;
    xHigherPriorityTaskWoken = pdFALSE;

    xResult = xEventGroupSetBitsFromISR(
                                cdc_evt_handle,
                                CDC_EVT_TX_DONE,
                                &xHigherPriorityTaskWoken );
    if( xResult != pdFAIL )
    {
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
}

static signed short shell_read(char *buf, unsigned short len)
{
    return ring_buf_get(&cdc_rb, (uint8_t *)buf, len);
}

static signed short shell_write(char *buf, unsigned short len)
{
    if(0 == len)
    {
        return 0;
    }
    memcpy(cdc_tx_buf, buf, len);
    cdc_send(cdc_tx_buf, len);

    EventBits_t uxBits = xEventGroupWaitBits(
        cdc_evt_handle,
        CDC_EVT_TX_DONE,
        pdTRUE,
        pdFALSE,
        pdMS_TO_TICKS(1000));
    if(0 == uxBits)
    {
        return 0;
    }

    return len;
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

    xEventGroupWaitBits(
        cdc_evt_handle,
        CDC_EVT_RX_DONE,
        pdTRUE,
        pdFALSE,
        portMAX_DELAY);

    shellInit(&shell, shell_buffer, sizeof(shell_buffer));

    while (1)
    {
        EventBits_t uxBits = xEventGroupWaitBits(
            cdc_evt_handle,
            CDC_EVT_RX_DONE,
            pdTRUE,
            pdFALSE,
            portMAX_DELAY);
        if(0 == uxBits)
        {
            continue;
        }

        while(0 != ring_buf_len(&cdc_rb))
        {
            shellTask(&shell);
        }
    }
}

void shell_task_init(void)
{
    ring_buf_init(&cdc_rb, cdc_rb_buf, sizeof(cdc_rb_buf));
    cdc_evt_handle = xEventGroupCreateStatic(&cdc_evt_group);
    cdc_register_cb(cdc_rx_cb, tx_done_cd);
    xTaskCreate(shell_task, "shell_task", 2048, NULL, 5, NULL);
}
