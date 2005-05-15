#include "zt_bstream_private.h"


zt_bstream*
zt_bstream_new (zt_bstream_vtbl *vptr, int flags, size_t len)
{
  zt_bstream *result;
  result = XCALLOCS(vptr->size, 1);
  result->vtbl = vptr;

  result->flags = flags;
  result->data = zt_array_new(len, sizeof(char));
  
  return(result);
}

void
zt_bstream_free(zt_bstream **bs)
{
	assert(bs);
	
	(*bs)->flags = 0;
	zt_array_free(&(*bs)->data);
	XFREE(bs);
	return;
}

	
