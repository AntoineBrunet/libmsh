/**
 * Physical groups management.
 * This library can extract a list of physical groups from a msh mesh.
 * Each physical group is stored in a resizing array, and the set of
 * physical groups is stored in a sorted linked list.
 * @author Antoine Brunet
 */
#ifndef MSH_PG_H
#define MSH_PG_H
#include "msh.h"

/** 
 * Physical group structure. A physical group (or pg) is a set of mesh
 * elements associated to a common group tag.
 */
typedef struct msh_pg {
	int tag; // Group tag
	size_t size;
	size_t elems_nb; // Number of elements
	elem_t ** elems; // Array of pointers to elements
} msh_pg_t;

/**
 * Physical group linked list. Should be sorted according to the pg tags.
 */
typedef struct node_pg * msh_list_pg_t;
struct node_pg {
	msh_pg_t * head;
	msh_list_pg_t tail;
};

/**
 * Extract the list of physical surfaces from a mesh.
 * @param msh The mesh.
 * @return A sorted linked list of the physical surfaces of the mesh, which
 * needs to be freed with msh_free_msh_pg.
 */
msh_list_pg_t * msh_get_surf_pg(const msh_t * msh);

/**
 * Frees a pg list. Also frees each physical group in the list.
 */
void msh_free_list_pg(msh_list_pg_t * spg);

/**
 * Create a physical surface with given tag.
 * @param tag The surface tag.
 * @return An empty physical surface with the given tag. Needs to be freed
 * with msh_pg_free.
 */
msh_pg_t * msh_pg_create(int tag);

/**
 * Frees a physical groupe.
 */
void msh_pg_free(msh_pg_t * pg);

/**
 * Insert a new element to a group.
 * @param pg The physical group to insert to.
 * @param elem The mesh element to insert.
 */
void msh_pg_insert(msh_pg_t * pg, elem_t * elem);

/**
 * Initialize a pg list.
 * Should be freed with msh_free_list_pg.
 */
void msh_list_pg_init(msh_list_pg_t * map);

/**
 * Add an element to a group in the pg list.
 * @param map The pg list.
 * @param tag The tag of the target group.
 * @param elem The element to insert.
 */
void msh_list_pg_insert(msh_list_pg_t * map, int tag, elem_t * elem);

#endif
