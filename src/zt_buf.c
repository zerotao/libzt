#include <stdio.h>

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STRING_H
# include <string.h>
#endif /* HAVE_STRING_H */

#include "zt.h"

struct zt_buf {
    size_t len;
    size_t max;
    void * buf;
    void * top;
};

zt_buf_t *
zt_buf_new(void) {
    zt_buf_t * b;

    if (!(b = zt_malloc(zt_buf_t, 1))) {
        return NULL;
    }

    b->len = 0;
    b->max = 0;
    b->buf = NULL;
    b->top = NULL;

    return b;
}

void *
zt_buf_get(zt_buf_t * b) {
    return b->len ? b->buf : NULL;
}

size_t
zt_buf_unused_sz(zt_buf_t * b) {
    return (size_t)(b->max - b->len);
}

size_t
zt_buf_length(zt_buf_t * b) {
    return b->len;
}

#define ZT_BUF_MIN 1024

int
zt_buf_expand(zt_buf_t * b, size_t len) {
    size_t rlen;

    if (len < ZT_BUF_MIN) {
        rlen = ZT_BUF_MIN;
    } else {
        rlen = len;
    }

    if (b->top == NULL) {
        b->top = b->buf;
    }

    b->buf = realloc(b->buf, b->len + rlen);
    b->top = b->buf;
    b->max = b->len + rlen;

    return 0;
}

size_t
zt_buf_drain(zt_buf_t * b, size_t len) {
    if (len == 0) {
        return 0;
    }

    if (len < b->len) {
        b->len -= len;
        b->max -= (size_t)((char *)b->top - (char *)b->buf);
        b->buf  = ((char *)b->buf + len);
    } else {
        b->len = 0;
    }

    return b->len;
}

int
zt_buf_add(zt_buf_t * b, void * data, size_t len) {
    if (len > zt_buf_unused_sz(b)) {
        zt_buf_expand(b, len);
    }

    memcpy((void *)((char *)b->buf + b->len), data, len);
    b->len += len;

    return 0;
}

int
zt_buf_add_buf(zt_buf_t * a, zt_buf_t * b) {
    zt_buf_add(a, b->buf, b->len);
    zt_buf_drain(b, b->len);
    return 0;
}

size_t
zt_buf_remove(zt_buf_t * b, void * out, size_t olen) {
    if (olen > b->len) {
        olen = b->len;
    }

    memcpy(out, b->buf, olen);
    zt_buf_drain(b, olen);

    return olen;
}

int
zt_buf_reserve(zt_buf_t * b, size_t len) {
    if (len > zt_buf_unused_sz(b)) {
        zt_buf_expand(b, len);
    }

    return 0;
}

size_t
zt_buf_iov_len(zt_iov_t * vec, int n_vec) {
    int    i;
    size_t len;

    len = 0;

    for (i = 0; i < n_vec; i++) {
        len += vec[i].iov_len;
    }

    return len;
}

int
zt_buf_iov_add(zt_buf_t * b, zt_iov_t * vec, int n_vec) {
    int    i;
    size_t tlen;

    tlen = zt_buf_iov_len(vec, n_vec);
    zt_buf_reserve(b, tlen);

    for (i = 0; i < n_vec; i++) {
        zt_buf_add(b, vec[i].iov_data, vec[i].iov_len);
    }

    return 0;
}

void
zt_buf_free(zt_buf_t * b) {
    if (!b) {
        return;
    }

    zt_free(b->top);
    zt_free(b);
}

