#ifndef __CDC_TRANS_H__
#define __CDC_TRANS_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void cdc_trans_init(void);
int cdc_trans_send(const uint8_t *buf, uint16_t len);
int cdc_trans_wait_readable(void);
int cdc_trans_read_nonblock(uint8_t *buf, uint16_t len);
int cdc_trans_printf(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* __CDC_TRANS_H__ */
