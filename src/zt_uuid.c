#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STRING_H
# include <string.h>
#endif /* HAVE_STRING_H */

#include <stdio.h>

#include <math.h>

#include "zt.h"
#include "zt_internal.h"

zt_uuid_t  NAMESPACE_DNS = { { { 0x6b, 0xa7, 0xb8, 0x10, 0x9d, 0xad, 0x11, 0xd1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8 } } };
zt_uuid_t  NAMESPACE_URL = { { { 0x6b, 0xa7, 0xb8, 0x11, 0x9d, 0xad, 0x11, 0xd1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8 } } };
zt_uuid_t  NAMESPACE_OID = { { { 0x6b, 0xa7, 0xb8, 0x12, 0x9d, 0xad, 0x11, 0xd1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8 } } };
zt_uuid_t  NAMESPACE_X500 = { { { 0x6b, 0xa7, 0xb8, 0x14, 0x9d, 0xad, 0x11, 0xd1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8 } } };

static int rand_initialized           = 0;

#define _UUID_BASE62_SEGMENT UUID_BASE62_STR_LEN / 2
static const char base62_alphabet[63] = \
    "0123456789"
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static int base62_encode_int64(u_int64_t in, char * out, size_t olen);
static int base62_decode_int64(const char * in, size_t ilen, u_int64_t * out);

void       srandomdev(void);
#ifndef HAVE_SRANDOMDEV
#include <fcntl.h>
#include <time.h>
void
srandomdev(void) {
    int      fd;
    uint32_t rand_data;

    fd = open("/dev/urandom", O_RDONLY);

    if (fd < 0 || read(fd, &rand_data,
                       sizeof(uint32_t)) != sizeof(uint32_t)) {
        /* this is a shitty situation.... */
        rand_data = (uint32_t)time(NULL);
    }

    if (fd >= 0) {
        close(fd);
    }

    srandom(rand_data);
}

#endif /* ifndef HAVE_SRANDOMDEV */

int
zt_uuid4(zt_uuid_t * uuid) {
    long v;

    zt_assert(uuid);
    if (rand_initialized == 0) {
        srandomdev();
        rand_initialized = 1;
    }

    v = random(), memcpy(&uuid->data.bytes, &v, 4);
    v = random(), memcpy(&uuid->data.bytes[4], &v, 4);
    v = random(), memcpy(&uuid->data.bytes[8], &v, 4);
    v = random(), memcpy(&uuid->data.bytes[12], &v, 4);

    /* set the version number */
    uuid->data.bytes[UUID_VERSION_OFFT]   = (uuid->data.bytes[UUID_VERSION_OFFT] & 0x0F) | (UUID_VER_PSEUDORANDOM << 4);
    /* clear the clock bits */
    uuid->data.bytes[UUID_CLOCK_SEQ_OFFT] = (uuid->data.bytes[UUID_CLOCK_SEQ_OFFT] & 0x3F) | 0x80;

    return 0;
}

int
zt_uuid5(char * value, size_t vlen, zt_uuid_ns type, zt_uuid_t * uuid) {
    zt_sha1_ctx ctx;
    uint8_t     digest[20];

    zt_sha1_init(&ctx);
    switch (type) {
        case UUID_NS_DNS:
            zt_sha1_update(&ctx, NAMESPACE_DNS.data.bytes, UUID_ALEN);
            break;
        case UUID_NS_URL:
            zt_sha1_update(&ctx, NAMESPACE_URL.data.bytes, UUID_ALEN);
            break;
        case UUID_NS_OID:
            zt_sha1_update(&ctx, NAMESPACE_OID.data.bytes, UUID_ALEN);
            break;
        case UUID_NS_X500:
            zt_sha1_update(&ctx, NAMESPACE_X500.data.bytes, UUID_ALEN);
            break;
        default:
            zt_log_printf(zt_log_err, "unknown namespace %d", type);
            return -1;
    }
    zt_sha1_update(&ctx, (uint8_t*)value, vlen);
    zt_sha1_finalize(&ctx, digest);
    memcpy(uuid->data.bytes, digest, 16);

    /* set the version number */
    uuid->data.bytes[UUID_VERSION_OFFT]   = (uuid->data.bytes[UUID_VERSION_OFFT] & 0x0F) | (UUID_VER_NAMESPACE_SHA1 << 4);
    /* clear the clock bits */
    uuid->data.bytes[UUID_CLOCK_SEQ_OFFT] = (uuid->data.bytes[UUID_CLOCK_SEQ_OFFT] & 0x3F) | 0x80;

    return 0;
}

