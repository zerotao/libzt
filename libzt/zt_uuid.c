
#include "zt_log.h"
#include "zt_sha1.h"
#include "zt_uuid.h"
#include "zt_cstr.h"

#define UUID_VERSION_OFFT 6
#define UUID_CLOCK_SEQ_OFFT 8

zt_uuid_t NAMESPACE_DNS  = {{ 0x6b, 0xa7, 0xb8, 0x10, 0x9d, 0xad, 0x11, 0xd1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8 }};
zt_uuid_t NAMESPACE_URL  = {{ 0x6b, 0xa7, 0xb8, 0x11, 0x9d, 0xad, 0x11, 0xd1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8 }};
zt_uuid_t NAMESPACE_OID  = {{ 0x6b, 0xa7, 0xb8, 0x12, 0x9d, 0xad, 0x11, 0xd1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8 }};
zt_uuid_t NAMESPACE_X500 = {{ 0x6b, 0xa7, 0xb8, 0x14, 0x9d, 0xad, 0x11, 0xd1, 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8 }};

int zt_uuid5(char *value, size_t vlen, zt_uuid_ns type, zt_uuid_t *uuid)
{
	zt_sha1_ctx		  ctx;
	uint8_t			  digest[20];
	
	zt_sha1_init(&ctx);
	switch(type){
		case UUID_NS_DNS:
			zt_sha1_update(&ctx, NAMESPACE_DNS.bytes, UUID_ALEN);
			break;
		case UUID_NS_URL:
			zt_sha1_update(&ctx, NAMESPACE_URL.bytes, UUID_ALEN);
			break;
		case UUID_NS_OID:
			zt_sha1_update(&ctx, NAMESPACE_OID.bytes, UUID_ALEN);
			break;
		case UUID_NS_X500:
			zt_sha1_update(&ctx, NAMESPACE_X500.bytes, UUID_ALEN);
			break;
		default:
			log_printf(log_err, "unknown namespace %d", type);
			return -1;
	}
	zt_sha1_update(&ctx, (uint8_t *)value, vlen);
	zt_sha1_finalize(&ctx, digest);
	memcpy(uuid->bytes, digest, 16);

	/* set the version number */
	uuid->bytes[UUID_VERSION_OFFT] = (uuid->bytes[UUID_VERSION_OFFT] & 0x0F) | (UUID_VER_NAMESPACE_SHA1 << 4);
	/* clear the clock bits */
	uuid->bytes[UUID_CLOCK_SEQ_OFFT] = (uuid->bytes[UUID_CLOCK_SEQ_OFFT] & 0x3F) | 0x80;
	
	return 0;
}



int zt_uuid_tostr(zt_uuid_t *uuid, char **uuids) 
{
	char	  uuids_hex[32 + 1];
	char	* result = NULL;
	int		  i;
	
	zt_binary_to_hex(uuid->bytes, 16, uuids_hex, 32);

	result = XCALLOC(char, UUID_STR_LEN + 1);
	
	i = snprintf(result, UUID_STR_LEN + 1, "%8.8s-%4.4s-%4.4s-%4.4s-%12.12s",
				 uuids_hex,
				 &uuids_hex[8],
				 &uuids_hex[12],
				 &uuids_hex[16],
				 &uuids_hex[20]);
	if(i == -1) {
		free(result);
		result = NULL;
	}
	*uuids = result;
	return i;
}

int zt_uuid_cmp(zt_uuid_t *uuid, zt_uuid_t *uuid2) 
{
	return memcmp(uuid->bytes, uuid2->bytes, 16);
}
