#include "zt_assert.h"
#include "zt_log.h"
#include "zt_sha1.h"
#include "zt_uuid.h"
#include "zt_cstr.h"


zt_uuid_t  NAMESPACE_DNS = { { { 0x6b, 0xa7, 0xb8, 0x10, 0x9d, 0xad, 0x11, 0xd1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8 } } };
zt_uuid_t  NAMESPACE_URL = { { { 0x6b, 0xa7, 0xb8, 0x11, 0x9d, 0xad, 0x11, 0xd1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8 } } };
zt_uuid_t  NAMESPACE_OID = { { { 0x6b, 0xa7, 0xb8, 0x12, 0x9d, 0xad, 0x11, 0xd1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8 } } };
zt_uuid_t  NAMESPACE_X500 = { { { 0x6b, 0xa7, 0xb8, 0x14, 0x9d, 0xad, 0x11, 0xd1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8 } } };

static int rand_initialized = 0;

#ifndef HAVE_SRANDOMDEV
#include <fcntl.h>
#include <time.h>
void
srandomdev(void)
{
    int      fd;
    uint32_t rand_data;

    fd = open("/dev/urandom", O_RDONLY);

    if (fd < 0 || read(fd, &rand_data,
                       sizeof(uint32_t)) != sizeof(uint32_t)) {
        /* this is a shitty situation.... */
        rand_data = time(NULL);
    }

    srandom(rand_data);
}
#endif /* ifndef HAVE_SRANDOMDEV */

int
zt_uuid4(zt_uuid_t * uuid)
{
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
    uuid->data.bytes[UUID_VERSION_OFFT] = (uuid->data.bytes[UUID_VERSION_OFFT] & 0x0F) | (UUID_VER_PSEUDORANDOM << 4);
    /* clear the clock bits */
    uuid->data.bytes[UUID_CLOCK_SEQ_OFFT] = (uuid->data.bytes[UUID_CLOCK_SEQ_OFFT] & 0x3F) | 0x80;

    return (0);
}

int
zt_uuid5(char *value, size_t vlen, zt_uuid_ns type, zt_uuid_t * uuid)
{
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
            return (-1);
    }
    zt_sha1_update(&ctx, (uint8_t *)value, vlen);
    zt_sha1_finalize(&ctx, digest);
    memcpy(uuid->data.bytes, digest, 16);

    /* set the version number */
    uuid->data.bytes[UUID_VERSION_OFFT] = (uuid->data.bytes[UUID_VERSION_OFFT] & 0x0F) | (UUID_VER_NAMESPACE_SHA1 << 4);
    /* clear the clock bits */
    uuid->data.bytes[UUID_CLOCK_SEQ_OFFT] = (uuid->data.bytes[UUID_CLOCK_SEQ_OFFT] & 0x3F) | 0x80;

    return (0);
}



int zt_uuid_tostr(zt_uuid_t *uuid, char **uuids, zt_uuid_flags_t flags)
{
    char   uuids_hex[UUID_SHORT_STR_LEN];
    char * result = NULL;
    int    i;

    zt_binary_to_hex(uuid->data.bytes, 16, uuids_hex, 32);

    if (flags == zt_uuid_std_fmt) {
        result = XCALLOC(char, UUID_STR_LEN + 1);
        i = snprintf(result, UUID_STR_LEN + 1, "%8.8s-%4.4s-%4.4s-%4.4s-%12.12s",
                     uuids_hex,
                     &uuids_hex[8],
                     &uuids_hex[12],
                     &uuids_hex[16],
                     &uuids_hex[20]);
    } else if (flags == zt_uuid_short_fmt) {
        result = XCALLOC(char, UUID_SHORT_STR_LEN + 1);
        i = snprintf(result, UUID_STR_LEN + 1, "%8.8s%4.4s%4.4s%4.4s%12.12s",
                     uuids_hex,
                     &uuids_hex[8],
                     &uuids_hex[12],
                     &uuids_hex[16],
                     &uuids_hex[20]);
    } else {
        zt_log_printf(zt_log_err, "unknown uuid format");
        return (-1);
    }
    if (i == -1) {
        free(result);
        result = NULL;
    }
    *uuids = result;
    return (i);
}

int zt_uuid_fromstr(char *uuidstr, zt_uuid_t * uuid, zt_uuid_flags_t flags)
{
    char uuid_hex[UUID_SHORT_STR_LEN];

    zt_assert(uuidstr);
    zt_assert(uuid);


    if (flags == zt_uuid_std_fmt) {
        if (strlen(uuidstr) != UUID_STR_LEN) {
            return (-1);
        }
        sscanf(uuidstr, "%8s-%4s-%4s-%4s-%12s",
               uuid_hex,
               &uuid_hex[8],
               &uuid_hex[12],
               &uuid_hex[16],
               &uuid_hex[20]);
    } else if (flags == zt_uuid_short_fmt) {
        if (strlen(uuidstr) != UUID_SHORT_STR_LEN) {
            return (-1);
        }
        /* sscanf(uuidstr, "%8s%4s%4s%4s%12s",  */
        /* uuid_hex, */
        /* &uuid_hex[8], */
        /* &uuid_hex[12], */
        /* &uuid_hex[16], */
        /* &uuid_hex[20]); */
        memcpy(uuid_hex, uuidstr, UUID_SHORT_STR_LEN);
    } else {
        zt_log_printf(zt_log_err, "unknown uuid format");
        return (-1);
    }

    zt_hex_to_binary(uuid_hex, 32, uuid->data.bytes, 16);

    return (0);
}

int zt_uuid_cmp(zt_uuid_t *uuid, zt_uuid_t *uuid2)
{
    return (memcmp(uuid->data.bytes, uuid2->data.bytes, 16));
}
