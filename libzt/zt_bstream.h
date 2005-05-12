#ifndef _ZT_STREAM_H_
#define _ZT_STREAM_H_

#include <libzt/zt.h>
#include <libzt/adt/zt_list.h>
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
	size_t		  offset;
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
	void		(* empty)(zt_bstream *);
	int		(* is_empty)(zt_bstream *);
	zt_bstream *	(* clone)(zt_bstream *);
};

END_C_DECLS

#include <libzt/zt_bstream/zt_bstream_interface.h>

#endif  /* _ZT_STREAM_H_ */
