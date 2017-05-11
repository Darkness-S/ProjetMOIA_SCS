/*
********************************************************************
 *
 *  Programme : serveur.c
 *
 *  ecrit par : Sammy Loudiyi.
 *
 *  resume :    Serveur du jeu "Colonnes de trois".
 *
 *  date :      15 / 03 / 17
 *
 *******************************************************************
 */
/* include generaux */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdbool.h>
/* include pour close */
#include <unistd.h>

/* include pour sockets */
#include <sys/socket.h>
#include <netinet/in.h>
/* include pour les fonctions du serveur*/
#include "../../include/fonctionsServeur.h"

/* include pour le protocole de communication */

#include "../../include/validation.h"
#define TAIL_BUF 100
#define TIME_MAX 6


int	nbJoueurAct=0,
	etatPartie=0,
	nbPartie=0,
	ordreJoueur=0,
	sock_cont, 
	sock_transJ1,
	sock_transJ2,	/* descripteurs des sockets locales */
	*j1, *j2, *jTemp,
	err;	/* code d'erreur */

/*int scoreJ1[3], scoreJ2[3];
scoreJ1[0]=0;scoreJ1[1]=0;scoreJ1[2]=0;
scoreJ2[0]=0;scoreJ2[1]=0;scoreJ2[2]=0;*/

struct sockaddr_in nom_transmis;	/* adresse de la socket de transmission */
static fd_set readSet;
char	buffer[TAIL_BUF],
		nomJ1[TNOM],
		nomJ2[TNOM];	/* buffer de reception */
		nomTemp1[TNOM],
		nomTemp2[TNOM];

TPartieReq	parReqJ1, parReqJ2, parReqTemp;
TPartieRep	parRepJ1, parRepJ2, parRepTemp;
TCoupReq	parCoupReq;
TCoupRep	parCoupRep;
socklen_t	size_addr_trans;	/* taille de l'adresse d'une socket */
 

  
int demPartie(int port) {
	/* 
	* creation de la socket, protocole TCP 
	*/
	printf("(serveur) creation de la socket sur %d\n", port);
	sock_cont = socketServeur(port);
	if (sock_cont < 0) {
		perror("serveur : erreur socketServeur");
		return 1;
	}
	/*
	*  initialisation de la taille de l'adresse d'une socket
	*/
	size_addr_trans = sizeof(struct sockaddr_in);
	/*
	* attente de connexion
	*/
	

	sock_transJ1 = accept(sock_cont, (struct sockaddr *)&nom_transmis, &size_addr_trans);
	if (sock_transJ1 < 0) {
		perror("serveur :  erreur sur accept (sock_transJ1)");
		close(sock_cont);
		return 2; 
	}
	printf("Connexion J1\n");
	sock_transJ2 = accept(sock_cont,(struct sockaddr *)&nom_transmis, &size_addr_trans);
	if (sock_transJ2 < 0) {
		perror("serveur :  erreur sur accept (sock_transJ2)");
		shutdown(sock_transJ1, SHUT_RDWR); close(sock_transJ1);
		close(sock_cont);
		return 3; 
	}
	printf("Connexion J2\n");
	FD_ZERO(&readSet);
	FD_SET(sock_transJ1, &readSet);
	FD_SET(sock_transJ2, &readSet);
 	nbJoueurAct = sizeof(&readSet)*2; 	
 	receptPartie();
 	affecterJoueur(ordreJoueur);
 	affectRep();
	repDemPartie();
	jouerPartie();
	closeSock();
	return 0;
}

