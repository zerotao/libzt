#include <errno.h>

#include "zt_set.h"
#include "zt_assert.h"


zt_set *
zt_set_init(int (*match)(const void *key1, size_t key1_size,
                         const void *key2, size_t key2_size, void *cdata),
            void (*destroy)(void *data),
            void *cdata)
{
    zt_set * set;

    set = zt_malloc(zt_set, 1);

    set->tbl = zt_table_init(NULL, zt_table_hash_int, match, 128, 0, cdata);
    set->match = match;
    set->destroy = destroy;
    set->length = 0;

    return set;
}

static int
_destroy(void *key UNUSED, size_t key_size, void *datum, size_t datum_size, void *param)
{
    void (*destroy)(void *) = ((zt_set *)param)->destroy;

    if (destroy) {
        destroy((void *)datum);
    }

    return 0;
}

void
zt_set_destroy(zt_set *set)
{
    zt_assert(set);

    zt_table_for_each(set->tbl, _destroy, set);
    zt_table_destroy(set->tbl);

    zt_free(set);
}


int
zt_set_insert(zt_set *set, const void *data, size_t len)
{
    if (zt_set_is_member(set, data, len)) {
        return 1;
    }

    set->length++;
    return zt_table_set(set->tbl, (void *)data, len, (void *)data, len);
}

int zt_set_remove(zt_set *set, void **data)
{
    *data = zt_table_del(set->tbl, *data, 0);
    set->length--;
    return 0;
}

struct _set_pair {
    zt_set * s1;
    zt_set * s2;
};

static int _union(void *key UNUSED, size_t key_len, void *datum, size_t datum_len, void *param)
{
    struct _set_pair * p;

    p = (struct _set_pair *)param;

    return zt_set_insert(p->s2, datum, datum_len);
}

int zt_set_union(zt_set *setu, const zt_set *set1, const zt_set *set2)
{
    struct _set_pair s = { NULL, NULL };

    s.s2 = setu;

    zt_table_for_each(set1->tbl, _union, &s);
    zt_table_for_each(set2->tbl, _union, &s);

    return 0;
}

static int _intersect(void *key UNUSED, size_t key_len, void *datum, size_t datum_len, void *param)
{
    struct _set_pair * p;

    p = (struct _set_pair *)param;

    if (zt_set_is_member(p->s1, datum, datum_len)) {
        return zt_set_insert(p->s2, datum, datum_len);
    }
    return 0;
}

int zt_set_intersection(zt_set *seti, const zt_set *set1, const zt_set *set2)
{
    struct _set_pair p = { NULL, NULL };

    p.s1 = (zt_set *)set2;
    p.s2 = (zt_set *)seti;

    return zt_table_for_each(set1->tbl, _intersect, &p);
}

static int _difference(void *key UNUSED, size_t key_len, void *datum, size_t datum_len, void *param)
{
    struct _set_pair * p;

    p = (struct _set_pair *)param;

    if (!zt_set_is_member(p->s1, datum, datum_len)) {
        return zt_set_insert(p->s2, datum, datum_len);
    }
    return 0;
}

int zt_set_difference(zt_set *setd, const zt_set *set1, const zt_set *set2)
{
    struct _set_pair p = { NULL, NULL };

    p.s1 = (zt_set *)set2;
    p.s2 = (zt_set *)setd;

    return zt_table_for_each(set1->tbl, _difference, &p);
}

int zt_set_is_member(const zt_set *set, const void *data, size_t len)
{
    errno = 0;
    if (zt_table_get(set->tbl, data, len) != NULL) {
        return 1;
    }
    if (errno == 0) {
        return 1;
    }
    return 0;
}

static int _is_subset(void *key UNUSED, size_t key_len, void *datum, size_t datum_len, void *param)
{
    struct _set_pair * p = (struct _set_pair *)param;

    if (zt_set_is_member(p->s1, datum, datum_len)) {
        return 0;
    }

    return 1;
}

int zt_set_is_subset(const zt_set *set1, const zt_set *set2)
{
    struct _set_pair p = { NULL, NULL };

    p.s1 = (zt_set *)set2;

    if (set1->length > set2->length) {
        return 0;
    }

    return zt_table_for_each(set1->tbl, _is_subset, &p) == 0;
}

int zt_set_is_equal(const zt_set *set1, const zt_set *set2)
{
    struct _set_pair p = { NULL, NULL };

    p.s1 = (zt_set *)set2;

    if (set1->length != set2->length) {
        return 0;
    }

    return zt_table_for_each(set1->tbl, _is_subset, &p) == 0;
}


struct _iterator_data {
    zt_set_iterator iterator;
    void          * param;
};

static int _table_to_set_iterator(void *key UNUSED, size_t key_len, void *data, size_t data_len, void *param)
{
    struct _iterator_data * p = (struct _iterator_data *)param;

    return p->iterator(data, p->param);
}

int
zt_set_for_each(zt_set *set, zt_set_iterator iterator, void *param)
{
    struct _iterator_data d;

    zt_assert(set);
    zt_assert(iterator);

    d.iterator = iterator;
    d.param = param;

    return zt_table_for_each(set->tbl, _table_to_set_iterator, &d);
}
