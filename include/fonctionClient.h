#ifndef _fonctionClient_h
#define _fonctionClient_h
#include "./protocolColonne.h"
int connectToServ(char* ip_serv, int pt_serv, int* socket);

int connectSocket(char* ip_serv, int pt_serv);  

int exitAll(); 

int setPlayer(char name[30]); 

int gameQuery(); 

int gamePlay(int game); 

int answer(); 

int gameReceiveFromAI(TCoupReq* query); 

int gameQueryTurn(TCoupReq* query); 

int gameOpponentTurn(TCoupReq* query); 

int gameReadTurn(TCoupReq* query); 

int gameReceive(TCoupRep* query); 

int gameGo(int nb); 

int gameEnd(TCoupRep* answer); 

#endif 
