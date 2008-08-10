#ifndef _ZT_UUID_H_
#define _ZT_UUID_H_

#include <stdint.h>

#include <libzt/zt.h>
BEGIN_C_DECLS

#define UUID_STR_LEN 36
#define UUID_ALEN 16

typedef struct zt_uuid zt_uuid_t;
struct zt_uuid {
	uint8_t		bytes[UUID_ALEN];
};

extern zt_uuid_t NAMESPACE_DNS;
extern zt_uuid_t NAMESPACE_URL;
extern zt_uuid_t NAMESPACE_OID;
extern zt_uuid_t NAMESPACE_X500;

typedef enum {
	UUID_NS_DNS,
	UUID_NS_URL,
	UUID_NS_OID,
	UUID_NS_X500,
} zt_uuid_ns;


#define UUID_VER_TIME           1
#define UUID_VER_DEC_POSIX      2
#define UUID_VER_NAMESPACE_MD5  3
#define UUID_VER_PSEUDORANDOM   4
#define UUID_VER_NAMESPACE_SHA1 5

int zt_uuid5(char *value, size_t vlen, zt_uuid_ns type, zt_uuid_t *uuid);
int zt_uuid_tostr(zt_uuid_t *uuid, char **uuids);
int zt_uuid_cmp(zt_uuid_t *uuid, zt_uuid_t *uuid2);

END_C_DECLS
#endif	/* _ZT_UUID_H_ */
