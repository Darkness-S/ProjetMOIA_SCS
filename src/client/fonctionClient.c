/* inclusions générales */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
/* inclusions pour les sockets */
#include <sys/socket.h>

/* According to POSIX.1-2001 */
#include <sys/select.h>

/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

/* inclusions pour close */
#include <unistd.h>

/* inclusion bibliothèque TCP */

#include "../../include/fonctionsTCP.h"
#include "../../include/fonctionClient.h"

#define SIZE_NAME 30

int err; 
char* IPTEST1 = "127.0.0.1"; 
char* IPTEST2 = "127.0.0.1"; 
int nbPort = 1235; 
char player[SIZE_NAME]; 
//player[0] = '\0'; 
char chaineComm[256]; 
fd_set readSet; 
int fdRS = sizeof(readSet)*2; 

int setPlayer(char playerName[30]){
	strcpy(player, ""); 
	strcat(player, playerName); 
	return 1; 
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

int connectToServ(char* ip_serv, int pt_serv, int* socket){
//	if(sock!=NULL){
		printf("Connect to %s, %d\n", ip_serv, pt_serv); 
		*socket = socketClient(ip_serv, pt_serv); 
		if (*socket < 0){
			perror("\t\nError connexion socketClient\n"); 
			exitAll();
			return -1;   
		}
		return 1; 
//	}
//	else
//	{
//		connectToServ(ip_serv, pt_serv, socketIA); 
//	}
}

int connectSocket(char* ip_serv, int pt_serv){
	connectToServ(ip_serv, pt_serv, &socketServer); 
	return 1; 
}

int gameQuery(){
	TPartieReq query; 
	query.idRequest = PARTIE; 
	strcat(query.nomJoueur, player);
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
	char* couleur;  
	err = recv(socketServer, &answer, sizeof(TPartieRep), 0); 
	if(err<0){
		perror("Error in answer"); 
		exitAll(); 
		return -1; 
	}
	pion = answer.coul; 
	strcpy(opponentName, answer.nomAdvers); 
	if(pion == 0) couleur = "BLANC";
	if(pion == 1) couleur = "NOIR"; 
	printf("Color : %s | Opponent : %s\n", couleur, opponentName); 
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

		case PASSE: 
			printf("Do nothing \n"); 
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

int gameOpponentTurn(TCoupRep* query){
	err = recv(socketServer, query, sizeof(TCoupRep), 0); 
	if( err != sizeof(TCoupRep)) {
		perror("Opponent turn error"); 
		exitAll(); 
		return -1; 
	}
	printf("Opponent turn done"); 
	return 1; 
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
		case PASSE: 
				printf("Do nothing\n"); 

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
	return 1; 
}

int gameGo(int nb){
	int state; 
	strcpy(chaineComm, ""); 
	//chaineComm[0] = '\0'; 
	TCoupReq query; 
	TCoupRep answer; 
	if(pion == BLANC){
		strcat(chaineComm, "BLANC"); 
		state = 1; 
	}
	else if(pion == NOIR){
		strcat(chaineComm, "NOIR"); 
		state = 0; 
	}
	
	//if(nb == 0){
		err = send(socketServer, &chaineComm, sizeof(chaineComm), 0); 		
			printf("Chaine comm : %s",&chaineComm); 
	//TODO//err = send(socketIA, &chaineComm, sizeof(chaineComm), 0); 
		if(err != sizeof(chaineComm)) {
			printf("Chaine comm : %s",&chaineComm); 
			perror("\n\tError sending color to server"); 
			exitAll(); 
			return -1; 
		}
	//}
	chaineComm[0] = '\0'; 
	strcpy(chaineComm, ""); 
	while(answer.validCoup == VALID && answer.propCoup == CONT){
		switch(state){
			case 0: 
				gameQueryTurn(&answer); 
				if(answer.validCoup == VALID){
					gameOpponentTurn(&query);
					gameReadTurn(&query); 
				}
				break; 

			case 1: 
				FD_ZERO(&readSet); 
				FD_SET(socketServer, &readSet); 
				FD_SET(socketIA, &readSet); 
				err  = select(fdRS, &readSet, NULL, NULL, NULL); 
				if(err < 0){
					perror("Select fail "); 
					exitAll(); 
					return -1; 
				}
				else if(FD_ISSET(socketIA, &readSet) != 0){
							gameReceiveFromAI(&query); 
							gameQueryTurn(&query); 
							gameOpponentTurn(&answer); 
				}
				else if(FD_ISSET(socketServer, &readSet) != 0){
							gameOpponentTurn(&answer); 
				}
				break; 			
		}
	}

	switch(answer.validCoup){
		case GAGNE:
			if (state == 1){
				printf("You win"); 
			}		
			else if (state == 0){
				printf("You lose"); 
			}
			break; 
		case PERDU: 
			if (state == 1){
				printf("You lose");
			}
			else if (state == 0){
				printf("You win"); 
			}
			break; 

		case NULLE: 
				printf("Equality"); 
				break; 
	}

	return 1; 
}

int gameEnd(TCoupRep* answer){
	switch(answer->validCoup){
		case VALID: 
			strcpy(chaineComm, "VALID;"); 
			break; 

		case TIMEOUT:
			strcpy(chaineComm, "TIMEOUT;"); 
			break; 

		case TRICHE: 
			strcat(chaineComm, "TRICHE;"); 
			break; 
	}
	switch(answer->propCoup){
		case CONT: 
			strcpy(chaineComm, "CONT;"); 
			break; 
		
		case GAGNE: 
			strcpy(chaineComm, "GAGNE;") ;
			break; 

		case NULLE: 
			strcpy(chaineComm, "NULLE;"); 
			break; 

		case PERDU: 
			strcpy(chaineComm, "PERDU;"); 
			break; 
	}
	err = send(socketIA, chaineComm, sizeof(chaineComm), 0); 
	if (err != sizeof(chaineComm)){
		perror("Send to server fail"); 
		exitAll(); 
		return -1; 
	}	
	return 1; 
}
