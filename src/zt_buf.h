#ifndef __ZT_BUF_H__
#define __ZT_BUF_H__

#include <zt.h>

BEGIN_C_DECLS

struct zt_buf;
typedef struct zt_buf zt_buf_t;
typedef struct zt_iov zt_iov_t;

struct zt_iov {
    size_t iov_len;
    void * iov_data;
};

zt_buf_t * zt_buf_new(void);
void     * zt_buf_get(zt_buf_t * buf);
size_t     zt_buf_unused_sz(zt_buf_t * buf);
size_t     zt_buf_length(zt_buf_t * buf);
int        zt_buf_expand(zt_buf_t * buf, size_t len);
int        zt_buf_add(zt_buf_t * buf, void * data, size_t dlen);
int        zt_buf_add_buf(zt_buf_t * dst, zt_buf_t * src);
size_t     zt_buf_drain(zt_buf_t * buf, size_t len);
size_t     zt_buf_remove(zt_buf_t * buf, void * out, size_t outlen);
int        zt_buf_reserve(zt_buf_t * buf, size_t len);
size_t     zt_buf_iov_len(zt_iov_t * vec, int n_vec);
int        zt_buf_iov_add(zt_buf_t * b, zt_iov_t * vec, int n_vec);
void       zt_buf_free(zt_buf_t * buf);

END_C_DECLS

#endif

