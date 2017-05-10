#ifndef FONCTIONCLIENT_H
#define FONCTIONCLIENT_H

int connectToServ(char* ip_serv, int pt_serv); 

int exitAll(); 

int setPlayer(char name[30]); 

int gameQuery(); 

int gamePlay(int game); 

int answer(); 

#endif 
