#ifndef _ZT_STREAM_FILE_H_
#define _ZT_STREAM_FILE_H_

#include <libzt/zt_bstream.h>

BEGIN_C_DECLS

extern zt_bstream *zt_bstream_file(char *path, int flags, mode_t mode);

END_C_DECLS
#endif  /* _ZT_STREAM_FILE_H_ */
