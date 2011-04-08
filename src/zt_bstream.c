#include "zt.h"

static size_t
_array_put(zt_array_t array, char *s, size_t offt, size_t len, int flip)
{
    if (flip) {
        s += len - 1;
        while (len--) {
            zt_array_put(array, offt++, s--);
        }
    } else {
        while (len--) {
            zt_array_put(array, offt++, s++);
        }
    }

    return offt;
}

static size_t
_array_get(zt_array_t array, char *s, size_t offt, size_t len, int flip)
{
    char *c;

    if (flip) {
        s += len - 1;
        while (len--) {
            c = zt_array_get(array, offt++);
            *s-- = *c;
        }
    } else {
        while (len--) {
            c = zt_array_get(array, offt++);
            *s++ = *c;
        }
    }

    return offt;
}

int
zt_bstream_is_empty(zt_bstream_t bs)
{
    zt_assert(bs);
    return zt_array_length(bs->data) == 0;
}


size_t
zt_bstream_truncate(zt_bstream_t bs)
{
    zt_assert(bs);
    return zt_array_set_length(bs->data, 0);
}

void
zt_bstream_set_data(zt_bstream_t bs, char *data, int len, char copy)
{
    zt_assert(bs);

    zt_array_set_data(bs->data, data, len, sizeof(char), copy);
}

void
zt_bstream_set_array(zt_bstream_t bs, zt_array_t array, size_t offt)
{
    zt_assert(bs);

    zt_array_free(&bs->data);

    bs->data = array;
    bs->offt = offt;
}

size_t
zt_bstream_rewind(zt_bstream_t bs)
{
    size_t oofft;

    zt_assert(bs);

    oofft = bs->offt;
    bs->offt = 0;

    return oofft;
}

zt_bstream_t
zt_bstream_new(void)
{
    zt_bstream_t bs;

    bs = zt_calloc(struct zt_bstream, 1);
    bs->data = zt_array_new(0, sizeof(char));

    return bs;
}

zt_bstream_t
zt_bstream_clone(zt_bstream_t bs)
{
    zt_bstream_t clone;

    zt_assert(bs);
    clone = zt_calloc(struct zt_bstream, 1);
    clone->data = zt_array_copy(bs->data, zt_array_length(bs->data));
    clone->offt = bs->offt;

    return clone;
}

void
zt_bstream_free(zt_bstream_t *bs)
{
    zt_assert(bs);

    zt_array_free(&(*bs)->data);
    zt_free(*bs);
    return;
}

size_t
zt_bstream_read(zt_bstream_t bs, char *buf, size_t len, char size, char tag UNUSED)
{
    zt_assert(bs);
    zt_assert(buf);

    bs->offt = _array_get(bs->data, buf, bs->offt, (len * size), bs->flipendian);

    return 0;
}

void
zt_bstream_read_byte(zt_bstream_t bs, char * data)
{
    zt_bstream_read(bs, data, 1, sizeof(char), 0);
}

void
zt_bstream_read_uint8(zt_bstream_t bs, uint8_t * data)
{
    zt_bstream_read(bs, (char *)data, 1, sizeof(uint8_t), 0);
}

void
zt_bstream_read_uint16(zt_bstream_t bs, uint16_t * data)
{
    zt_bstream_read(bs, (char *)data, 1, sizeof(uint16_t), 0);
}

void
zt_bstream_read_uint32(zt_bstream_t bs, uint32_t * data)
{
    zt_bstream_read(bs, (char *)data, 1, sizeof(uint32_t), 0);
}

void
zt_bstream_read_uint64(zt_bstream_t bs, uint64_t * data)
{
    zt_bstream_read(bs, (char *)data, 1, sizeof(uint64_t), 0);
}

void
zt_bstream_read_float(zt_bstream_t bs, float * data)
{
    zt_bstream_read(bs, (char *)data, 1, sizeof(float), 0);
}

void
zt_bstream_read_double(zt_bstream_t bs, double * data)
{
    zt_bstream_read(bs, (char *)data, 1, sizeof(double), 0);
}

size_t
zt_bstream_write(zt_bstream_t bs, char *data, size_t len, char size, char tag UNUSED)
{
    size_t tlen = len * size;
    size_t alen = 0;

    zt_assert(bs);
    zt_assert(data);

    alen = zt_array_length(bs->data);

    if (tlen > alen - bs->offt) {
        zt_array_resize(bs->data, (alen + tlen) * 2);
    }

    bs->offt = _array_put(bs->data, data, bs->offt, tlen, bs->flipendian);

    return 0;
}

void
zt_bstream_write_byte(zt_bstream_t bs, char data)
{
    zt_bstream_write(bs, &data, 1, sizeof(char), 0);
}

void
zt_bstream_write_uint8(zt_bstream_t bs, uint8_t data)
{
    zt_bstream_write(bs, (char *)&data, 1, sizeof(uint8_t), 0);
}

void
zt_bstream_write_uint16(zt_bstream_t bs, uint16_t data)
{
    zt_bstream_write(bs, (char *)&data, 1, sizeof(uint16_t), 0);
}

void
zt_bstream_write_uint32(zt_bstream_t bs, uint32_t data)
{
    zt_bstream_write(bs, (char *)&data, 1, sizeof(uint32_t), 0);
}

void
zt_bstream_write_uint64(zt_bstream_t bs, uint64_t data)
{
    zt_bstream_write(bs, (char *)&data, 1, sizeof(uint64_t), 0);
}

void
zt_bstream_write_float(zt_bstream_t bs, float data)
{
    zt_bstream_write(bs, (char *)&data, 1, sizeof(float), 0);
}

void
zt_bstream_write_double(zt_bstream_t bs, double data)
{
    zt_bstream_write(bs, (char *)&data, 1, sizeof(double), 0);
}
