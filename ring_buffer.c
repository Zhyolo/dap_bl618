#include "ring_buffer.h"
#include <string.h>
#include <stddef.h>

#define min(a,b) ( (a) < (b) )? (a):(b)

bool ring_buf_init(ring_buf_t *r,unsigned char *buf, unsigned int len)
{
    r->buf    = buf;
    r->size   = len;
    r->front  = r->rear = 0;
    return buf != NULL && (len & (len -1)) == 0;
}

void ring_buf_clr(ring_buf_t *r)
{
    r->front = r->rear = 0;
}

unsigned int ring_buf_len(ring_buf_t *r)
{
    return r->rear - r->front;
}

unsigned int ring_buf_put(ring_buf_t *r,unsigned char *buf,unsigned int len)
{
    unsigned int i;
    unsigned int left;
    left = r->size + r->front - r->rear;
    len  = min(len , left);
    i    = min(len, r->size - (r->rear & (r->size - 1)));
    memcpy(r->buf + (r->rear & (r->size - 1)), buf, i);
    memcpy(r->buf, buf + i, len - i);
    r->rear += len;
    return len;

}

unsigned int ring_buf_get(ring_buf_t *r,unsigned char *buf,unsigned int len)
{
    unsigned int i;
    unsigned int left;
    left = r->rear - r->front;
    len  = min(len , left);
    i    = min(len, r->size - (r->front & (r->size - 1)));
    memcpy(buf, r->buf + (r->front & (r->size - 1)), i);
    memcpy(buf + i, r->buf, len - i);
    r->front += len;
    return len;
}
