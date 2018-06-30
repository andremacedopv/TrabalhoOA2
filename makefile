IDIR =./include

CC = gcc
CFLAGS = -g -I$(IDIR) -lm 

ODIR = ./src/obj

SDIR = ./src

_DEPS = Arvore_B.h Registros.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = Arvore_B.o Registros.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: main

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