int receptPartie(){
	struct timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
	err = select(nbJoueurAct, &readSet, NULL, NULL, &timeout);
 	if (err < 0) {
		perror("serveur: erreur dans la selection du socket");
		closeSock();
		return 4;
	}else if(FD_ISSET(sock_transJ1, &readSet)!=0){
		err = recv(sock_transJ1, &parReqJ1, sizeof(TPartieReq), 0);
		ordreJoueur=1;
		if (err < 0) {
			perror("serveur: erreur dans la reception (J1 : sock_transJ1)");
			closeSock();
			return 5;
		}
		if (parReqJ1.idRequest!=0){
			parRepJ1.err=3;
			parRepJ1.coul=0;
			parRepJ1.nomAdvers[0]='\0';
		}
		err = recv(sock_transJ2, &parReqJ2, sizeof(TPartieReq), 0);
		if (err < 0 && parReqJ1.idRequest==0) {
			perror("serveur: erreur dans la reception (J2 : sock_transJ2)");
			closeSock();
			return 6;
		} 
		if (parReqJ2.idRequest!=0){
			parRepJ2.err=3;
			parRepJ2.coul=0;
			parRepJ2.nomAdvers[0]='\0';
		}	 
	}else if(FD_ISSET(sock_transJ2, &readSet)!=0){
		err = recv(sock_transJ2, &parReqJ1, sizeof(TPartieReq), 0);
		ordreJoueur=2;
		if (err < 0) {
			perror("serveur: erreur dans la reception(J1 : sock_transJ2)");
			closeSock();
			return 7;
		}
		if (parReqJ1.idRequest!=0){
			parRepJ1.err=3;
			parRepJ1.coul=0;
			parRepJ1.nomAdvers[0]='\0';
		}
		err = recv(sock_transJ1, &parReqJ2, sizeof(TPartieReq), 0);
		if (err < 0 && parReqJ1.idRequest==0) {
			perror("serveur: erreur dans la reception(J2 : sock_transJ1)");
			closeSock();
			return 8;
		} 
		if (parReqJ2.idRequest!=0){
			parRepJ2.err=3;
			parRepJ2.coul=0;
			parRepJ2.nomAdvers[0]='\0';
		}	
	}
	return 0;
}

int jouerPartie(){
	while (nbPartie<2){
		if(nbPartie==0){
			nbJoueurAct=0;
		}else{
			nbJoueurAct=1;
		}
		printf("Début partie %d\n", nbPartie+1);
		while (etatPartie<1){
			FD_ZERO(&readSet);
			FD_SET(sock_transJ1, &readSet);
			FD_SET(sock_transJ2, &readSet);
			jouerCoupPar();
			if(nbJoueurAct==0){
				jTemp = j2;
				j2 = j1;
				j1 = jTemp;
				nbJoueurAct=1;				
			}else{
				jTemp = j1;
				j1 = j2;
				j2 = jTemp;
				nbJoueurAct=0;
			}
		}
		nbPartie++;
		/*jTemp = j2;
		j2 = j1;
		j1 = jTemp;
		strcpy(nomTemp, nomJ2);
		strcpy(nomJ2, nomJ1);
		strcpy(nomJ1, nomTemp);*/
	}
}

int jouerCoupPar(){
	struct timeval timeout;
	timeout.tv_sec = 6;
	timeout.tv_usec = 0;
	printf("----%d----\n",nbJoueurAct);
	err=select(nbJoueurAct, &readSet, NULL, NULL, &timeout);
	if (err == 0) {
		perror("serveur : erreur dans la selection du socket (timeout)");
		parCoupRep.err=2;
		parCoupRep.validCoup=1;
		parCoupRep.propCoup=3;
		repCoupPartie();
		etatPartie++;
	}else if(err < 0){
		perror("serveur : erreur dans la selection du socket (non timeout)");
		closeSock();
		return 12;
	}else{
		err = recv(*j1, &parCoupReq, sizeof(TCoupReq), 0);
		if (err < 0) {
			perror("serveur: erreur dans la reception (J1)");
			closeSock();
			return 4;
		}
		if(parCoupReq.coul==0){
			printf("Joueur Blanc :\n");
		}else{
			printf("Joueur Noir :\n");
		}
		if (parCoupReq.idRequest!=1){
			parCoupRep.err=3;
			parCoupRep.validCoup=2;
			parCoupRep.propCoup=3;
			repCoupPartie();
			etatPartie++;
		}else{
			if(validationCoup(nbJoueurAct+1, parCoupReq, &parCoupRep.propCoup)==true){
				parCoupRep.err=0;
				parCoupRep.validCoup=0;
				repCoupPartie();
				if(parCoupRep.propCoup==0){
					reqCoupPartieAdv();
				}else{
					messageFinPartie();
				}
			}	
		}
	}
	return 0;
}

