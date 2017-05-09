CC=gcc
CFLAGS=-W -Wall

OBJDIR=obj
BINDIR=bin
SRCDIR=src
TMPDIR=tmp
INCDIR=include

SRC=$(wildcard $(SRCDIR)/*.c)
INC=$(wildcard $(INCDIR)/*.h)
OBJ=$(SRC:.c=.o)

EXECS=$(BINDIR)/serveur


all : $(EXECS)

$(EXECS): $(OBJ)
	@$(CC) -o $@ $^ $(CFLAGS)

$(OBJDIR)/main.o : $(INCDIR)/fonctionsServeur.h

$(OBJDIR)/%.o: $(SRCDIR)/Serveur/%.c
	@$(CC) -o $@ -c $< $(CFLAGS)

clean:
		mv $(OBJDIR)/colonne.o $(TMPDIR); 
		mv $(OBJDIR)/fonctionsTCP.o $(TMPDIR); 
		rm $(OBJDIR)/*.o; rm $(BINDIR)/$(EXECS);
		mv $(TMPDIR)/colonne.o $(OBJDIR); 
		mv $(TMPDIR)/fonctionsTCP.o $(OBJDIR); 
	
mvin : 
		mv $(TMPDIR)/colonne.o $(OBJDIR); 
		mv $(TMPDIR)/fonctionsTCP.o $(OBJDIR); 
