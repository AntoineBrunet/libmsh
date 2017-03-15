# libmsh
This is a basic C library to manage gmsh meshes. It can load `.msh` files
and extract physical surfaces.

# msh2pgf
The `msh2pgf` tool allows conversion from the msh format to data files that
can be rendered with pgf. It first extract the list of physical surfaces
from the msh file, and then export the coordinates of the vertices of these
surfaces to a `.vrt` file. The triangles of the physical surfaces are
exported in a `.idx` file with the appropriate vertex numbering.

The `.vrt` and `.idx` files can be used in pgf like this: 

	\begin{axis}
		\addplot3[patch, patch table with point meta={my_mesh.msh.idx}] 
			file {my_mesh.msh.vrt};
	\end{axis}

See `data/render.tex` for an example.
