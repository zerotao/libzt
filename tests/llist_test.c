#include "test.h"
#include <libzt/adt/zt_llist.h>



int main(int argc, char **argv)
{
	char	* m = "c";
	zt_pair	* x, * y;
	
	x = zt_llist_cons("a", NULL);
	x = zt_llist_cons("b", x);

	y = zt_llist_reverse(x);

	m = (char *)zt_llist_nth(y, 1);
	printf("%s\n", m);

	

}
