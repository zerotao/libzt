#include "zt_gc.h"
#include "zt_assert.h"
/*
 * Implements a form of Baker's treadmill with 'write protection'
 *
 */

static int one_is_white(zt_gc_mark *mark) { return mark->colour == 1 ? 1 : 0; }
static void one_clear_white(zt_gc_mark *mark) { mark->colour = 0; }
static void one_set_white(zt_gc_mark *mark) { mark->colour = 1; }

static int zero_is_white(zt_gc_mark *mark) { return mark->colour == 0 ? 1 : 0; }
static void zero_clear_white(zt_gc_mark *mark) { mark->colour = 1; }
static void zero_set_white(zt_gc_mark *mark) { mark->colour = 0; }

static void switch_white(gc *gc)
{
	if (gc->is_white == NULL || gc->is_white == one_is_white) {
		gc->is_white = zero_is_white;
		gc->clear_white = zero_clear_white;
		gc->set_white = zero_set_white;
	} else {
		gc->is_white = one_is_white;
		gc->clear_white = one_clear_white;
		gc->set_white = one_set_white;
	}
}


static void
resize_rootset(gc *gc, int new_size)
{
	if (gc->rootset == NULL){
		gc->rootset = XCALLOC(zt_elist *, new_size);
	} else {
		int	diff = new_size - gc->rootset_size;
		int i;
		gc->rootset = XREALLOC(zt_elist *, gc->rootset, new_size);
		for( i = diff; i < new_size; i++) {
			gc->rootset[i] = NULL;
		}
	}	
	gc->rootset_size = new_size;
}

static void
dump_elist(char *name, zt_elist *p)
{
	zt_elist	* tmp;
	int			  c;
	
	printf("%s [", name);
	c = 1;
	zt_elist_for_each(p, tmp){
		if (tmp->next == p) {
			c = 0;
		}
		printf("%p%s", tmp, c ? ", " : "");
	}
	printf("]\n");	
}

void
zt_gc_init(gc *gc,
		void (*mark_fn)(struct gc *, void *),
		void (*release_fn)(struct gc *, void **),
		int marks_per_scan,
		int allocs_before_scan)
{
	

	zt_elist_reset(&gc->list_1);
	zt_elist_reset(&gc->list_2);
	zt_elist_reset(&gc->list_3);
	
	gc->rootset = XCALLOC(zt_elist *, 1024);
	gc->rootset_size = 1024;
	gc->rootset_next = 0;
	
	gc->black = &gc->list_1;
	gc->grey = &gc->list_2;
	gc->white = &gc->list_3;

	gc->scan = gc->grey;
	
	gc->marks_per_scan = marks_per_scan;
	gc->allocs_before_scan = allocs_before_scan;
	gc->current_allocs = 0;

	/* initialize white */
	gc->is_white = one_is_white;
	gc->clear_white = one_clear_white;
	gc->set_white = one_set_white;

	gc->mark_fn = mark_fn;
	gc->release_fn = release_fn;
}

void
zt_gc_register_root(gc *gc, void *v)
{
	zt_elist	* el = (zt_elist *)v;
	gc->rootset[gc->rootset_next++] = el;
	if (gc->rootset_next > gc->rootset_size) {
		resize_rootset(gc, gc->rootset_size + 1024);
	}
}

void
zt_gc_register_value(gc *gc, void *v) 
{
	zt_gc_mark		* mark = (zt_gc_mark *) v;
	printf("Registering value %p(%p)\n", mark, &mark->list);
	
	gc->clear_white(mark);
	zt_elist_reset(&mark->list);
	/* The mutator is registering interest in this value so we must place it in
	 * the the grey list no the white. */
	zt_elist_add_tail(gc->grey, &mark->list);
	
	if(++gc->current_allocs >= gc->allocs_before_scan){
		gc->current_allocs = 0;
		zt_gc_scan(gc, 0);
	}
}

void
zt_gc_free_white(gc *gc)
{
	zt_elist	* elt;
	zt_elist	* dont_use;
	
	zt_elist_for_each_safe(gc->white, elt, dont_use){
		zt_elist_remove(elt);
		gc->release_fn(gc, (void **)&elt);
	}
}

void
zt_gc_switch(gc *gc) 
{
	zt_elist	* tmp_white;
	/* in a traditional GC we would just swap the swap out the white */
	zt_gc_free_white(gc);
	tmp_white = gc->white;
	gc->white = gc->black;
	gc->black = tmp_white;
	switch_white(gc);
	
}

void
zt_gc_scan(gc *gc, int full_scan)
{
	int	  current_marks = 0;
	printf("Scanning starting\n");
	
	if (gc->scan == gc->grey) {
		printf(" Initializing scan\n");
		dump_elist("  scan", gc->grey);
		
		/* beginning of a scan */
		int	  i;
		for(i = 0; i < gc->rootset_next; i++) {
			/* add each object in the rootset into the grey list */
			zt_gc_mark	* mark	= (zt_gc_mark *) gc->rootset[i];
			//set_grey(mark);
			
			zt_elist_remove(&mark->list);
			zt_elist_add(gc->grey, &mark->list);
		}
		gc->scan = gc->scan->next;
	}
	

	while(gc->scan != gc->grey) {
		/* move thru each object in the grey list moving it to the
		 * black list and marking it as we go.
		 */
		zt_elist	* next;
		zt_elist	* elt;
		zt_gc_mark		* mark;
		printf(" Scanning %p(n:%p, p:%p)\n", gc->scan, gc->scan->next, gc->scan->prev);
		elt = gc->scan;
		mark = zt_elist_data(elt, zt_gc_mark, list);
		
		gc->mark_fn(gc, elt);		
		next = gc->scan->next;
		
		gc->clear_white(mark);
		zt_elist_remove(elt);
		zt_elist_add(gc->black, elt);
		
		gc->scan = next;

		if (!full_scan && ++current_marks >= gc->marks_per_scan) {
			if (gc->scan != gc->grey) {
				return;
			}
		}
	}
	printf("Scan Complete\n");
	zt_gc_switch(gc);
}

void
zt_gc_mark_value(gc *gc, void *value)
{
	zt_gc_mark	* mark = (zt_gc_mark *)value;
	assert(value != NULL);
	
	if (gc->is_white(mark)) {
		gc->clear_white(mark);
		zt_elist_remove(&mark->list);
		zt_elist_add(gc->scan, &mark->list);
	}
}

/* 
 * void zt_gc_mark(gc *gc)
 * {
 * 	
 * }
 */


void
zt_gc_print_heap(gc *gc)
{
	printf("Heap Dump\n============\n");
	printf("l1: %p l2: %p l3: %p\nblack: %p grey: %p white: %p\nscan: %p\n",
		   &gc->list_1, &gc->list_2, &gc->list_3,
		   gc->black, gc->grey, gc->white, gc->scan);
	
	dump_elist("Black", gc->black);
	dump_elist("Grey", gc->grey);
	dump_elist("White", gc->white);
}


