
#include "zt_bstream.h"
#include "zt_assert.h"

void
bstream_close(zt_bstream *bs) 
{
	assert(bs);
	if(assert_nf(bs->vtbl->close)) {
		bs->vtbl->close(bs);
	}
}

int
bstream_truncate(zt_bstream *bs)
{
	assert(bs);
	if(assert_nf(bs->vtbl->truncate)) {
		return bs->vtbl->truncate(bs);
	}
	
	return 0;
}

int
bstream_empty(zt_bstream *bs) 
{
	assert(bs);
	if(assert_nf(bs->vtbl->empty)) {
		return bs->vtbl->empty(bs);
	}
	return 0;
}

int
bstream_eos(zt_bstream *bs)
{
	assert(bs);
	return 0;
}


zt_bstream *
bstream_clone(zt_bstream *bs) 
{
	assert(bs);
	if(assert_nf(bs->vtbl->clone)) {
		return bs->vtbl->clone(bs);
	}
	
	return 0;
}

size_t
bstream_read(zt_bstream *bs, void *buf, size_t len, char size, char tagged)
{
	assert(bs);
	if(assert_nf(bs->vtbl->read)) {
		return bs->vtbl->read(bs, buf, len, size, tagged);
	}
	
	return 0;
}


size_t
bstream_write(zt_bstream *bs, void *buf, size_t len, char size, char tagged)
{
	assert(bs);
	if(assert_nf(bs->vtbl->write)) {
		return bs->vtbl->write(bs, buf, len, size, tagged);
	}
	
	return 0;
}
