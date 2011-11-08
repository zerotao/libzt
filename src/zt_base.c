#include "zt_base.h"
typedef uint64_t _bits_t;

static zt_base_definition_t _base64_rfc = {
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/",
    /* valid: alphanum, '+', '/'; allows CR,LF,'=',SP,TAB
     * -1 (invalid) -2 (ignored)
     */
    { -1, -1, -1, -1, -1, -1, -1, -1, -1, -2, -2, -1, -1, -2, -1, -1,     /*  0-15 */
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,     /* 16-31 */
      -2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,     /* 32-47 */
      52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -2, -1, -1,     /* 48-63 */
      -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,     /* 64-79 */
      15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,     /* 80-95 */
      -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,     /* 96-111 */
      41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,     /* 112-127 */
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    },
    64,
    3,
    4,
    6,
    '=',
    zt_base_encode_with_padding
};

static zt_base_definition_t _base64_uri = {
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789-_",
    /* valid: alphanum, '-', '_'; ignores: CR,LF,'=',SP,TAB */
    { -1, -1, -1, -1, -1, -1, -1, -1, -1, -2, -2, -1, -1, -2, -1, -1,     /* 0-15 */
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,     /* 16-31 */
      -2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1,     /* 32-47 */
      52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -2, -1, -1,     /* 48-63 */
      -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,     /* 64-79 */
      15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, 63,     /* 80-95 */
      -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,     /* 96-111 */
      41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,     /* 112-127 */
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    },
    64,
    3,
    4,
    6,
    '=',
    zt_base_encode_with_padding
};


zt_base_definition_t * zt_base64_rfc = &_base64_rfc;
zt_base_definition_t * zt_base64_uri = &_base64_uri;

static zt_base_definition_t _base32_rfc = {
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "234567",
    { -1, -1, -1, -1, -1, -1, -1, -1, -1, -2, -2, -1, -1, -2, -1, -1,     /* 0-15 */
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,     /* 16-31 */
      -2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,     /* 32-47 */
      -1, -1, 26, 27, 28, 29, 30, 31, -1, -1, -1, -1, -1, -2, -1, -1,     /* 48-63 */
      -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,     /* 64-79 */
      15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,     /* 80-95 */
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,     /* 96-111 */
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,     /* 112-127 */
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    },
    32,
    5,
    8,
    5,
    '=',
    zt_base_encode_with_padding
};

static zt_base_definition_t _base32_hex = {
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUV",
    { -1, -1, -1, -1, -1, -1, -1, -1, -1, -2, -2, -1, -1, -2, -1, -1,     /* 0-15 */
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,     /* 16-31 */
      -2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,     /* 32-47 */
       0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -2, -1, -1,     /* 48-63 */
      -1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,     /* 64-79 */
      25, 26, 27, 28, 29, 30, 31, -1, -1, -1, -1, -1, -1, -1, -1, -1,     /* 80-95 */
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,     /* 96-111 */
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,     /* 112-127 */
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    },
    32,
    5,
    8,
    5,
    '=',
    zt_base_encode_with_padding
};

zt_base_definition_t * zt_base32_rfc = &_base32_rfc;
zt_base_definition_t * zt_base32_hex = &_base32_hex;

static zt_base_definition_t _base16_rfc = {
    "0123456789ABCDEF",
    { -1, -1, -1, -1, -1, -1, -1, -1, -1, -2, -2, -1, -1, -2, -1, -1,     /* 0-15 */
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,     /* 16-31 */
      -2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,     /* 32-47 */
       0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -2, -1, -1,     /* 48-63 */
      -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,     /* 64-79 */
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,     /* 80-95 */
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,     /* 96-111 */
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,     /* 112-127 */
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    },
    16,
    1,
    2,
    4,
    '=',
    0
};
zt_base_definition_t * zt_base16_rfc = &_base16_rfc;


static int
_valid_dictionary_p(zt_base_definition_t * def) {
    if (!def) {
        return 0;
    }

    if ( 8 * def->igroups != def->ogroups * def->obits) {
        return 0;
    }

    /* can we overflow the bit container */
    if ( def->igroups > sizeof(_bits_t)) {
        return 0;
    }

    return 1;
}

