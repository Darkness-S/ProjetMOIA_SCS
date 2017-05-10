CC=gcc
CFLAGS=-W -Wall

OBJDIR=obj
OBJSERDIR=$(OBJDIR)/serveur
OBJCLIDIR=$(OBJDIR)/client
BINDIR=bin
SRCDIR=src
SRCSERDIR=$(SRCDIR)/serveur
SRCCLIDIR=$(SRCDIR)/client
TMPDIR=tmp
INCDIR=include

SRCSER=$(wildcard $(SRCSERDIR)/*.c)
INC=$(wildcard $(INCDIR)/*.h)
OBJSER=$(SRCSER:$(SRCSERDIR)/%.c=$(OBJSERDIR)/%.o)

EXECS=$(BINDIR)/serveur
EXECC=$(BINDIR)/client

all : $(EXECS)

$(EXECS): $(OBJSER) $(OBJSERDIR)/*.o
	@$(CC) $^ $(CFLAGS) -o $@

$(OBJSERDIR)/main.o : $(INCDIR)/fonctionsServeur.h

$(OBJSERDIR)/%.o : $(SRCSERDIR)/%.c
	@$(CC) -o $@ -c $< $(CFLAGS)


clean:
		mv $(OBJSERDIR)/colonne.o $(TMPDIR); 
		mv $(OBJSERDIR)/fonctionsTCP.o $(TMPDIR);
		rm $(OBJSERDIR)/*.o;
		rm -rf $(SRCDIR)/*/*.*~;
		rm -rf $(OBJDIR)/*/*.*~;
		rm -rf $(INCDIR)/*.*~;
		rm -rf ./*.*~;
		rm -rf ./*~;
		mv $(TMPDIR)/colonne.o $(OBJSERDIR); 
		mv $(TMPDIR)/fonctionsTCP.o $(OBJSERDIR);
		rm $(EXECS);

mvin : 
		mv $(TMPDIR)/colonne.o $(OBJSERDIR); 
		mv $(TMPDIR)/fonctionsTCP.o $(OBJSERDIR); 
