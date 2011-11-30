#ifndef _ZT_BASE_H_
#define _ZT_BASE_H_

#include <zt.h>

typedef enum {
    zt_base_encode_with_padding = 1,
    zt_base_ignore_non_encoded_data = 2,
} zt_base_flags;

BEGIN_C_DECLS

typedef struct zt_base_definition {
    const char  * alphabet;
    const char    dictionary[256];
    size_t        base;    /* base of the encoding (ie strlen of encoder) */

    uint8_t       igroups; /* number of 8 bits input groups to consume */

    uint8_t       ogroups; /* number of obits input groups to produce */
    uint8_t       obits;   /* number of bits in an output group */

    char          pad;
    zt_base_flags flags;
} zt_base_definition_t;

extern zt_base_definition_t * zt_base64_rfc; /* rfc 3548/4648 definition */
extern zt_base_definition_t * zt_base64_uri; /* rfc 3548/4648 URI safe definition */

extern zt_base_definition_t * zt_base32_rfc; /* rfc 3548/4648 definition */
extern zt_base_definition_t * zt_base32_hex; /* rfc 4648 HEX definition */

extern zt_base_definition_t * zt_base16_rfc; /* rfc 3548/4648 definition */
int
zt_base_encode(zt_base_definition_t *def, void *in, size_t in_bytes, void **out, size_t *out_bytes);

#define zt_base64_encode(in, in_bytes, out, out_bytes) \
    zt_base_encode(zt_base64_rfc, in, in_bytes, out, out_bytes)

#define zt_base64_encode_uri(in, in_bytes, out, out_bytes) \
    zt_base_encode(zt_base64_encode_uri, in, in_bytes, out, out_bytes)

#define zt_base32_encode(in, in_bytes, out, out_bytes) \
    zt_base_encode(zt_base32_encode_rfc, in, in_bytes, out, out_bytes)

#define zt_base32_encode_hex(in, in_bytes, out, out_bytes) \
    zt_base_encode(zt_base32_encode_hex, in, in_bytes, out, out_bytes)

#define zt_base16_encode(in, in_bytes, out, out_bytes) \
    zt_base_encode(zt_base16_encode_rfc, in, in_bytes, out, out_bytes)

int zt_base_decode(zt_base_definition_t * def, void * in, size_t in_bytes, void **out, size_t *out_bytes);

#define zt_base64_decode(in, in_bytes, out, out_bytes) \
    zt_base_encode(zt_base64_rfc, in, in_bytes, out, out_bytes)

#define zt_base64_decode_uri(in, in_bytes, out, out_bytes) \
    zt_base_encode(zt_base64_encode_uri, in, in_bytes, out, out_bytes)

#define zt_base32_decode(in, in_bytes, out, out_bytes) \
    zt_base_encode(zt_base32_encode_rfc, in, in_bytes, out, out_bytes)

#define zt_base32_decode_hex(in, in_bytes, out, out_bytes) \
    zt_base_encode(zt_base32_encode_hex, in, in_bytes, out, out_bytes)

#define zt_base16_decode(in, in_bytes, out, out_bytes) \
    zt_base_encode(zt_base16_encode_rfc, in, in_bytes, out, out_bytes)


END_C_DECLS
#endif /* _ZT_BASE_H_ */
