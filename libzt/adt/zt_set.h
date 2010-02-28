#ifndef _ZT_SET_H_
#define _ZT_SET_H_

#include <libzt/zt.h>
#include <libzt/adt/zt_table.h>

BEGIN_C_DECLS
typedef struct zt_set zt_set;
struct zt_set {
    zt_table * tbl;
    long       length;
    int        (*match)(const void *key1, const void *key2, void *cdata);
    void       (*destroy)(void *data);
};
typedef int (*zt_set_iterator)(void *, void *);

#define zt_set_size(set) ((set)->size)

zt_set *zt_set_init(int (*match)(const void *key1, const void *key2, void *cdata),
                    void (*destroy)(void *data),
                    void *cdata);

void zt_set_destroy(zt_set *set);
int zt_set_insert(zt_set *set, const void *data);
int zt_set_remove(zt_set *set, void **data);
int zt_set_union(zt_set *setu, const zt_set *set1, const zt_set *set2);
int zt_set_intersection(zt_set *seti, const zt_set *set1, const zt_set *set2);
int zt_set_difference(zt_set *setd, const zt_set *set1, const zt_set *set2);
int zt_set_is_member(const zt_set *set1, const void *data);
int zt_set_is_subset(const zt_set *set1, const zt_set *set2);
int zt_set_is_equal(const zt_set *set1, const zt_set *set2);
int zt_set_for_each(zt_set *set, zt_set_iterator iterator, void *param);


END_C_DECLS
#endif /* _ZT_SET_H_ */
