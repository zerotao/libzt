/*
 * sha1_test.c        test assertions
 *
 * Copyright (C) 2008, Jason L. Shiffer <jshiffer@zerotao.org>.  All Rights Reserved.
 * See file COPYING for details.
 *
 */

/*
 * Description: 
 */
#include <libzt/zt.h>
#include <libzt/zt_unit.h>
#include <libzt/zt_uuid.h>


static void
basic_tests(struct zt_unit_test *test, void *data)
{
	/* get rid of the log message for the moment */
	char		* tdata[] = { "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
							  "abc",
							  "http://www.google.com" };
	
	char		* rdata[] = { "028eea4f-70e7-5c44-a3d2-fc85d9847fb8",
							  "7697a46f-b283-5da3-8e7c-62c11c03dd9e",
							  "df65da1c-a04a-5bab-841a-e87753378510"};

	zt_uuid_t	  uuid;
	char		* uuid_s;
	int			  i;

	for (i=0; i < sizeof_array(tdata); i++) {
		zt_uuid5(tdata[i], strlen(tdata[i]), UUID_NS_OID, &uuid);
		zt_uuid_tostr(&uuid, &uuid_s);
		ZT_UNIT_ASSERT(test, memcmp(uuid_s, rdata[i], 36) == 0);
	}	
}

int
register_uuid_suite(struct zt_unit *unit)
{
	struct zt_unit_suite	* suite;

	suite = zt_unit_register_suite(unit, "uuid tests", NULL, NULL, NULL);
	zt_unit_register_test(suite, "basic", basic_tests);
	return 0;
}
