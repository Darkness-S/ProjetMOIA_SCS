#ifndef FONCTIONCLIENT_H
#define FONCTIONCLIENT_H

#include "./protocolColonne.h"

int connectToServ(char* ip_serv, int pt_serv); 

int exitAll(); 

int setPlayer(char name[30]); 

int gameQuery(); 

int gamePlay(int game); 

int answer(); 

int gameReceiveFromAI(TCoupReq* query); 

int gameQueryTurn(TCoupReq* query); 

int gameOpponentTurn(TCoupReq* query); 

int gameReadTurn(TCoupReq* query); 

int reponseServJeu1(TCoupRep* query); 

int gameGo(int nb); 

#endif 