int
zt_uuid_tostr(zt_uuid_t * uuid, char ** uuids, zt_uuid_flags_t flags) {
    char   uuids_hex[UUID_SHORT_STR_LEN];
    char * result = NULL;
    int    i;

    if (flags == zt_uuid_std_fmt) {
        zt_binary_to_hex(uuid->data.bytes, 16, uuids_hex, 32);

        result = zt_calloc(char, UUID_STR_LEN + 1);
        i      = snprintf(result, UUID_STR_LEN + 1, "%8.8s-%4.4s-%4.4s-%4.4s-%12.12s",
                          uuids_hex,
                          &uuids_hex[8],
                          &uuids_hex[12],
                          &uuids_hex[16],
                          &uuids_hex[20]);
    } else if (flags == zt_uuid_short_fmt) {
        zt_binary_to_hex(uuid->data.bytes, 16, uuids_hex, 32);

        result = zt_calloc(char, UUID_SHORT_STR_LEN + 1);
        i      = snprintf(result, UUID_SHORT_STR_LEN + 1, "%8.8s%4.4s%4.4s%4.4s%12.12s",
                          uuids_hex,
                          &uuids_hex[8],
                          &uuids_hex[12],
                          &uuids_hex[16],
                          &uuids_hex[20]);
    } else if (flags == zt_uuid_base62_fmt) {
        result = zt_calloc(char, 22);

        u_int64_t v1 = 0;
        u_int64_t v2 = 0;

        for (i = 0; i < 8; i++) {
            const int shift = (7 - i) * 8;
            v1 |= ((u_int64_t)uuid->data.bytes[i] << shift);
            v2 |= ((u_int64_t)uuid->data.bytes[i + 8] << shift);
        }

        if (base62_encode_int64(v1, result, _UUID_BASE62_SEGMENT) >= 0) {
            if (base62_encode_int64(v2, result + _UUID_BASE62_SEGMENT, _UUID_BASE62_SEGMENT) >= 0) {
                i = UUID_BASE62_STR_LEN;
            }
        }
    } else {
        zt_log_printf(zt_log_err, "unknown uuid format");
        return -1;
    }
    if (i == -1) {
        free(result);
        result = NULL;
    }
    *uuids = result;
    return i;
} /* zt_uuid_tostr */

int
zt_uuid_fromstr(char * uuidstr, zt_uuid_t * uuid, zt_uuid_flags_t flags) {
    char uuid_hex[UUID_SHORT_STR_LEN + 1]; /* +1 for sscanf */

    zt_assert(uuidstr);
    zt_assert(uuid);


    if (flags == zt_uuid_std_fmt) {
        if (strlen(uuidstr) != UUID_STR_LEN) {
            return -1;
        }
        sscanf(uuidstr, "%8s-%4s-%4s-%4s-%12s",
               uuid_hex,
               &uuid_hex[8],
               &uuid_hex[12],
               &uuid_hex[16],
               &uuid_hex[20]);
        zt_hex_to_binary(uuid_hex, 32, uuid->data.bytes, 16);
    } else if (flags == zt_uuid_short_fmt) {
        if (strlen(uuidstr) != UUID_SHORT_STR_LEN) {
            return -1;
        }
        memcpy(uuid_hex, uuidstr, UUID_SHORT_STR_LEN);
        zt_hex_to_binary(uuid_hex, 32, uuid->data.bytes, 16);
    } else if (flags == zt_uuid_base62_fmt) {
        u_int64_t v1 = 0;
        u_int64_t v2 = 0;
        int       i;

        if (base62_decode_int64(uuidstr, _UUID_BASE62_SEGMENT, &v1) != 0) {
            return -1;
        }
        if (base62_decode_int64(uuidstr + _UUID_BASE62_SEGMENT, UUID_BASE62_STR_LEN, &v2) != 0) {
            return -1;
        }

        for (i = 0; i < 8; i++) {
            const int shift = (7 - i) * 8;
            uuid->data.bytes[i]     = (v1 >> shift) & 0xFF;
            uuid->data.bytes[i + 8] = (v2 >> shift) & 0xFF;
        }
    } else {
        zt_log_printf(zt_log_err, "unknown uuid format");
        return -1;
    }


    return 0;
} /* zt_uuid_fromstr */

