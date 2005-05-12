#include <stdlib.h>
#include "zt_bstream_private.h"

zt_bstream*
zt_bstream_new (zt_bstream_vtbl *vptr)
{
  zt_bstream *result;
  result = calloc(1, vptr->size);
  result->vtbl = vptr;
  return(result);
}
