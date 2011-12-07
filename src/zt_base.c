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
zt_base_encode(zt_base_definition_t * def, void * in, size_t in_bytes, void **out, size_t *out_bytes) {
    size_t                ocount;
    size_t                lcount;
    size_t                mod;
    size_t                olen;
    const unsigned char * inp = in;
    unsigned char       * outp = out ? *(unsigned char **)out : NULL;

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

    if (out && outp == NULL && *out_bytes == 0) {
        outp = zt_calloc(unsigned char, ocount);
        *out = (void *)outp;
        *out_bytes = ocount;
    }

    olen = *out_bytes;
    *out_bytes = ocount;

    /* if there is not enough space in the output then fail */
    if (olen < ocount) {
        return -2;
    }

    /* if there is no output then return just the size */
    if (!out || !outp) {
        return 0;
    }

    /* perform encoding */
    {
        _bits_t               bits;
        uint8_t               mask = MAKE_MASK(def->obits);
        size_t                i;
        const char *          alphabet = def->alphabet;
        uint8_t               igroups = def->igroups;
        uint8_t               ogroups = def->ogroups;
        uint8_t               obits = def->obits;

        for (i=0; i < lcount; i++) {

            bits = *inp++;
            switch(igroups) {
                case 8:  bits = bits << 8 | *inp++;
                case 7:  bits = bits << 8 | *inp++;
                case 6:  bits = bits << 8 | *inp++;
                case 5:  bits = bits << 8 | *inp++;
                case 4:  bits = bits << 8 | *inp++;
                case 3:  bits = bits << 8 | *inp++;
                case 2:  bits = bits << 8 | *inp++;
                         break;
                case 1: /* there is no spoon */
                         break;
                default: /* error */
                         break;
            }

            switch(ogroups) {
                case 8: outp[7] = alphabet[bits & mask], bits >>= obits;
                case 7: outp[6] = alphabet[bits & mask], bits >>= obits;
                case 6: outp[5] = alphabet[bits & mask], bits >>= obits;
                case 5: outp[4] = alphabet[bits & mask], bits >>= obits;
                case 4: outp[3] = alphabet[bits & mask], bits >>= obits;
                case 3: outp[2] = alphabet[bits & mask], bits >>= obits;
                case 2: outp[1] = alphabet[bits & mask], bits >>= obits;
                case 1: outp[0] = alphabet[bits & mask];
                        break;
            }
            outp += ogroups;
        }

        if (mod > 0) {
            bits = inp[0];
            for(i=1; i < igroups; i++) {
                bits = bits << 8 | ((mod > i) ? inp[i] : 0);
            }
            for (i=ogroups; i > 0; i--) {
                if (mod >= i-1) {
                    outp[i-1] = alphabet[ bits & mask ];
                } else {
                    if (ZT_BIT_ISSET(def->flags, zt_base_encode_with_padding)) {
                        outp[i-1] = def->pad;
                    }
                }
                bits >>= obits;
            }
        }
    }
    return 0;
}

