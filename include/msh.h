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
	size_t nodes_nb;
	size_t tags_nb;
	node_t ** nodes;
	tag_t * tags;
} elem_t;

typedef struct msh {
	size_t nodes_nb;
	size_t elems_nb;
	node_t * nodes;	
	elem_t * elems;
} msh_t;

/**
 * Loads a mesh from a msh file.
 * @param c Open file with read flag.
 * @return The corresponding allocated mesh. Needs to be freed with
 * msh_free.
 */
msh_t * msh_load(FILE * c);

/**
 * Print mesh info
 */
void msh_print_info(const msh_t * msh);

/**
 * Frees a mesh allocated by msh_load.
 * @param msh The mesh to free.
 */
void msh_free(msh_t * msh);

#endif
