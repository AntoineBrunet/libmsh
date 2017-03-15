#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "msh.h"
#include "surf_pg.h"

void generateOutputNames(const char * mshFile, char ** vrt, char ** idx){
	static const char extv[] = ".vrt";
	static const char exti[] = ".idx";
	size_t len = strlen(mshFile);
	*vrt = malloc(len+5);
	*idx = malloc(len+5);
	strcpy(*vrt, mshFile);	
	strcpy(*idx, mshFile);	
	strcpy((*vrt)+len, extv);	
	strcpy((*idx)+len, exti);	
}

int main(int argc, char * argv[]) {
	if (argc < 2) {
		printf("USAGE: %s <file.msh>", argv[0]);
		return -1;
	}
	FILE * fin = fopen(argv[1], "r");
	if (fin == NULL) { 
		perror(argv[1]); 
		return -2;
	}
	char * vrtFile, * idxFile;
	generateOutputNames(argv[1], &vrtFile, &idxFile);

	FILE * fvrt = fopen(vrtFile, "w");
	if (fvrt == NULL) { 
		perror(vrtFile);
		fclose(fin);
		return -2;
	}
	
	FILE * fidx = fopen(idxFile, "w");
	if (fidx == NULL) { 
		perror(idxFile);
		fclose(fin);
		fclose(fvrt);
		return -3;
	}
	free(vrtFile);
	free(idxFile);
	
	msh_t * my_mesh = msh_load(fin);
	fclose(fin);

	list_pg_t * phys_grps = msh_get_surf_pg(my_mesh);
	
	for (list_pg_t it = *phys_grps; it != NULL; it = it->tail) {
		elem_t ** elems = it->head->elems;
		for (int i = 0; i < it->head->elems_nb; i++) {
			for (int v = 0; v < elems[i]->nodes_nb; v++) {
				elems[i]->nodes[v]->flags = 1;
			}
		}
	}
	
	int nb_nodes = 0;
	for (int i = 0; i < my_mesh->nodes_nb; i++) {
		node_t * node = my_mesh->nodes + i;
		if (node->flags) {
			node->id = nb_nodes++;
			fprintf(fvrt, "%lf\t%lf\t%lf\n",node->x, node->y, node->z);
		}
	}
	fclose(fvrt);
	
	int color = 0;
	for (list_pg_t it = *phys_grps; it != NULL; it = it->tail) {
		elem_t ** elems = it->head->elems;
		for (int i = 0; i < it->head->elems_nb; i++) {
			for (int v = 0; v < elems[i]->nodes_nb; v++) {
				fprintf(fidx, "%d\t", elems[i]->nodes[v]->id);
			}
			fprintf(fidx, "%d\n", color);
		}
		color++;
	}
	fclose(fidx);

	msh_free_surf_pg(phys_grps);
	msh_free(my_mesh);
}
