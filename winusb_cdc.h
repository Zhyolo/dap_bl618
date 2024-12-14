#ifndef __WINUSB_CDC_H__
#define __WINUSB_CDC_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void winusb_cdc_init(uint8_t busid, uintptr_t reg_base);

void winusb_register_cb(void (*rx_cb)(uint32_t len), void (*tx_done)(void), void (*cfg_done)(void));
void winusb_send(uint8_t *buf, uint32_t len);
void winusb_recv(uint8_t *buf, uint32_t len);

void cdc_register_cb(void (*rx_cb)(uint32_t len), void (*tx_done)(void), void (*cfg_done)(void));
void cdc_send(uint8_t *buf, uint32_t len);
void cdc_recv(uint8_t *buf, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* __WINUSB_CDC_H__ */
