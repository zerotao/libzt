#include "../zt_bytearray.h"
#include "../zt_assert.h"

#define SET_DATA(_this_,_data_,_size_)				\
	do {							\
		void 		* _data = (void *)(_data_);	\
		zt_byteArray	* _this = (_this_);		\
		size_t		  _size = (_size_);		\
								\
		memcpy(_this->data, _data, _size);		\
		_this->offset = _this->data + _size;		\
		_this->size = _size;				\
	} while(0)

#define ALLOC_DATA(_this_, _size_)				\
	do {							\
		zt_byteArray	* _this = (_this_);		\
		size_t		  _size = (_size_);		\
								\
		_this->data = XCALLOC(unsigned char, _size);	\
		_this->size = _size;				\
		_this->offset = _this->data + _size;		\
	} while(0)

#define RESIZE_DATA(_this_, _size_)				\
	do {							\
		zt_byteArray	* _this = (_this_);		\
		size_t		  _size = (_size_);		\
		unsigned char	* _tmp = 0;			\
		size_t		  _offt = 0;			\
								\
		_offt = DATA_LEN(_this);			\
		_tmp = XCALLOC(unsigned char, _size);		\
		memcpy(_tmp, _this->data, DATA_LEN(_this));	\
		XFREE(_this->data);				\
		_this->data = _tmp;				\
		_this->offset = _this->data + _offt;		\
	} while(0)

#define DATA_LEN(this) ((size_t) (this->offset - this->data))

zt_byteArray *
zt_byteArray_new(void)
{
	return zt_byteArray_newWithSize(0);
}

zt_byteArray *
zt_byteArray_newWithSize(size_t size)
{
	zt_byteArray	* new = 0;

	new = XCALLOC(zt_byteArray, 1);
	ALLOC_DATA(new, size);
	return new;
}

zt_byteArray *zt_byteArray_newWithCstring(const char *data)
{
	assert(data);
	
	return zt_byteArray_newWithCstring_size(data, strlen(data));
}

zt_byteArray *zt_byteArray_newWithCstring_size(const char *data, size_t size)
{
	zt_byteArray	* new = 0;

	assert(data);
	
	new = zt_byteArray_newWithSize(size);

	SET_DATA(new, data, size);
	
	return new;
}

	
zt_byteArray *zt_byteArray_newCopy(zt_byteArray *this)
{
	zt_byteArray	* new = 0;

	assert(this);
	
	new = zt_byteArray_newWithSize(this->size);
	SET_DATA(new, (this->data), (this->size));

	return new;
}


void
zt_byteArray_free(zt_byteArray **this) 
{
	assert(this);
	
	XFREE((*this)->data);
	XFREE(*this);

	this = 0;
}

size_t zt_byteArray_length(zt_byteArray *this) 
{
	assert(this);
	return DATA_LEN(this);
}

size_t zt_byteArray_memUse(zt_byteArray *this) 
{
	assert(this);
	return (size_t)(this->size + sizeof(zt_byteArray));
}


void zt_byteArray_append(zt_byteArray *this,
			 zt_byteArray *more)
{
	size_t	  total_len;
	size_t	  more_len;
	
	assert(this);
	assert(more);

	more_len = DATA_LEN(more);
	
	total_len = DATA_LEN(this) + more_len;

	if(this->size < total_len) {
		RESIZE_DATA(this, total_len);
	}

	memcpy(this->offset, more->data, DATA_LEN(more));

	this->offset += DATA_LEN(more);
}

void
zt_byteArray_toCstring(unsigned char *buf, zt_byteArray *this, char rep) 
{
	int	  len = 0;
	
	assert(this);
	assert(buf);

	len = DATA_LEN(this);
	
	memcpy(buf, this->data, len);

	if(rep) {
		int		  i;
		unsigned char	* cp;
		
		for(i=0, cp = buf; i < len; i++, cp++) {
			if(*cp == 0) { /* NULL */
				*cp = rep;
			}
		}
	}
	
	buf[len] = '\0';
}

