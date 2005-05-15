#ifndef _ZT_ADT_BSTREAM_H_
#define _ZT_ADT_BSTREAM_H_

#include <libzt/zt.h>
BEGIN_C_DECLS
typedef struct zt_bstream zt_bstream;
enum zt_bstream_flags {
	stream_flip_endian 	= 0x01,
	stream_input 		= 0x02,
	stream_output 		= 0x04,
	stream_tagged		= 0x08,
	stream_record		= 0x16,
};


/* predicates */
extern int bstrea_is_input(zt_bstream *);
extern int bstrea_is_output(zt_bstream *);
extern int bstrea_is_tagged(zt_bstream *);
extern int bstream_is_empty(zt_bstream *);
extern int bstream_is_eos(zt_bstream *); /* end of stream */

extern void bstream_close(zt_bstream *);
extern int bstream_truncate(zt_bstream *);
extern zt_bstream * bstream_clone(zt_bstream *);
extern size_t bstream_read(zt_bstream *, void *, size_t, char, char);
extern size_t bstream_write(zt_bstream *, void *, size_t, char, char);

#define bstream_write_byte(bs, buf) \
	bstream_write(bs, buf, 1, sizeof(unsigned char), 0)
#define bstream_read_byte(bs, buf) \
	bstream_read(bs, buf, 1, sizeof(unsigned char), 0)

#define bstream_write_uint8(bs, buf) \
	bstream_write(bs, buf, 1, sizeof(uint8_t), 0)
#define bstream_read_uint8(bs, buf) \
	bstream_read(bs, buf, 1, sizeof(uint8_t), 0)

#define bstream_write_uint16(bs, buf) \
	bstream_write(bs, buf, 1, sizeof(uint16_t), 0)
#define bstream_read_uint16(bs, buf) \
	bstream_read(bs, buf, 1, sizeof(uint16_t), 0)

#define bstream_write_uint32(bs, buf) \
	bstream_write(bs, buf, 1, sizeof(uint32_t), 0)
#define bstream_read_uint32(bs, buf) \
	bstream_read(bs, buf, 1, sizeof(uint32_t), 0)

#define bstream_write_uint64(bs, buf) \
	bstream_write(bs, buf, 1, sizeof(uint64_t), 0)
#define bstream_read_uint64(bs, buf) \
	bstream_read(bs, buf, 1, sizeof(uint64_t), 0)

#define bstream_write_float(bs, buf) \
	bstream_write(bs, buf, 1, sizeof(float), 0)
#define bstream_read_float(bs, buf) \
	bstream_read(bs, buf, 1, sizeof(float), 0)

#define bstream_write_double(bs, buf) \
	bstream_write(bs, buf, 1, sizeof(double), 0)
#define bstream_read_double(bs, buf) \
	bstream_read(bs, buf, 1, sizeof(double), 0)

END_C_DECLS
#endif /* _ZT_ADT_BSTREAM_H_ */
