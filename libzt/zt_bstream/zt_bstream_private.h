/* Private interface to zt_bstream
 * You should NOT access this directly
 * unless you are extending the component
 */
#ifndef _ZT_STREAM_PRIVATE_H_
#define _ZT_STREAM_PRIVATE_H_

#include <stdlib.h>
#include <libzt/zt_bstream.h>

BEGIN_C_DECLS

zt_bstream *zt_bstream_new (zt_bstream_vtbl *);

END_C_DECLS
#endif  /* _ZT_STREAM_PRIVATE_H_ */
