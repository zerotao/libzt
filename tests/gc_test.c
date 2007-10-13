#include <libzt/zt.h>
#include <libzt/zt_gc.h>

#define INT  1
#define ATOM 2

typedef struct atom {
	zt_gc_collectable_t	  mark;
	int		  type;
	union {
		struct atom	* atom;
		int			  number;
	}value;
}atom;

static void mark_atom(gc_t *gc, void *pdata, void *v)
{
	atom	*a = (atom *)v;
	if(a->type == INT) {
		printf("  Marking Number: %p (%d)\n", a, a->value.number);
	} else {
		printf("  Marking Atom: %p (%p)\n", a, a->value.atom);
		if (a->value.atom != NULL) {
			zt_gc_mark_value(gc, a->value.atom);
		}
	}
	return;
}

static void
release_atom(gc_t *gc, void *pdata, void **v)
{
	atom **a = (atom **)v;
	printf("Releasing Atom %p\n", *a);
	free(*a);
	*a = NULL;
}

int main(int argc, char *argv[])
{
	gc_t		  gc;
	atom	  	  root;
	int			  i;
	
	root.type = ATOM;
	root.value.atom = NULL;
	
	zt_gc_init(&gc, NULL, mark_atom, release_atom, 1, 1);
	zt_gc_register_root(&gc, &root);

	zt_gc_print_heap(&gc);
	for(i=0; i < 10; i++)
	{
 		atom	* a = XCALLOC(atom, 1);
		
		a->type = INT;
		a->value.number = i;
		if (i == 1) {
			root.value.atom = a;
		}
		
		zt_gc_register_value(&gc, a);
	}
	
	//zt_gc_print_heap(&gc);
	//printf("Scanning Again\n");
	zt_gc_scan(&gc, 0);
	//zt_gc_print_heap(&gc);
	zt_gc_scan(&gc, 1);
	//zt_gc_print_heap(&gc);
	return 0;
}

