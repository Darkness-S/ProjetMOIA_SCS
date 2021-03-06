CC=gcc
CFLAGS=-W -Wall

OBJDIR=obj
OBJSERDIR=$(OBJDIR)/serveur
OBJCLIDIR=$(OBJDIR)/client
OBJCOMDIR=$(OBJDIR)/common
BINDIR=bin
SRCDIR=src
SRCSERDIR=$(SRCDIR)/serveur
SRCCLIDIR=$(SRCDIR)/client
TMPDIR=tmp
INCDIR=include

EXECS=$(BINDIR)/serveur
EXECC=$(BINDIR)/client

all : $(EXECS) $(EXECC)

serveur : $(EXECS)

client : $(EXECC)

$(EXECS): $(OBJSERDIR)/fonctionsServeur.o $(OBJSERDIR)/serveur.o $(OBJCOMDIR)/fonctionsTCP.o $(OBJCOMDIR)/colonne.o 
	@$(CC) $^ $(CFLAGS) -o $@

$(EXECC): $(OBJCLIDIR)/fonctionClient.o $(OBJCLIDIR)/client.o $(OBJCOMDIR)/fonctionsTCP.o $(OBJCOMDIR)/colonne.o 
	@$(CC) $^ $(CFLAGS) -o $@

$(OBJCLIDIR)/fonctionClient.o : $(INCDIR)/fonctionClient.h

$(OBJCLIDIR)/client.o : $(SRCCLIDIR)/client.c
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJCLIDIR)/fonctionClient.o : $(SRCCLIDIR)/fonctionClient.c
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJSERDIR)/fonctionsServeur.o : $(INCDIR)/fonctionsServeur.h

$(OBJSERDIR)/serveur.o : $(SRCSERDIR)/serveur.c
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJSERDIR)/fonctionsServeur.o : $(SRCSERDIR)/fonctionsServeur.c
	$(CC) -o $@ -c $< $(CFLAGS)
	
	
clean:
		rm $(OBJSERDIR)/*.o;
		rm $(OBJCLIDIR)/*.o;
		rm -rf $(SRCDIR)/*/*.*~;
		rm -rf $(OBJDIR)/*/*.*~;
		rm -rf $(INCDIR)/*.*~;
		rm -rf ./*.*~;
		rm -rf ./*~;
		rm $(EXECS);
		rm $(EXECC);

cleanS :
		rm $(OBJSERDIR)/*.o;
		rm -rf $(SRCDIR)/*/*.*~;
		rm -rf $(OBJDIR)/*/*.*~;
		rm -rf $(INCDIR)/*.*~;
		rm -rf ./*.*~;
		rm -rf ./*~;
		rm $(EXECS);
		
cleanC :
		rm $(OBJCLIDIR)/*.o;
		rm -rf $(SRCDIR)/*/*.*~;
		rm -rf $(OBJDIR)/*/*.*~;
		rm -rf $(INCDIR)/*.*~;
		rm -rf ./*.*~;
		rm -rf ./*~;
		rm $(EXECC);

mvin : 
		mv $(TMPDIR)/colonne.o $(OBJSERDIR); 
		mv $(TMPDIR)/fonctionsTCP.o $(OBJSERDIR);
