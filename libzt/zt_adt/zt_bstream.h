#ifndef _ZT_BSTREAM_H_
#define _ZT_BSTREAM_H_

#include <libzt/zt.h>

BEGIN_C_DECLS


typedef enum {
	zt_bstream_uint		0,
	zt_bstream_int		1,
	zt_bstream_float	2,
	zt_bstream_address	3
} zt_bstream_tagged_type;

typedef struct {
	u_int32_t	array	: 1;
	u_int32_t	type	: 2;
	u_int32_t	count	: 5;
} zt_bstream_tag;

zt_bstream_tag zt_bstream_char_to_tag(u_int8_t c);
u_int8_t zt_bstream_tag_tag_to_char(zt_bstream_tag t);
zt_bstream_tag zt_bstream_tag_make(u_int8_t array, u_int8_t type, u_int8_t bytes);
u_int32_t zt_bstream_tag_cmp(zt_bstream_tag *stream1, zt_bstream_tag *stream2);
void zt_bstream_tag_display(zt_bstream_tag *stream);
char *zt_bstream_tag_type_name(zt_bstream_tag *stream);

typedef struct {
	
} zt_bstream;



END_C_DECLS
#endif /* _ZT_BSTREAM_H_ */