/*
 * decodes the encoded data from void * in and writes the output to
 * the void ** out pointer. If *out is NULL and *out_bytes is 0, *out is
 * calloc'd and must be free()'d by the user.
*/
int
zt_base_decode(zt_base_definition_t * def, void * in, size_t in_bytes, void ** out, size_t * out_bytes) {
    const unsigned char * inp;
    unsigned char       * outp;
    int                   oval   = 0;
    /* size_t                used   = 0; */
    /* size_t                avail  = 0; */
    /* bool                  heaped = false; */
    size_t                lcount = 0;
    size_t                ocount = 0;
    size_t                mod = 0;

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

    inp  = (unsigned char *)in;

    ssize_t i;
    /* strip any follow on pad chars */
    for (i=in_bytes-1; i > 0; i--) {
        if (inp[i] != def->pad) {
            i = in_bytes - 1 - i;
            break;
        }
    }
    lcount = (in_bytes-i) / def->ogroups; /* number of whole input groups */
    mod = ((in_bytes-i) % def->ogroups); /* partial inputs */
    ocount = (lcount * def->igroups) + mod; /* total output bytes */

    if (out == NULL) {
        *out_bytes = ocount;
        return 0;
    }

    if (*out != NULL && *out_bytes < ocount) {
        *out_bytes = ocount;
        return -2;
    }

    outp = *((unsigned char **)out);

    if ((outp == NULL && *out_bytes == 0)) {
        /* dynamically allocate the buffer */
        if (!(outp = zt_calloc(unsigned char, ocount+1))) {
            return -1;
        }

        *out = outp;
        *out_bytes = ocount;
    }

    {
        /* perform decoding */
        size_t        i;
        _bits_t       bits;
        const char  * dictionary = def->dictionary;
        uint8_t       obits = def->obits;
        uint8_t       ogroups = def->ogroups;
        uint8_t       igroups = def->igroups;
        uint8_t       ibits = (obits * ogroups) / igroups;
        uint8_t       mask = MAKE_MASK(ibits);
        size_t        bytes = 0;
        uint8_t       fault_on_error = !ZT_BIT_ISSET(def->flags, zt_base_ignore_non_encoded_data);

        for (i=0; i < lcount; i++) {

            /* bits = dictionary[*inp++]; */
            /* for(i=1; i < ogroups; i++) { */
                /* bits = bits << obits | dictionary[*inp++]; */
            /* } */

            ssize_t  v;
            v = dictionary[*inp++]; if (v >= 0) { bits = v; } else if(fault_on_error) { return -1; }
            switch(ogroups) {
                case 8: { v = dictionary[*inp++]; if (v >= 0) { bits = bits << obits | v; } else if(fault_on_error) { return -1; } }
                case 7: { v = dictionary[*inp++]; if (v >= 0) { bits = bits << obits | v; } else if(fault_on_error) { return -1; } }
                case 6: { v = dictionary[*inp++]; if (v >= 0) { bits = bits << obits | v; } else if(fault_on_error) { return -1; } }
                case 5: { v = dictionary[*inp++]; if (v >= 0) { bits = bits << obits | v; } else if(fault_on_error) { return -1; } }
                case 4: { v = dictionary[*inp++]; if (v >= 0) { bits = bits << obits | v; } else if(fault_on_error) { return -1; } }
                case 3: { v = dictionary[*inp++]; if (v >= 0) { bits = bits << obits | v; } else if(fault_on_error) { return -1; } }
                case 2: { v = dictionary[*inp++]; if (v >= 0) { bits = bits << obits | v; } else if(fault_on_error) { return -1; } }
                        break;
                case 1: /* there is no spoon */
                        break;
            }

            switch(igroups) {
                case 8: outp[7] = bits & mask, bits >>= ibits;
                case 7: outp[6] = bits & mask, bits >>= ibits;
                case 6: outp[5] = bits & mask, bits >>= ibits;
                case 5: outp[4] = bits & mask, bits >>= ibits;
                case 4: outp[3] = bits & mask, bits >>= ibits;
                case 3: outp[2] = bits & mask, bits >>= ibits;
                case 2: outp[1] = bits & mask, bits >>= ibits;
                case 1: outp[0] = bits & mask;
                        break;
            }
            outp += igroups;
            bytes += igroups;
            /* *out_bytes += igroups; */
        }


        /* now handle partials */
        if (mod > 0) {
            bits = dictionary[*inp++];
            for(i=1; i < ogroups; i++) {
                bits = bits << obits | ((mod > i) ? dictionary[*inp++] : 0);
            }

            for(i=igroups; i > 0; i--) {
                if (mod >= i-1) {
                    if (bits & mask) {
                        outp[i-1] = bits & mask;
                        bytes += 1;
                    }
                } else {
                    outp[i-1] = 0;
                }
                bits >>= ibits;
            }
        }
        *out_bytes = bytes;
    }

    return oval;
} /* zt_base_decode */

