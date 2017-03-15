CC=gcc 
CFLAGS=-g -W -Wall 

MAINT1=TP5Serveur
MAINT2=TP5Client
FTCP=fonctionsTCP

all: $(MAINT1) $(MAINT2)  

$(MAINT1): $(MAINT1).o $(FTCP).o
	$(CC) $(CFLAGS) -o $(MAINT1) $(MAINT1).o $(FTCP).o

$(MAINT2): $(MAINT2).o fonctionsTCP.o
	$(CC) $(CFLAGS) -o $(MAINT2) $(MAINT2).o $(FTCP).o
	
$(MAINT2).o: $(MAINT2).c 
	$(CC) $(CFLAGS) -c -o $(MAINT2).o $(MAINT2).c 
	
$(MAINT1).o: $(MAINT1).c
	$(CC) $(CFLAGS) -c -o $(MAINT1).o $(MAINT1).c
	
#fonctionsTCP.a : fonctionsTCP.o
#	ar -q fonctionsTCP.a fonctionsTCP.o
	
$(FTCP).o : $(FTCP).c $(FTCP).h
	$(CC) $(CFLAGS) -c -o $(FTCP).o $(FTCP).c


clean:
	rm *.o; rm $(MAINT1) $(MAINT2); 
