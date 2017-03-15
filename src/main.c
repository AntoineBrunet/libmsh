#include <stdio.h>
#include "msh.h"
#include "surf_pg.h"

int main(int argc, char * argv[]) {
	if (argc < 2) {
		printf("USAGE: %s <file.msh>", argv[0]);
	}
	FILE * f = fopen(argv[1], "r");
	msh_t * my_mesh = msh_load(f);
	msh_print_info(my_mesh);
	list_pg_t * phys =  msh_get_surf_pg(my_mesh);

	list_pg_t it = *phys;
	int size = 0;
	while (it != NULL) { 
		printf("Surface Phy. %d: %d elements\n", it->head->id, 
				it->head->elems_nb);
		size++;
		it = it->tail;
	}
	printf("Nombres de Surfaces Physiques: %d\n", size);

	msh_free(my_mesh);
}
