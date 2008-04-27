#include <libzt/zt.h>
#include <libzt/zt_gc.h>
#include <libzt/zt_unit.h>

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

static int ints_marked = 0;
static int atoms_marked = 0;
static int atoms_freed = 0;

static void mark_atom(gc_t *gc, void *pdata, void *v)
{
	atom	*a = (atom *)v;
	if(a->type == INT) {
		ints_marked++;
	} else {
		atoms_marked++;
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

	atoms_freed++;
	free(*a);
	*a = NULL;
}

static void
basic_tests(struct zt_unit_test *test, void *data)
{
	gc_t		  gc;
	atom		* root;
	int			  i;

	root = XCALLOC(atom, 1);
	
	root->type = ATOM;
	root->value.atom = NULL;
	
	zt_gc_init(&gc, NULL, mark_atom, release_atom, 10, 1);
	zt_gc_register_root(&gc, root);

	//zt_gc_print_heap(&gc);
	for(i=0; i < 100; i++)
	{
 		atom	* a = XCALLOC(atom, 1);
		
		a->type = INT;
		a->value.number = i;
		if (i == 1) {
			root->value.atom = a;
		}
		
		zt_gc_register_value(&gc, a);
	}
	
	zt_gc_scan(&gc, 0);
	printf("Ints %d %d %d\n", ints_marked, atoms_marked, atoms_freed);
	zt_gc_scan(&gc, 1);
	printf("Ints %d %d %d\n", ints_marked, atoms_marked, atoms_freed);
	ZT_UNIT_ASSERT(test, ints_marked == 199);
	ZT_UNIT_ASSERT(test, atoms_marked == 102);
	ZT_UNIT_ASSERT(test, atoms_freed == 99);
	zt_gc_destroy(&gc);	
}

int
register_gc_suite(struct zt_unit *unit)
{
	struct zt_unit_suite	* suite;

	suite = zt_unit_register_suite(unit, "gc tests", NULL, NULL, NULL);
	zt_unit_register_test(suite, "basic", basic_tests);
	return 0;
}
