#include <stdio.h>
#include "msh.h"
#include "msh_pg.h"

void print_list(const char * name, msh_list_pg_t it) {
	int size = 0;
	while (it != NULL) { 
		printf("%s %d: %lu elements\n", name,
				it->head->tag, 
				it->head->elems_nb);
		size++;
		it = it->tail;
	}
	printf("Total number of %ss: %d\n", name, size);
}

int main(int argc, char * argv[]) {
	if (argc < 2) {
		printf("USAGE: %s <file.msh>", argv[0]);
	}
	FILE * f = fopen(argv[1], "r");
	msh_t * my_mesh = msh_load(f);
	fclose(f);

	msh_print_info(my_mesh);
	
	msh_list_pg_t * surf =  msh_get_surf_pg(my_mesh);
	print_list("Phy. Surface", *surf);
	msh_free_list_pg(surf);
	
	msh_list_pg_t * volume =  msh_get_volume_pg(my_mesh);
	print_list("Phy. Volume", *volume);
	msh_free_list_pg(volume);

	msh_free(my_mesh);
}
