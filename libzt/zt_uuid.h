#ifndef _ZT_UUID_H_
#define _ZT_UUID_H_

#include <stdint.h>

#include <libzt/zt.h>
BEGIN_C_DECLS

#define UUID_STR_LEN 36
#define UUID_SHORT_STR_LEN 32
#define UUID_ALEN 16
#define UUID_VERSION_OFFT 6
#define UUID_CLOCK_SEQ_OFFT 8

typedef enum zt_uuid_flags_t zt_uuid_flags_t;
enum zt_uuid_flags_t {
    zt_uuid_std_fmt = 0,
    zt_uuid_short_fmt,
};

typedef struct zt_uuid zt_uuid_t;
struct zt_uuid {
    union {
        uint8_t bytes[UUID_ALEN];
        struct {
            uint32_t time_low;                             /* 0-3 */
            uint16_t time_mid;                             /* 4-5 */
            uint16_t time_hi_and_version;                  /* 6-7 */
            uint8_t  clock_seq_hi_and_reserved;            /* 8 */
            uint8_t  clock_seq_low;                        /* 9 */
            uint8_t  node[6];                              /* 10-15 */
        } field;
    } data;
};

extern zt_uuid_t NAMESPACE_DNS;
extern zt_uuid_t NAMESPACE_URL;
extern zt_uuid_t NAMESPACE_OID;
extern zt_uuid_t NAMESPACE_X500;

typedef enum zt_uuid_ns zt_uuid_ns;
enum zt_uuid_ns {
    UUID_NS_DNS,
    UUID_NS_URL,
    UUID_NS_OID,
    UUID_NS_X500
};


#define UUID_VER_TIME           1
#define UUID_VER_DEC_POSIX      2
#define UUID_VER_NAMESPACE_MD5  3
#define UUID_VER_PSEUDORANDOM   4
#define UUID_VER_NAMESPACE_SHA1 5

int zt_uuid4(zt_uuid_t *uuid);
int zt_uuid5(char *value, size_t vlen, zt_uuid_ns type, zt_uuid_t *uuid);
int zt_uuid_tostr(zt_uuid_t *uuid, char **uuids, zt_uuid_flags_t flags);
int zt_uuid_fromstr(char *value, zt_uuid_t *uuid, zt_uuid_flags_t flags);
int zt_uuid_cmp(zt_uuid_t *uuid, zt_uuid_t *uuid2);

END_C_DECLS
#endif    /* _ZT_UUID_H_ */
