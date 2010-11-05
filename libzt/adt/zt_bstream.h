#ifndef _ZT_ADT_BSTREAM_H_
#define _ZT_ADT_BSTREAM_H_

#include <stdint.h>

#include <zt_internal.h>
#include <adt/zt_array.h>

BEGIN_C_DECLS
typedef struct zt_bstream *zt_bstream_t;
struct zt_bstream {
    int      flipendian;
    size_t   offt;
    zt_array_t data;
};


/* predicates */
extern int zt_bstream_is_empty(zt_bstream_t bs);
extern int zt_bstream_truncate(zt_bstream_t bs);
extern void zt_bstream_set_data(zt_bstream_t bs, char *data, int len, char copy);
extern void zt_bstream_set_array(zt_bstream_t bs, zt_array_t array, size_t offt);
extern int zt_bstream_rewind(zt_bstream_t bs);

extern zt_bstream_t  zt_bstream_new(void);
extern zt_bstream_t  zt_bstream_clone(zt_bstream_t bs);
extern void zt_bstream_free(zt_bstream_t *bs);

extern size_t zt_bstream_read(zt_bstream_t bs, char *buf, size_t len, char size, char tag);
extern void zt_bstream_read_byte(zt_bstream_t bs, char *data);
extern void zt_bstream_read_uint8(zt_bstream_t bs, uint8_t *data);
extern void zt_bstream_read_uint16(zt_bstream_t bs, uint16_t *data);
extern void zt_bstream_read_uint32(zt_bstream_t bs, uint32_t *data);
extern void zt_bstream_read_uint64(zt_bstream_t bs, uint64_t *data);
extern void zt_bstream_read_float(zt_bstream_t bs, float *data);
extern void zt_bstream_read_double(zt_bstream_t bs, double *data);

extern void zt_write_bytes(char *s, char *d, size_t len, int flip);

extern size_t zt_bstream_write(zt_bstream_t bs, char *data, size_t len, char szie, char tag);
extern void zt_bstream_write_byte(zt_bstream_t bs, char data);
extern void zt_bstream_write_uint8(zt_bstream_t bs, uint8_t data);
extern void zt_bstream_write_uint16(zt_bstream_t bs, uint16_t data);
extern void zt_bstream_write_uint32(zt_bstream_t bs, uint32_t data);
extern void zt_bstream_write_uint64(zt_bstream_t bs, uint64_t data);
extern void zt_bstream_write_float(zt_bstream_t bs, float data);
extern void zt_bstream_write_double(zt_bstream_t bs, double data);

END_C_DECLS
#endif /* _ZT_ADT_ZT_BSTREAM_H_ */
