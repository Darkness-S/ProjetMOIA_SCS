/* inclusions générales */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
/* inclusions pour les sockets */
#include <sys/socket.h>

/* inclusions pour close */
#include <unistd.h>

/* inclusion bibliothèque TCP */
#include "../../include/fonctionsTCP.h"
#include "../../include/fonctionClient.h"
#include "../../include/protocolColonne.h"
#include "../../include/validation.h"

int err; 

char player[SIZE_NAME]; 
char chaineComm[256]; 

int setPlayer(char playerName[30]){
	strcpy(player, playerName); 
}

int socketServer; 
int socketIA; 

int exitAll(){
	shutdown(socketIA, SHUT_RDWR); 
    shutdown(socketServer, SHUT_RDWR); 
    close(socketIA); 
    close(socketServer); 
    return 0; 
}

int connexion(char* ip_serv, int pt_serv, int *sock){
	printf("Connect to %s, %d\n", ip_serv, pt_serv); 
    *sock = socketClient(ip_serv, pt_serv); 
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
	if( err != sizeof(TPartieReq)){
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
		perror("Error in answer"); 
		exitAll(); 
		return -1; 
	}
	pion = answer.coul; 
	strcpy(opponentName, answer.nomAdvers); 
	printf("Color : %d | Opponent : %s\n", pion, opponentName); 
	return 1; 
}

int gameReceiveFromAI(TCoupReq* query){
	char separator[] = ";"; 
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
			break;
	}
	
	switch(query->typeCoup){
		case POS_PION: 
			query->action.posPion.col = colonne[tokens[0]-'A']; 
			query->action.posPion.lg = ligne[tokens[1]-'1']; 
			break; 

		case DEPL_PION: 
			// Récupération des quatre valeurs : Deux de départ, deux d'arrivée
			query->action.deplPion.caseDep.col = colonne[tokens[0]-'A']; 
			query->action.deplPion.caseDep.lg = ligne[tokens[1]-'1']; 
			query->action.deplPion.caseArr.col = colonne[tokens[2]-'A']; 
			query->action.deplPion.caseArr.lg = ligne[tokens[3]-'A']; 
			break; 
	}

	query->coul = pion; 
	return 1; 
}

int gameQueryTurn(TCoupReq* query){
	err = send(socketServer, query, sizeof(TCoupReq), 0); 
	if ( err != sizeof(TCoupReq)) {
		perror("Error in COUP query"); 
		exitAll(); 
		return -1; 
	}
	printf("Turn done"); 
	return 1; 
}

int gameOpponentTurn(TCoupReq* query){
	err = recv(socketServer, query, sizeof(TCoupReq), 0); 
	if( err != sizeof(TCoupReq)) {
		perror("Opponent turn error"); 
		exitAll(); 
		return -1; 
	}
	printf("Opponent turn done"); 
}

