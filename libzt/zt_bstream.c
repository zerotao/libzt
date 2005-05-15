
#include "zt_bstream.h"
#include "zt_bstream/zt_bstream_private.h"
#include "zt_assert.h"

int bstrea_is_input(zt_bstream *bs)
{
	assert(bs);
	return bs->flags & stream_input;
}

int bstrea_is_output(zt_bstream *bs)
{
	assert(bs);
	return bs->flags & stream_output;
}

int bstrea_is_tagged(zt_bstream *bs)
{
	assert(bs);
	return bs->flags & stream_tagged;
}

int bstream_is_eos(zt_bstream *bs)
{
	assert(bs);
	if(assert_nf(bs->vtbl->eos)) {
		bs->vtbl->eos(bs);
	}
}	

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
bstream_is_empty(zt_bstream *bs) 
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
