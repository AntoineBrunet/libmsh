#include "msh.h"
#include <stdlib.h>
#include <stdio.h>

#define debug printf

typedef enum section_type {
	FORMAT, NODES, ELEMS, OTHER
} section_t;


int _msh_read_section(FILE * f, section_t * t) {
	static char sn[30];
	int ret = fscanf(f, "$%29s", sn);
	if (ret < 1) { return 0; }
	debug("Found section:%s\n", sn);
	if (sn[0] == 'M') { *t = FORMAT; }
	else if (sn[4] == 's') { *t = NODES; }
	else if (sn[7] == 's') { *t = ELEMS; }
	else { *t = OTHER; }
	return 1;
}

void _msh_read_nodes(FILE * f, msh_t * m) {
	fscanf(f, "%d", &(m->nodes_nb));
	m->nodes = malloc(sizeof(node_t)*m->nodes_nb);
	for (int i = 0; i < m->nodes_nb; i++) {
		fscanf(f, "%d %lf %lf %lf", 
				&(m->nodes[i].id),
				&(m->nodes[i].x),
				&(m->nodes[i].y),
				&(m->nodes[i].z)
				);
		m->nodes[i].flags = 0;
	}
}

node_t * _msh_find_node(msh_t * m, int n) {
	if (m->nodes[n-1].id == n) {
		return &(m->nodes[n-1]);
	}
	// TODO Binary search
	return NULL;
}
void _msh_read_elems(FILE * f, msh_t * m) {
	static const int elems_size[] = {0,2,3,4,4};
	fscanf(f, "%d", &(m->elems_nb));
	m->elems = malloc(sizeof(elem_t)*m->elems_nb);
	for (int i = 0; i < m->elems_nb; i++) {
		elem_t * elm = m->elems + i;
		fscanf(f, "%d %u %d",
				&(elm->id),
				&(elm->type),
				&(elm->tags_nb));
		elm->nodes_nb = elems_size[elm->type];		
		elm->tags = malloc(elm->tags_nb * sizeof(tag_t));
		elm->nodes = malloc(elm->nodes_nb * sizeof(node_t*));
		for (int t = 0; t < elm->tags_nb; t++) {
			fscanf(f, "%d", elm->tags + t);
		}
		for (int n = 0; n < elm->nodes_nb; n++) {
			int node;
			fscanf(f, "%d", &node);
			elm->nodes[n] = _msh_find_node(m, node);
		}
	}
}

void _msh_close_section(FILE * f) {
	char tmp[100];
	tmp[0] = '\0';
	while (tmp[0] != '$') {
		if (fgets(tmp, 100, f) == NULL) { return; }
	}
}

msh_t * msh_load(FILE * file) {
	msh_t * new_mesh = malloc(sizeof(msh_t));
	section_t section;
	while (_msh_read_section(file, &section)) {
		switch (section) {
			case NODES:
				debug("NODES\n");
				_msh_read_nodes(file, new_mesh);
				break;
			case ELEMS:
				debug("ELEMS\n");
				_msh_read_elems(file, new_mesh);
				break;
			default:
				debug("OTHER %d\n", section);
				break;
		}
		_msh_close_section(file);
	}	
	return new_mesh;
}

void msh_print_info(const msh_t * msh) {
	printf("%d nodes\n",msh->nodes_nb);
	printf("%d elems\n",msh->elems_nb);
	for (int i = 0; i < msh->elems_nb; i++) {
		elem_t * elm = msh->elems + i;
		if (elm->type == TRIANGLE) {
			printf("Triangle %d in PG %d, GG %d\n", elm->id, elm->tags[0], elm->tags[1]);
		}
	}
}

void msh_free(msh_t * msh) {
	for (int i = 0; i < msh->elems_nb; i++) {
		free(msh->elems[i].tags);
		free(msh->elems[i].nodes);
	}
	free(msh->elems);
	free(msh->nodes);
	free(msh);
}

