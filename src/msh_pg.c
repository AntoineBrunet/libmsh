#include "msh_pg.h"
#include <stdlib.h>

msh_list_pg_t * msh_get_surf_pg(const msh_t * msh) {
	msh_list_pg_t * map = malloc(sizeof(msh_list_pg_t));
	msh_list_pg_init(map);
	for (size_t i = 0; i < msh->elems_nb; i++) {
		elem_t * elm = msh->elems + i;
		if (elm->type == TRIANGLE) {
			msh_list_pg_insert(map, elm->tags[0], elm);
		}
	}
	return map;
}

msh_list_pg_t * msh_get_volume_pg(const msh_t * msh) {
	msh_list_pg_t * map = malloc(sizeof(msh_list_pg_t));
	msh_list_pg_init(map);
	for (size_t i = 0; i < msh->elems_nb; i++) {
		elem_t * elm = msh->elems + i;
		if (elm->type == TETRAHEDRON) {
			msh_list_pg_insert(map, elm->tags[0], elm);
		}
	}
	return map;
}

void msh_free_list_pg(msh_list_pg_t * spg) {
	msh_list_pg_t it = *spg;
	while (it != NULL) {
		msh_list_pg_t next  = it->tail;
		msh_pg_free(it->head);
		free(it);
		it = next;
	}
}

msh_pg_t * msh_pg_create(int tag) {
	msh_pg_t * new_pg = malloc(sizeof(msh_pg_t));
	new_pg->tag = tag;
	new_pg->size = 1;
	new_pg->elems_nb = 0;
	new_pg->elems = malloc(sizeof(elem_t *));
	return new_pg;
}

void msh_pg_free(msh_pg_t * pg) {
	free(pg->elems);
	free(pg);
}

void msh_pg_insert(msh_pg_t * pg, elem_t * elem) {
	if (pg->elems_nb >= pg->size) {
		pg->size *= 2;
		pg->elems = realloc(pg->elems, pg->size*sizeof(elem_t));
	}
	pg->elems[pg->elems_nb++] = elem;
}

void msh_list_pg_init(msh_list_pg_t * map) { *map = NULL; }

void msh_list_pg_insert(msh_list_pg_t * map, int tag, elem_t * elem) {
	while (*map != NULL && (*map)->head->tag < tag) {
		map = &((*map)->tail);
	}
	if (*map == NULL || (*map)->head->tag != tag) { 
		msh_list_pg_t tail = *map;
		*map = malloc(sizeof(struct node_pg));
		(*map)->tail = tail;
		(*map)->head = msh_pg_create(tag);
	}
	msh_pg_insert((*map)->head, elem);
}
