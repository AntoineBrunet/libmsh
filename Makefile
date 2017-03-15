BIN_DEST = bin
CC = gcc
INCLUDES = -Iinclude
LDINCLUDES = -L$(BIN_DEST)
CFLAGS = $(INCLUDES) -Wall -pedantic -std=c99 -g
LDFLAGS = $(LDINCLUDES)
.DEFAULT = $(BIN_DEST)/msh2pgf

$(BIN_DEST)/%.o : src/%.c
	$(CC) $(CFLAGS) -c -o $@ $^

$(BIN_DEST)/main : $(BIN_DEST)/msh.o $(BIN_DEST)/surf_pg.o $(BIN_DEST)/main.o
	$(CC) $(LDFLAGS) -o $@ $^

$(BIN_DEST)/msh2pgf : $(BIN_DEST)/msh.o $(BIN_DEST)/surf_pg.o $(BIN_DEST)/msh2pgf.o
	$(CC) $(LDFLAGS) -o $@ $^
