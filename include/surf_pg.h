#ifndef SURF_PG_H
#define SURF_PG_H
#include "msh.h"

typedef struct surf_pg {
	int id;
	size_t size;
	size_t elems_nb;
	elem_t ** elems;
} surf_pg_t;

typedef struct node_pg * list_pg_t;
struct node_pg {
	surf_pg_t * head;
	list_pg_t tail;
};

list_pg_t * msh_get_surf_pg(const msh_t * msh);

void msh_free_surf_pg(list_pg_t * spg);

surf_pg_t * surf_pg_create(int tag);
void surf_pg_free(surf_pg_t * pg);

void surf_pg_insert(surf_pg_t * pg, elem_t * elem);

void map_pg_init(list_pg_t * map);

void map_pg_insert(list_pg_t * map, int tag, elem_t * elem);

#endif