int gameReadTurn(TCoupReq* query){
	TCol colonne; 
	TLigne ligne; 
	chaineComm[0] = '\0'; 

	strcpy(chaineComm, "COUP;");
	switch(query->typeCoup){
		case POS_PION: 
			strcat(chaineComm, "0;");
			break; 
		
		case DEPL_PION:
			strcat(chaineComm, "1;");
			break; 

		case PASSE:
			strcat(chaineComm, "2;"); 
			break; 

		default: 
			printf("Unknow type \"COUT\" "); 
			return 1; 
	}

	switch(query->typeCoup){
		case POS_PION:
			colonne = query->action.posPion.col; 
			switch(colonne){
				case A: 
					strcat(chaineComm, "A;");
					break; 
		
				case B:
					strcat(chaineComm, "B;");
					break; 
	
				case C:
					strcat(chaineComm, "C;"); 
					break; 

				default: 
					printf("Unknow type \"POS_PION_COLONNE\" "); 
					return 1; 
			}
			ligne = query->action.posPion.lg; 
			switch(ligne){
				case UN: 
					strcat(chaineComm, "1;"); 
					break; 
				case DEUX:
					strcat(chaineComm, "2;"); 
					break; 
				case TROIS: 
					strcat(chaineComm, "3;"); 
					break; 
				default: 
					printf("Unknow type \"POS_PION_LIGNE\" "); 
					return 1; 
			}
			break; 
	
		case DEPL_PION: 
			colonne = query->action.deplPion.caseDep.col; 
			switch(colonne){
				case A: 
					strcat(chaineComm, "A"); 
					break; 
				case B: 
					strcat(chaineComm, "B"); 
					break; 
				case C: 
					strcat(chaineComm, "C"); 
					break; 
				default: 
					printf("Unknow type \"POS_PION_COLONNE\" "); 
					return 1; 
			}
		//	ligne = query->action.posPion.lg; 
			ligne = query->action.deplPion.caseDep.lg; 
			switch(ligne){
				case UN: 
					strcat(chaineComm, "1"); 
					break; 
				case DEUX:
					strcat(chaineComm, "2"); 
					break; 
				case TROIS: 
					strcat(chaineComm, "3"); 
					break; 
				default: 
					printf("Unknow type \"DEPL_PION_LIGNE\" "); 
					return 1; 
			}
			colonne = query->action.deplPion.caseArr.col; 
			switch(colonne){
				case A: 
					strcat(chaineComm, "A;"); 
					break; 
				case B:
					strcat(chaineComm, "B;"); 
					break;
				case C:  
					strcat(chaineComm, "C;"); 
					break; 
				default: 
					printf("Unknow type \"DEPL_PION_COLONNE\" "); 
					return 1; 
			}
			ligne = query->action.deplPion.caseArr.lg; 
			switch(ligne){
				case UN: 
					strcat(chaineComm, "1;"); 
					break; 
				case DEUX:
					strcat(chaineComm, "2;"); 
					break; 
				case TROIS: 
					strcat(chaineComm, "3;"); 
					break; 
				default: 
					printf("Unknow type \"POS_PION_LIGNE\" "); 
					return 1; 
			}
	}

	switch(pion){
		case NOIR: 
			strcat(chaineComm, "BLANC;"); 
			break; 
		case BLANC: 
			strcat(chaineComm, "NOIR;"); 
			break; 
	}

	err = send(socketIA, chaineComm, sizeof(chaineComm), 0); 
	if (err != sizeof(chaineComm)){
		perror(" Error color pion "); 
		exitAll(); 
		return -1; 
	}
	printf("Turn done"); 
}

// A RETIRER TODO
int reponseServJeu1(TCoupRep* rep){
  	printf("hell\n");
	err = recv(socketServer, rep, sizeof(TCoupRep),0);
	if(err<0){
		perror("serveur :  erreur sur receive 1");
		exitAll();
		return -1;
	}
	return 1;
}

int gameGo(int nb){
	int state; 
	TCoupReq query; 
	TCoupRep answer; 
	if(pion == BLANC){
		strcpy(chaineComm, "BLANC"); 
		state = 1; 
	}
	else if(pion == NOIR){
		strcpy(chaineComm, "NOIR"); 
		state = 0; 
	}
	//if(pion != BLANC && !numPartie){
//		connexion2(
	//}

	if(nb == 0){
		err = send(socketIA, &chaineComm, sizeof(chaineComm), 0); 
		if(err != sizeof(chaineComm)) {
			perror("Error sending color to server"); 
			exitAll(); 
			return -1; 
		}
	}
	chaineComm[0] = '\0'; 
	while(answer.validCoup == VALID && answer.propCoup == CONT){
		switch(state){
			case 0: 
				reponseServJeu1(&query); 
				if(answer.validCoup == VALID){
					gameOpponentTurn(&query);
					gameReadTurn(&query); 
				}
				break; 

			case 1: 
				gameReceiveFromAI(&answer); 
				gameQueryTurn(&answer); 
				reponseServJeu1(&answer); 
				break; 			
		}
		if(state == 1){
			state = 1; 
		}
		else
		{
			state = 0; 
		}
	}

	return 1; 
}