#define MAKE_MASK(x) ((1<<x)-1)
int
zt_base_encode(zt_base_definition_t * def, void * in, size_t in_bytes, void *out, size_t *out_bytes) {
    size_t                ocount;
    size_t                lcount;
    size_t                mod;
    size_t                olen;
    const unsigned char * inp = in;
    unsigned char       * outp = out;

    if (! _valid_dictionary_p(def)) {
        return -1;
    }

    /* if we are missing out_bytes then we cannot continue */
    if (!out_bytes) {
        return -1;
    }

    /* if there is no input then we cannot continue */
    if(!in || !in_bytes) {
        *out_bytes = 0;
        return 0;
    }

    /* calculate the number of (N * 8-bit) input groups and remainder */
    lcount = in_bytes / def->igroups;                        /* number of whole input groups */
    mod = ((8 * (in_bytes % def->igroups)) / def->obits);    /* portion of partial output groups */
    ocount = (lcount * def->ogroups);                        /* number of whole output groups */

    if (mod > 0) {
        if ZT_BIT_ISSET(def->flags, zt_base_encode_with_padding) {
            ocount += def->ogroups;
        } else {
            ocount += mod + 1;
        }
    }

    //printf("%s\t(lcount:%ld mod:%ld ocount:%ld, pp:%ld)\n", inp, lcount, mod, ocount, mod ? (8 * mod) / def->obits : 0);

    olen = *out_bytes;
    *out_bytes = ocount;

    /* if there is not enough space in the output then fail */
    if (olen < ocount) {
        return -2;
    }

    /* if there is no output then return just the size */
    if (!out) {
        return 0;
    }

    /* perform encoding */
    {
        _bits_t               bits;
        uint8_t               mask = MAKE_MASK(def->obits);
        size_t                i;

        for (i=0; i < lcount; i++) {
            size_t                x;

            bits = inp[0];
            for(x=1; x < def->igroups; x++) {
                bits = bits << 8 | inp[x];
            }

            for (x=def->ogroups; x > 0; x--) {
                outp[x-1] = def->alphabet[ bits & mask ], bits >>= def->obits;
            }

            inp += def->igroups;
            outp += def->ogroups;
        }

        if (mod > 0) {
            bits = inp[0];
            for(i=1; i < def->igroups; i++) {
                bits = bits << 8 | ((mod > i) ? inp[i] : 0);
            }
            for (i=def->ogroups; i > 0; i--) {
                if (mod >= i-1) {
                    outp[i-1] = def->alphabet[ bits & mask ];
                } else {
                    if (ZT_BIT_ISSET(def->flags, zt_base_encode_with_padding)) {
                        outp[i-1] = def->pad;
                    }
                }
                bits >>= def->obits;
            }
        }
    }
    return 0;
}

/*
 * decodes the encoded data from void * in and writes the output to
 * the void ** out pointer. If *out is NULL and *out_bytes is 0, *out is
 * malloc'd and must be free()'d by the user.
*/
int
zt_base_decode(zt_base_definition_t * def, void * in, size_t in_bytes, void ** out, size_t * out_bytes) {
    const unsigned char * inp;
    unsigned char       * outp;
    unsigned char       * outp_top;
    int                   oval   = 0;
    size_t                used   = 0;
    size_t                avail  = 0;
    bool                  heaped = false;

    if (!_valid_dictionary_p(def)) {
        return -1;
    }

    if (!out_bytes) {
        return -1;
    }

    if (!in || !in_bytes) {
        *out_bytes = 0;
        return 0;
    }

    if (out == NULL) {
        *out_bytes = in_bytes;

        return 0;
    }

    if (*out != NULL && *out_bytes < in_bytes) {
        *out_bytes = in_bytes;

        return -2;
    }

    outp = *((unsigned char **)out);
    inp  = (unsigned char *)in;

    if ((outp == NULL && *out_bytes == 0)) {
        /* dynamically allocate the buffer */
        avail = in_bytes;

        if (!(outp = zt_malloc(unsigned char, avail))) {
            return -1;
        }

        heaped = true;
    } else {
        avail = *out_bytes;
    }

    outp_top = outp;

    /* perform decodeing */
    {
        size_t  x;
        size_t  i;
        size_t  byte_count = 0;
        uint8_t ibits      = (def->obits * def->ogroups) / def->igroups;
        uint8_t mask       = MAKE_MASK(ibits);
        _bits_t bits       = 0;

        *out_bytes = 0;

        for (i = 0; i < in_bytes; i++) {
            const ssize_t v = def->dictionary[*inp++];

            if (v >= 0) {
                bits = bits << def->obits | v;
                byte_count++;
                /* if we have enough bytes to output */
                if (byte_count == def->ogroups) {
                    for (x = def->igroups; x > 0; x--) {
                        outp[x - 1] = bits & mask, bits >>= ibits;
                        *out_bytes  = *out_bytes + 1;

                        if (++used >= avail) {
                            if (heaped == false) {
                                return -2;
                            }

                            avail += 64;
                            outp   = realloc(outp, avail);
                        }
                    }
                    outp      += def->igroups;
                    byte_count = 0;
                }
            } else if (v == -1) {
                /* invalid byte */
                if (!ZT_BIT_ISSET(def->flags, zt_base_ignore_non_encoded_data)) {
                    return -1;
                }
            } else {
                /* ignored byte */
            }
        }

        if (!oval && byte_count > 0) {
            for (x = byte_count; x < def->ogroups; x++) {
                bits = bits << def->obits;
                byte_count++;
            }

            for (x = def->igroups; x > 0; x--) {
                if (bits & mask) {
                    outp[x - 1] = bits & mask;
                    *out_bytes  = *out_bytes + 1;

                    if (++used >= avail) {
                        if (heaped == false) {
                            return -2;
                        }

                        avail += 64;
                        outp   = realloc(outp, avail);
                    }
                }
                bits >>= ibits;
            }

            outp += def->igroups;
        }
    }

    *out = (void *)outp_top;

    return oval;
} /* zt_base_decode */

