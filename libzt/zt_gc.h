#ifndef _ZT_GC_H_
#define _ZT_GC_H_

#include <libzt/adt/zt_list.h>

typedef struct zt_gc_mark {
 	zt_elist	  list;
 	int	  		  colour;
} zt_gc_mark;

typedef struct gc {
	int			  current_allocs;
	
	int			  marks_per_scan;
	int			  allocs_before_scan;

	int			  rootset_size;
	int			  rootset_next;
	
	int 		(*is_white)(zt_gc_mark *mark);
	void 		(*clear_white)(zt_gc_mark *mark);
	void 		(*set_white)(zt_gc_mark *mark);

	zt_elist	  **rootset;
	
	zt_elist	  list_1;
	zt_elist	  list_2;
	zt_elist	  list_3;
	
	zt_elist	* black;		/* scanned */
	zt_elist	* grey;			/* seen but not scanned */
	zt_elist	* white;		/* not seen */

	zt_elist	* scan;		    /* current offset into the grey list */
	
	void		(*mark_fn)(struct gc *, void *);
	void		(*release_fn)(struct gc *, void **);
	
} gc;

void zt_gc_init(gc *gc, void (*mark_fn)(struct gc *, void *), 
                     void (*release_fn)(struct gc *, void **),
                     int marks_per_scan, 
                     int allocs_to_scan);

void zt_gc_register_value(gc *gc, void *value);
void zt_gc_register_root(gc *gc, void *value);
/* void zt_gc_protect(gc *gc, void *value); */
void zt_gc_print_heap(gc *gc);
void zt_gc_mark_value(gc *gc, void *value);
void zt_gc_scan(gc *gc, int full_scan);
/* void zt_gc_mark(gc *gc); */


#endif /*_ZT_GC_H_*/
