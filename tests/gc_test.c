#include <libzt/zt.h>
#include <libzt/zt_gc.h>
#include <libzt/zt_unit.h>

#define DEBUG 0

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
		/* printf("Marking Int: %p\n", (void *)a); */
		ints_marked++;
	} else {
		/* printf("Marking Atom: %p %p\n", (void *)a, (void *)a->value.atom); */
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
	/* printf("release %p\n", *v); */
	
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
	atom		* protected;

#define RSTART 1
#define REND 100
#define MARKS_PER 100
#define ALLOCS_PER 1

	
	root = XCALLOC(atom, 1);
	
	root->type = ATOM;
	root->value.atom = NULL;
	
	zt_gc_init(&gc, NULL, mark_atom, release_atom, MARKS_PER, ALLOCS_PER);
	zt_gc_register_root(&gc, root);

	/* zt_gc_print_heap(&gc); */
	for(i=RSTART; i <= REND; i++)
	{
 		atom	* a = XCALLOC(atom, 1);

		/* you can register a value immediatly but it must be setup
		 * before you allocate another
		 */
		zt_gc_register_value(&gc, a);
		
		if (i == RSTART ) {
			a->type = INT;
			a->value.number = i;
			root->value.atom = a;
		} else if (i == RSTART+1) {
			a->type = INT;
			a->value.number = i;
			zt_gc_protect(&gc, a);
			protected = a;
		} else { /* if(i / 2 == 0 || last == NULL) { */
			a->type = INT;
			a->value.number = i;
			/* last = a; */
			/* protecting a value makes certain that it will be
			 * checked each scan
			 */
			/* zt_gc_protect(&gc, a); */
		} /* else {
		 * 	a->type = ATOM;
		 * 	a->value.atom = last;
		 * 	/\* unprotecting a value leaves it available to be GC'd *\/
		 * 	zt_gc_unprotect(&gc, last);
		 * 	last = NULL;
		 * } */
		
		
		
	}

	{
		int		  mark_base = REND;
		int		  free_base = (REND - 3) - 1; /* - 3 for root, root->value.a, protected - 1 for last object added*/
		int		  int_seen_base = ((REND * 3) - 6); /* root->value.a, protected, last_object need to be subtracted out */

		/* for the last object placed on the scan queue */
#if DEBUG
		printf("Base seen: %d mark: %d free: %d\n", int_seen_base, mark_base, free_base);
		printf("First seen: %d mark: %d free: %d\n", ints_marked, atoms_marked, atoms_freed);
#endif
		ZT_UNIT_ASSERT(test, ints_marked == int_seen_base);
		ZT_UNIT_ASSERT(test, atoms_marked == mark_base);
		ZT_UNIT_ASSERT(test, atoms_freed == free_base);

		/* second pass */
		zt_gc_scan(&gc, 1);

		int_seen_base += 3;		/* root->value.a + protected + last_alloc */
		mark_base += 1;			/* root->value.a */
		free_base += 1;
#if DEBUG		
		printf("Base seen: %d mark: %d free: %d\n", int_seen_base, mark_base, free_base);
		printf("Second seen: %d mark: %d free: %d\n", ints_marked, atoms_marked, atoms_freed);
#endif
		ZT_UNIT_ASSERT(test, ints_marked == int_seen_base);
		ZT_UNIT_ASSERT(test, atoms_marked == mark_base);
		ZT_UNIT_ASSERT(test, atoms_freed == free_base);

		/* third pass */
		zt_gc_scan(&gc, 1);

		int_seen_base += 2;		/* protected + root->value.a */
		mark_base += 1;			/* root->value.a */
		free_base += 1;
#if DEBUG
		printf("Base seen: %d mark: %d free: %d\n", int_seen_base, mark_base, free_base);
		printf("Third seen: %d mark: %d free: %d\n", ints_marked, atoms_marked, atoms_freed);
#endif
		ZT_UNIT_ASSERT(test, ints_marked == int_seen_base);
		ZT_UNIT_ASSERT(test, atoms_marked == mark_base);
		ZT_UNIT_ASSERT(test, atoms_freed == free_base);

		/* final */
		zt_gc_destroy(&gc);
		/* printf("Destory %d %d %d\n", ints_marked, atoms_marked, atoms_freed); */
		
		int_seen_base += 2;
		mark_base += 1;			/* root->value.a */
		free_base += 3;			/* root, root->value.a, protected */
#if DEBUG
		printf("Base seen: %d mark: %d free: %d\n", int_seen_base, mark_base, free_base);
		printf("Final seen: %d mark: %d free: %d\n", ints_marked, atoms_marked, atoms_freed);
#endif
		ZT_UNIT_ASSERT(test, ints_marked == int_seen_base);
		ZT_UNIT_ASSERT(test, atoms_marked == mark_base);
		ZT_UNIT_ASSERT(test, atoms_freed == free_base);
	}
	
	
}

int
register_gc_suite(struct zt_unit *unit)
{
	struct zt_unit_suite	* suite;

	suite = zt_unit_register_suite(unit, "gc tests", NULL, NULL, NULL);
	zt_unit_register_test(suite, "basic", basic_tests);
	return 0;
}