int
zt_uuid_cmp(zt_uuid_t * uuid, zt_uuid_t * uuid2) {
    return memcmp(uuid->data.bytes, uuid2->data.bytes, 16);
}

#define VALID_CHARS "abcdefABCDEF0123456789"

int
zt_uuid_isvalid(char * uuid, zt_uuid_flags_t flags) {
    size_t len;
    size_t i;

    if (uuid == NULL) {
        return -1;
    }

    len = strlen(uuid);

    switch (flags) {
        case zt_uuid_std_fmt:
            if (len != UUID_STR_LEN) {
                return -1;
            }
            break;
        case zt_uuid_short_fmt:
            if (len != UUID_SHORT_STR_LEN) {
                return -1;
            }
            break;
        case zt_uuid_base62_fmt:
            if (len != UUID_BASE62_STR_LEN) {
                return -1;
            }
            break;
        default:
            return -1;
    }


    /* 550e8400-e29b-41d4-a716-446655440000
     * 550e8400 - e 2  9  b  -  4  1  d  4  -  a  7  1  6  -  446655440000
     * 01234567 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23              */

    for (i = 0; i < len; i++) {
        if (flags == zt_uuid_std_fmt) {
            switch (i) {
                case 8:
                case 13:
                case 18:
                case 23:
                    if (uuid[i] != '-') {
                        return -1;
                    }
                    break;
                default:
                    if (strchr(VALID_CHARS, uuid[i]) == NULL) {
                        return -1;
                    }
                    break;
            }
        } else if (flags == zt_uuid_base62_fmt) {
            if (strchr(base62_alphabet, uuid[i]) == NULL) {
                return -1;
            }
        } else {
            if (strchr(VALID_CHARS, uuid[i]) == NULL) {
                return -1;
            }
        }
    }

    return 0;
} /* zt_uuid_isvalid */

static int
base62_encode_int64(u_int64_t in, char * out, size_t olen) {
    unsigned int i   = _UUID_BASE62_SEGMENT;
    int          ret = 0;

    if (!out || olen < _UUID_BASE62_SEGMENT) {
        return -1;
    }

    while (in) {
        const unsigned int v = (unsigned int)(in % 62);

        out[--i] = base62_alphabet[v];
        in      /= 62;
    }

    ret = i;
    while (i > 0) {
        out[--i] = base62_alphabet[0];
    }

    if (olen > _UUID_BASE62_SEGMENT) {
        out[_UUID_BASE62_SEGMENT] = 0;
    }

    return ret;
}

static int
base62_decode_int64(const char * in, size_t ilen, u_int64_t * out) {
    u_int64_t    v    = 0;
    const size_t tlen = (ilen > _UUID_BASE62_SEGMENT) ? _UUID_BASE62_SEGMENT : ilen;
    size_t       i;

    if (!in || !out) {
        return -1;
    }

    for (i = 0; i < tlen; i++) {
        const char c = in[i];
        int        n;

        /* shift back from our alphabet */
        if (c >= '0' && c <= '9') {
            n = c - '0';
        } else if (c >= 'a' && c <= 'z') {
            n = c - ('a' - 10); /* 10 = offset of 'a' in our alphabet */
        } else if (c >= 'A' && c <= 'Z') {
            n = c - ('A' - 36); /* 36 = offset of 'A' in our alphabet */
        } else {
            return -1; /* invalid character */
        }

        v += n * powl(62, tlen - i - 1);
    }

    *out = v;
    return 0;
}

