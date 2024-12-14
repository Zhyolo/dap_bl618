#include "cdc_trans.h"
#include "usbd_core.h"
#include "winusb_cdc.h"
#include "ring_buffer.h"
#include "FreeRTOS.h"
#include "event_groups.h"
#include <stdio.h>
#include <stdarg.h>

static ring_buf_t cdc_rb;
static uint8_t cdc_rb_buf[512];
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t cdc_rx_buf[512];
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t cdc_tx_buf[512];

static EventGroupHandle_t cdc_evt_handle;
static StaticEventGroup_t cdc_evt_group;

#define CDC_EVT_TX_DONE    (0x01 << 0)
#define CDC_EVT_RX_DONE    (0x01 << 1)

static void cdc_rx_cb(uint32_t len)
{
    ring_buf_put(&cdc_rb, cdc_rx_buf, len);

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

    cdc_recv(cdc_rx_buf, sizeof(cdc_rx_buf));
}

static void tx_done_cb(void)
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

static void cfg_done(void)
{
    cdc_recv(cdc_rx_buf, sizeof(cdc_rx_buf));
}

void cdc_trans_init(void)
{
    ring_buf_init(&cdc_rb, cdc_rb_buf, sizeof(cdc_rb_buf));
    cdc_evt_handle = xEventGroupCreateStatic(&cdc_evt_group);
    cdc_register_cb(cdc_rx_cb, tx_done_cb, cfg_done);
}

int cdc_trans_send(const uint8_t *buf, uint16_t len)
{
    if(0 == len || NULL == buf)
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

int cdc_trans_wait_readable(void)
{
    if(0 != ring_buf_len(&cdc_rb))
    {
        return ring_buf_len(&cdc_rb);
    }

    EventBits_t uxBits = xEventGroupWaitBits(
        cdc_evt_handle,
        CDC_EVT_RX_DONE,
        pdTRUE,
        pdFALSE,
        portMAX_DELAY);
    if(0 == uxBits)
    {
        return 0;
    }

    return ring_buf_len(&cdc_rb);
}

int cdc_trans_read_nonblock(uint8_t *buf, uint16_t len)
{
    if(0 == len || NULL == buf)
    {
        return 0;
    }
    
    if(0 == ring_buf_len(&cdc_rb))
    {
        return 0;
    }
    
    return ring_buf_get(&cdc_rb, (uint8_t *)buf, len);
}

int cdc_trans_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf((char *)cdc_tx_buf, sizeof(cdc_tx_buf), fmt, args);
    va_end(args);

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
