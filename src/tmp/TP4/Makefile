################################
#
# Makefile CS (client/serveur)
#  mode connecté
#  bibliothèque : fonctionsTCP
#
################################

# For Solaris
#LD_FLAGS = -lsocket -lnsl

# For Linux
LD_FLAGS =

all: client serveur

fonctionsSocket.o: fonctionsTCP.c fonctionsTCP.h
	gcc -c fonctionsTCP.c

client: client.c fonctionsTCP.o fonctionsTCP.h
	gcc client.c -o client fonctionsTCP.o  $(LD_FLAGS)

serveur: serveur.c fonctionsSocket.o fonctionsTCP.h
	gcc serveur.c -o serveur fonctionsTCP.o  $(LD_FLAGS)

clean:
	rm *~ ; rm -i \#* ; rm *.o; \
        rm client ; rm serveur
