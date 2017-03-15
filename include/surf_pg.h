/**
 * Surfacic physical groups management library.
 * This library can extract a list of physical surfaces from a msh mesh.
 * Each physical surface is stored in a resizing array, and the set of
 * physical surfaces is stored in a sorted linked list.
 * @author Antoine Brunet
 */
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

/**
 * Extract the list of physical surfaces from a mesh.
 * @param msh The mesh.
 * @return A sorted linked list of the physical surfaces of the mesh, which
 * needs to be freed with msh_free_surf_pg.
 */
list_pg_t * msh_get_surf_pg(const msh_t * msh);

/**
 * Frees a surface list. Also frees each physical surfaces in the list.
 */
void msh_free_surf_pg(list_pg_t * spg);

/**
 * Create a physical surface with given tag.
 * @param tag The surface tag.
 * @return An empty physical surface with the given tag. Needs to be freed
 * with surf_pg_free.
 */
surf_pg_t * surf_pg_create(int tag);

/**
 * Frees a physical surface.
 */
void surf_pg_free(surf_pg_t * pg);

/**
 * Insert a new element to a surface.
 * @param pg The physical surface to insert to.
 * @param elem The mesh element to insert.
 */
void surf_pg_insert(surf_pg_t * pg, elem_t * elem);

/**
 * Initialize a surface list.
 * Should be freed with msh_free_surf_pg.
 */
void map_pg_init(list_pg_t * map);

/**
 * Add an element to a surface in the surface list.
 * @param map The surface list.
 * @param tag The tag of the target surface.
 * @param elem The element to insert.
 */
void map_pg_insert(list_pg_t * map, int tag, elem_t * elem);

#endif
