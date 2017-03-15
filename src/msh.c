#include "msh.h"
#include <stdlib.h>
#include <stdio.h>


typedef enum section_type {
	FORMAT, NODES, ELEMS, OTHER
} section_t;


int _msh_read_section(FILE * f, section_t * t) {
	static char sn[30];
	int ret = fscanf(f, "$%29s", sn);
	if (ret < 1) { return 0; }
	if (sn[0] == 'M') { *t = FORMAT; }
	else if (sn[4] == 's') { *t = NODES; }
	else if (sn[7] == 's') { *t = ELEMS; }
	else { *t = OTHER; }
	return 1;
}

void _msh_read_nodes(FILE * f, msh_t * m) {
	fscanf(f, "%lu", &(m->nodes_nb));
	m->nodes = malloc(sizeof(node_t)*m->nodes_nb);
	for (size_t i = 0; i < m->nodes_nb; i++) {
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
	size_t pos = n-1;
	size_t min = 0, max = m->nodes_nb - 1;
	while (m->nodes[pos].id != n) {
		if (m->nodes[pos].id > n) {
			if (pos == min) { return NULL; }
			max = pos - 1;
		} else {
			if (pos == max) { return NULL; }
			min = pos + 1;
		}
		pos = min + (max-min)/2;
	}
	return m->nodes + pos;
}
void _msh_read_elems(FILE * f, msh_t * m) {
	static const int elems_size[] = {0,2,3,4,4};
	fscanf(f, "%lu", &(m->elems_nb));
	m->elems = malloc(sizeof(elem_t)*m->elems_nb);
	for (size_t i = 0; i < m->elems_nb; i++) {
		elem_t * elm = m->elems + i;
		fscanf(f, "%d %u %lu",
				&(elm->id),
				&(elm->type),
				&(elm->tags_nb));
		elm->nodes_nb = elems_size[elm->type];		
		elm->tags = malloc(elm->tags_nb * sizeof(tag_t));
		elm->nodes = malloc(elm->nodes_nb * sizeof(node_t*));
		for (size_t t = 0; t < elm->tags_nb; t++) {
			fscanf(f, "%d", elm->tags + t);
		}
		for (size_t n = 0; n < elm->nodes_nb; n++) {
			int node;
			fscanf(f, "%d", &node);
			elm->nodes[n] = _msh_find_node(m, node);
		}
	}
}

void _msh_read_format(FILE * f, msh_t * m) {
	fscanf(f, "%u.%u %u %u",
			&(m->format.major),
			&(m->format.minor),
			&(m->format.type),
			&(m->format.data_size)
		);

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
				_msh_read_nodes(file, new_mesh);
				break;
			case ELEMS:
				_msh_read_elems(file, new_mesh);
				break;
			case FORMAT:
				_msh_read_format(file, new_mesh);
			default:
				break;
		}
		_msh_close_section(file);
	}	
	return new_mesh;
}

void msh_print_info(const msh_t * msh) {
	printf("GMSH %u.%u %u %u\n", 
			msh->format.major,
			msh->format.minor,
			msh->format.type,
			msh->format.data_size
		);
	printf("%lu nodes\n",msh->nodes_nb);
	printf("%lu elems\n",msh->elems_nb);
}

void msh_free(msh_t * msh) {
	for (size_t i = 0; i < msh->elems_nb; i++) {
		free(msh->elems[i].tags);
		free(msh->elems[i].nodes);
	}
	free(msh->elems);
	free(msh->nodes);
	free(msh);
}

