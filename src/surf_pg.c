#include "surf_pg.h"
#include <stdlib.h>

list_pg_t * msh_get_surf_pg(const msh_t * msh) {
	list_pg_t * map = malloc(sizeof(list_pg_t));
	map_pg_init(map);
	for (size_t i = 0; i < msh->elems_nb; i++) {
		elem_t * elm = msh->elems + i;
		if (elm->type == TRIANGLE) {
			map_pg_insert(map, elm->tags[0], elm);
		}
	}
	return map;
}

void msh_free_surf_pg(list_pg_t * spg) {
	list_pg_t it = *spg;
	while (it != NULL) {
		list_pg_t next  = it->tail;
		surf_pg_free(it->head);
		free(it);
		it = next;
	}
}

surf_pg_t * surf_pg_create(int tag) {
	surf_pg_t * new_pg = malloc(sizeof(surf_pg_t));
	new_pg->id = tag;
	new_pg->size = 1;
	new_pg->elems_nb = 0;
	new_pg->elems = malloc(sizeof(elem_t *));
	return new_pg;
}

void surf_pg_free(surf_pg_t * pg) {
	free(pg->elems);
	free(pg);
}

void surf_pg_insert(surf_pg_t * pg, elem_t * elem) {
	if (pg->elems_nb >= pg->size) {
		pg->size *= 2;
		pg->elems = realloc(pg->elems, pg->size*sizeof(elem_t));
	}
	pg->elems[pg->elems_nb++] = elem;
}

void map_pg_init(list_pg_t * map) { *map = NULL; }

void map_pg_insert(list_pg_t * map, int tag, elem_t * elem) {
	while (*map != NULL && (*map)->head->id < tag) {
		map = &((*map)->tail);
	}
	if (*map == NULL || (*map)->head->id != tag) { 
		list_pg_t tail = *map;
		*map = malloc(sizeof(struct node_pg));
		(*map)->tail = tail;
		(*map)->head = surf_pg_create(tag);
	}
	surf_pg_insert((*map)->head, elem);
}


