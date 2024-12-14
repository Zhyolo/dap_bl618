#include "cmsis_dap.h"
#include "usbd_core.h"
#include "winusb_cdc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "DAP_config.h"
#include "DAP.h"

USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t winusb_rx_buf[DAP_PACKET_COUNT][DAP_PACKET_SIZE];
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t winusb_tx_buf[DAP_PACKET_COUNT][DAP_PACKET_SIZE];
static volatile uint8_t rx_head = 0;
static volatile uint8_t rx_head_flag = 0;
static volatile uint8_t rx_tail = 0;
static volatile uint8_t rx_tail_flag = 0;

static volatile uint8_t tx_head = 0;
static volatile uint8_t tx_head_flag = 0;
static volatile uint8_t tx_tail = 0;
static volatile uint8_t tx_tail_flag = 0;

static void winusb_rx_cb(uint32_t len)
{
    if(rx_head_flag != rx_tail_flag && rx_head == rx_tail)
    {
        // full
        return;
    }

    rx_tail++;
    if(rx_tail >= DAP_PACKET_COUNT)
    {
        rx_tail_flag = !rx_tail_flag;
        rx_tail = 0;
    }
    winusb_recv(winusb_rx_buf[rx_tail], sizeof(winusb_rx_buf[rx_tail]));
}

static void tx_done_cb(void)
{

}

static void dap_task(void *pvParameters)
{
    while(1)
    {
        if(rx_head_flag == rx_tail_flag && rx_head == rx_tail)
        {
            // empty
            vTaskDelay(1);
            continue;
        }

    }
}

static void cfg_done(void)
{
    winusb_recv(winusb_rx_buf[rx_tail], sizeof(winusb_rx_buf[rx_tail]));
}

void cmsis_dap_init(void)
{
    winusb_register_cb(winusb_rx_cb, tx_done_cb, cfg_done);
    xTaskCreate(dap_task, "dap_task", 2048, NULL, 5, NULL);
}
