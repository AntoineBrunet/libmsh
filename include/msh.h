#ifndef MSH_H
#define MSH_H

#include <stdio.h>

typedef struct node {
	int id;
	int flags;
	double x, y, z;
} node_t;

typedef int tag_t;

typedef enum elem_type {
	NONE,
	LINE,
	TRIANGLE,
	QUADRANGLE,
	TETRAHEDRON
} elem_type_t;

typedef struct elem {
	int id;
	elem_type_t type;
	int nodes_nb;
	int tags_nb;
	node_t ** nodes;
	tag_t * tags;
} elem_t;

typedef struct msh {
	int nodes_nb;
	int elems_nb;
	node_t * nodes;	
	elem_t * elems;
} msh_t;

msh_t * msh_load(FILE * c);
void msh_print_info(const msh_t * msh);
void msh_free(msh_t * msh);

#endif
