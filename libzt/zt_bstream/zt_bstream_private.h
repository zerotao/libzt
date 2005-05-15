/* Private interface to zt_bstream
 * You should NOT access this directly
 * unless you are extending the component
 */
#ifndef _ZT_STREAM_PRIVATE_H_
#define _ZT_STREAM_PRIVATE_H_

#include <stdlib.h>
#include <libzt/zt_bstream.h>
#include <libzt/adt/zt_array.h>

BEGIN_C_DECLS

struct zt_bstream {
	struct zt_bstream_vtbl *vtbl;

	/* rest of opts */
	int		  input_fildes;
	int		  output_fildes;
	zt_array	  data;
	int		  flags;
};

typedef struct zt_bstream_vtbl zt_bstream_vtbl;
struct zt_bstream_vtbl {
	size_t size;
	/* virtual function pointers */
	void		(* close)(zt_bstream *);
	int		(* truncate)(zt_bstream *);
	int		(* empty)(zt_bstream *);
	int		(* eos)(zt_bstream *);
	zt_bstream *	(* clone)(zt_bstream *);
	size_t		(* read)(zt_bstream *, void *, size_t, char, char);
	size_t		(* write)(zt_bstream *, void *, size_t, char, char);
};

zt_bstream *zt_bstream_new (zt_bstream_vtbl *, int , size_t);
void zt_bstream_free (zt_bstream **);

END_C_DECLS
#endif  /* _ZT_STREAM_PRIVATE_H_ */
