#ifndef _ZT_BSTREAM_H_
#define _ZT_BSTREAM_H_

#include <libzt/zt.h>
#include <libzt/adt/zt_array.h>

BEGIN_C_DECLS

/* This is here for optimization purposes
 * you should not access this directly unless
 * you are extending the component
 */

typedef struct zt_bstream zt_bstream;
struct zt_bstream {
	struct zt_bstream_vtbl *vtbl;
	
	/* rest of opts */
	int		  flags;
	zt_array	  data;
};

enum zt_bstream_flags {
	stream_flip_endian = 0x01,
};

typedef struct zt_bstream_vtbl zt_bstream_vtbl;
struct zt_bstream_vtbl {
	size_t size;
	/* virtual function pointers */
	void		(* close)(zt_bstream *);
	int		(* truncate)(zt_bstream *);
	int		(* empty)(zt_bstream *);
	zt_bstream *	(* clone)(zt_bstream *);
	size_t		(* read)(zt_bstream *, void *, size_t, char, char);
	size_t		(* write)(zt_bstream *, void *, size_t, char, char);
};

extern void bstream_close(zt_bstream *);
extern int bstream_truncate(zt_bstream *);

extern int bstream_empty(zt_bstream *);
extern int bstream_eos(zt_bstream *); /* end of stream */
extern zt_bstream * bstream_clone(zt_bstream *);

extern size_t bstream_read(zt_bstream *, void *, size_t, char, char);
extern size_t bstream_write(zt_bstream *, void *, size_t, char, char);

/* writers */
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

#include <libzt/zt_bstream/zt_bstream_interface.h>

#endif  /* _ZT_BSTREAM_H_ */
