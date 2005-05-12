
#include "zt_bstream_private.h"

/* local declarations */
static void destructor(zt_bstream *zt_bstream);

/* component data */
/* Uncomment to create a private data section
 * also check zt_bstream_vtbl
 *
 * typedef struct zt_bstream_file zt_bstream_file;
 * struct zt_bstream_file {
 *   zt_bstream      inherited;
 */
     /* private data */
 /* };
  */
static zt_bstream_vtbl vtbl = {
/* for private data uncomment the following line and
 * remove the zt_bstream
 * sizeof(zt_bstream_file), 
 */
  sizeof(zt_bstream),
  destructor,
};

zt_bstream *
zt_bstream_file(void)
{
    zt_bstream	* result;
    /* zt_bstream_file * file; */

    result = zt_bstream_new(&vtbl);
    /* file = (zt_bstream_file *)result; */
    /* do local work here */

    return result;
}

/* local definitions */
static void 
destructor(zt_bstream *zt_bstream)
{
    return;
}

