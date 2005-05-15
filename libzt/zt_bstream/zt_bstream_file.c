#include <fcntl.h>
#include "zt_bstream_private.h"


/* local definitions */
static void 
file_close(zt_bstream *bs)
{
	return;
}

static int
file_truncate(zt_bstream *bs)
{
	return 0;	
}


static int
file_empty(zt_bstream *bs) 
{
	return 0;
}

static int
file_eos(zt_bstream *bs) 
{
	return 0;
}

static zt_bstream *
file_clone(zt_bstream *bs)
{
	return 0;
}

static size_t
file_read(zt_bstream *bs, void *buf, size_t num, char size, char flag)
{
	return 0;
	
}

static size_t
file_write(zt_bstream *bs, void *buf, size_t num, char size, char flag)
{
	return 0;
}



/* component data */
typedef struct zt_bstream_file_ty zt_bstream_file_ty;
struct zt_bstream_file_ty {
	zt_bstream        inherited;
	
	/* private data */
	int		  file;
};

static zt_bstream_vtbl vtbl = {
	sizeof(zt_bstream_file_ty),
	
	file_close,
	file_truncate,
	file_empty,
	file_eos,
	file_clone,
	file_read,
	file_write
};

zt_bstream *
zt_bstream_file(char *path, int flags, mode_t mode)
{
	zt_bstream		* result;
	zt_bstream_file_ty	* file;

	result = zt_bstream_new(&vtbl, flags, 2048);
	file = (zt_bstream_file_ty *)result;
	/* do local work here */
	
	
	file->file = open(path, O_RDWR, mode);
	if(!file->file) {
		zt_bstream_free(&result);
		return 0;
	}
	
	return result;
}


