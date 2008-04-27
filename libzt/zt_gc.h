#ifndef _ZT_GC_H_
#define _ZT_GC_H_

#include <libzt/zt.h>
#include <libzt/adt/zt_list.h>

BEGIN_C_DECLS
typedef struct zt_gc_collectable {
 	zt_elist	  list;
 	int	  		  colour;
} zt_gc_collectable_t;

typedef struct gc {
	int			  enabled;
	
	int			  current_allocs;
	
	int			  marks_per_scan;
	int			  allocs_before_scan;

	int			  rootset_size;
	int			  rootset_next;
	
	int 		(*is_white)(zt_gc_collectable_t *mark);
	void 		(*clear_white)(zt_gc_collectable_t *mark);
	void 		(*set_white)(zt_gc_collectable_t *mark);

	zt_elist	  **rootset;
	
	zt_elist	  list_1;
	zt_elist	  list_2;
	zt_elist	  list_3;
	
	zt_elist	* black;		/* scanned */
	zt_elist	* grey;			/* seen but not scanned */
	zt_elist	* white;		/* not seen */

	zt_elist	* scan;		    /* current offset into the grey list */

	void		* private_data;
	
	void		(*mark_fn)(struct gc *, void *, void *);
	void		(*release_fn)(struct gc *, void *, void **);
	
} gc_t;

void zt_gc_init(gc_t *gc,
				void *private_data,
				void (*mark_fn)(struct gc *, void *, void *), 
				void (*release_fn)(struct gc *, void *, void **),
				int marks_per_scan, 
				int allocs_to_scan);

void zt_gc_destroy(gc_t *gc);

void zt_gc_enable(gc_t *gc);
void zt_gc_disable(gc_t *gc);

void zt_gc_prepare_value(gc_t *gc, void *value);
void zt_gc_register_value(gc_t *gc, void *value);
void zt_gc_unregister_value(gc_t *gc, void *value);
void zt_gc_register_root(gc_t *gc, void *value);

/* void zt_gc_protect(gc *gc, void *value); */
void zt_gc_print_heap(gc_t *gc);
int zt_gc_mark_value(gc_t *gc, void *value);
void zt_gc_scan(gc_t *gc, int full_scan);
/* void zt_gc_mark(gc *gc); */

END_C_DECLS

#endif /*_ZT_GC_H_*/
