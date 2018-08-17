#ifndef __ZT_SHA1_H__
#define __ZT_SHA1_H__

#include <zt.h>
#include <libzt/zt_stdint.h>

BEGIN_C_DECLS

typedef struct zt_sha1_ctx zt_sha1_ctx;
struct zt_sha1_ctx {
    uint32_t state[5];
    uint32_t count[2];
    uint8_t  buffer[64];
};


/* standard API for incremental building a sha1 hash */
extern void zt_sha1_init(zt_sha1_ctx *ctx);
extern void zt_sha1_update(zt_sha1_ctx *ctx, uint8_t *data, size_t len);
extern void zt_sha1_finalize(zt_sha1_ctx * ctx, uint8_t digest[20]);

/* build a sha1 hash of a fixed amount of data */
extern void     zt_sha1_data(void *data, size_t len, uint8_t digest[20]);
extern char    *zt_sha1_tostr(uint8_t digest[20], char sha1[41]);
extern uint8_t *zt_str_tosha1(char sha1[41], uint8_t digest[20]);

END_C_DECLS
#endif    /* __ZT_SHA1_H__ */
