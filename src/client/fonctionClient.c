/* inclusions générales */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* inclusions pour les sockets */
#include <sys/socket.h>

/* inclusions pour close */
#include <unistd.h>

/* inclusion bibliothèque TCP */
#include "fonctionsTCP.h"
#include "fonctionClient.h"

#define TAIL_BUF 100
#define SIZE_NAME 30

char player[SIZE_NAME]; 
char chaineComm[256]; 

int setPlayer(char playerName[30]){
	strcpy(player, playerName); 
}

int socketServer; 
int socketIA; 

int exitAll(){
	shutdown(socketIA, SHUT_RDWR); 
    shutdown(sock, SHUT_RDWR); 
    close(socketIA); 
    close(sock); 
    return 0; 
}

int connexion(char* ip_serv, int pt_serv, int *sock){
	printf("Connect to %s, %d\n", ip_serv, pt_serv); 
    *sock = socketclient(ip_serv, pt_serv); 
    if (*sock < 0){
		perror("\t\nError connexion socketClient\n"); 
		exitAll();
		return -1;   
	}
	printf("Connected %s | %d",ip_serv, *sock); 
	return 1; 
}

int gameQuery(){
	TPartieReq query; 
	query.idRequest = PARTIE; 
	
	err = send(socketServer, &query, sizeof(TPartieReq), 0); 
	if( err != sizeof(TPartiqueReq)){
		perror("\t\nError in gameQuery function | Request fail"); 
		exitAll(); 
		return -1; 
	}	
	printf("PARTIE query send\n"); 
	answer(); 
	return 1; 
}

TPion pion; 
char opponentName[SIZE_NAME]; 
int answer(){
	TPartieRep answer; 
	err = recv(socketServer, &answer, sizeof(TPartieRep), 0); 
	if(err<0){
		perror("Error in answer); 
		exitAll(); 
		return -1; 
	}
	pion = answer.coul; 
	strcpy(opponentName, answer.nomAdvers); 
	printf("Color : %d | Opponent : %s\n", pion, opponentName); 
	return 1; 
}

int gameReceiveFromIA(TCoupReq* query){
	TCol colonne[3] = {A, B, C}; 
	TLigne ligne[3] = {UN, DEUX, TROIS}; 
	err = recv(socketIA, chaineComm, sizeof(chaineComm), 0); 
	if(err < 0){
		perror("Error in gameReceiveFromIA"); 
		exitAll(); 
		return -1; 
	}

	query->idRequest = COUP; 
	char* tokens; 
	tokens = strtok(chaineComm, separator); 
	switch(atoi(tokens)){
		case 0 : 
			query->typeCoup = POS_PION; 
			break; 

		case 1 : 
			query->typeCoup = DEPL_PION; 
			break; 
		
		case 2 : 
			query->typeCoup = PASSE; 
	}
	
	switch(query->typeCoup){
		case POS_PION: 
			query->action
	}
	

}