void affectRep(){
 	parRepJ1.err=0;
	parRepJ1.coul=0;
	strcpy(parRepJ1.nomAdvers, parReqJ2.nomJoueur);
	strcpy(nomJ1, parReqJ1.nomJoueur);
	parRepJ2.err=0;
	parRepJ2.coul=1;
	strcpy(parRepJ2.nomAdvers, parReqJ1.nomJoueur);
	strcpy(nomJ2, parReqJ2.nomJoueur);
}

int affecterJoueur(int oj){
	if(oj==1){
		j1=&sock_transJ1;
		j2=&sock_transJ2;
	}else if(oj==2){
		j1=&sock_transJ2;
		j2=&sock_transJ1;
	}else{
		perror("Erreur sur la définition de lordre de partie");
		printf("(ordreJoueur = %d)\n", oj);
		closeSock();
		return 9;
	}
	return 0;
}

int repDemPartie(){
	err=send(*j1, &parRepJ1, sizeof(TPartieRep), 0);
	if (err != sizeof(TPartieRep)){
		perror("Serveur : Erreur sur le renvoi de la réponse de la requète de partie (J1)");
		closeSock();
		return 10;
	}
	err=send(*j2, &parRepJ2, sizeof(TPartieRep), 0);
	if (err != sizeof(TPartieRep)){
		perror("Serveur : Erreur sur le renvoi de la réponse de la requète de partie (J2)");
		closeSock();
		return 11 ;
	}
	printf("Joueur 1 : %s, Joueur 2 : %s\n", &nomJ1, &nomJ2);  
	return 0;
}

int repCoupPartie(){
	err=send(*j1, &parCoupRep, sizeof(TCoupRep),0);
	if (err != sizeof(TCoupRep)){
		perror("Serveur : Erreur sur le renvoi de la réponse de la requète de coup (joueur 1)");
		closeSock();
		return 13;
	}
	err=send(*j2, &parCoupRep, sizeof(TCoupRep),0);
	if (err != sizeof(TCoupRep)){
		perror("Serveur : Erreur sur le renvoi de la réponse de la requète de coup (joueur 1)");
		closeSock();
		return 13;
	}
	printf("Serveur : envoi réponse coup. Code erreur : %d, validité du coup : %d, propriété du coup : %d.\n", parCoupRep.err, parCoupRep.validCoup, parCoupRep.propCoup);
	return 0;
}

int reqCoupPartieAdv(){
	err=send(*j2, &parCoupReq, sizeof(TCoupReq),0);
	if (err != sizeof(TCoupReq)){
		perror("Serveur : Erreur sur le renvoi de la requète de coup à l'adversaire");
		closeSock();
		return 13;
	}
	printf("Serveur : envoi coup joué à l'adversaire.\n");
	return 0;
}

void messageFinPartie(){
	if((nbJoueurAct==0 && nbPartie==0)||(nbJoueurAct==1 && nbPartie==1)){
		strcpy(nomTemp1, nomJ1);
		strcpy(nomTemp2, nomJ2);
	}else{
		strcpy(nomTemp1, nomJ2);
		strcpy(nomTemp2, nomJ1);
	}
	switch(parCoupRep.propCoup){
		case 1 :
		 	printf("Fin partie. Gagnant : %s - Perdant : %s.\n", &nomTemp1, &nomTemp2);
		 	break; 
		case 2 :
			printf("Fin partie. Egalité entre %s et %s.\n", &nomTemp1, &nomTemp2);
			break; 
		case 3 :
		 	printf("Fin partie. Gagnant : %s - Perdant : %s.\n", &nomTemp2, &nomTemp1);
		 	break; 
		default :
			perror("Erreur sur la propriété du coup");
			break;
	}
}

void closeSock(){
	shutdown(sock_transJ1, SHUT_RDWR); close(sock_transJ1);
	shutdown(sock_transJ2, SHUT_RDWR); close(sock_transJ2);
	close(sock_cont);
}
