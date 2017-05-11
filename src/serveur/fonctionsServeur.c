/*
********************************************************************
 *
 *  Programme : fonctionsServeur.c
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

#define TIME_MAX 6


int	nbJoueurAct=0,
	etatPartie=0,
	nbPartie=0,
	ordreJoueur=0,
	sock_cont, 
	sock_transJ1,
	sock_transJ2,	/* descripteurs des sockets locales */
	*joueur1, *joueur2, *jTemp,
	pgJ1=0, pgJ2=0, ppJ1=0, ppJ2=0, pnJ1=0, pnJ2=0,
	err, errTimeout;	/* code d'erreur */


struct sockaddr_in nom_transmis;	/* adresse de la socket de transmission */
static fd_set readSet;
char	nomJ1[TNOM],
		nomJ2[TNOM];	/* buffer de reception */

TPartieReq	parReqJ1, parReqJ2;
TPartieRep	parRepJ1, parRepJ2;
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
	afficherScore();
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
	nbJoueurAct=0;
	while (nbPartie<2){
		if(nbPartie==1){
			nbJoueurAct=1;
			jTemp = joueur2;
			joueur2 = joueur1;
			joueur1 = jTemp;
		}
		etatPartie=0;
		printf("Début partie %d\n", nbPartie+1);
		while (etatPartie<1){
			jouerCoupPar();
			if(nbJoueurAct==0){
				jTemp = joueur2;
				joueur2 = joueur1;
				joueur1 = jTemp;
				nbJoueurAct=1;				
			}else{
				jTemp = joueur1;
				joueur1 = joueur2;
				joueur2 = jTemp;
				nbJoueurAct=0;
			}
		}
		nbPartie++;
	}
}

int jouerCoupPar(){
	struct timeval timeout;
	timeout.tv_sec = TIME_MAX;
	timeout.tv_usec = 0;
	if(nbPartie==0){
		errTimeout=select(nbJoueurAct, &readSet, NULL, NULL, &timeout);
		if (errTimeout == 0) {
			perror("serveur : erreur dans la selection du socket (timeout)");
			parCoupRep.err=2;
			parCoupRep.validCoup=1;
			parCoupRep.propCoup=3;
			repCoupPartie();
			messageFinPartieTimeout();
			etatPartie++;
		}else if(errTimeout < 0){
			perror("serveur : erreur dans la selection du socket (non timeout)");
			closeSock();
			return 12;
		}else if(errTimeout>0){
			err = recv(*joueur1, &parCoupReq, sizeof(TCoupReq), 0);
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
						etatPartie++;
					}
				}	
			}
		}
	}else{
		errTimeout=select(nbJoueurAct, &readSet, NULL, NULL, &timeout);
		if (errTimeout == 0) {
			perror("serveur : erreur dans la selection du socket (timeout)");
			parCoupRep.err=2;
			parCoupRep.validCoup=1;
			parCoupRep.propCoup=3;
			repCoupPartie();
			messageFinPartie();
			etatPartie++;
		}else if(errTimeout < 0){
			perror("serveur : erreur dans la selection du socket (non timeout)");
			closeSock();
			return 12;
		}else if(errTimeout>0){
			err = recv(*joueur2, &parCoupReq, sizeof(TCoupReq), 0);
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
				if(validationCoup(nbJoueurAct, parCoupReq, &parCoupRep.propCoup)==true){
					parCoupRep.err=0;
					parCoupRep.validCoup=0;
					repCoupPartie();
					if(parCoupRep.propCoup==0){
						reqCoupPartieAdv();
					}else{
						messageFinPartie();
						etatPartie++;
					}
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
		joueur1=&sock_transJ1;
		joueur2=&sock_transJ2;
	}else if(oj==2){
		joueur1=&sock_transJ2;
		joueur2=&sock_transJ1;
	}else{
		perror("Erreur sur la définition de lordre de partie");
		printf("(ordreJoueur = %d)\n", oj);
		closeSock();
		return 9;
	}
	return 0;
}

int repDemPartie(){
	err=send(*joueur1, &parRepJ1, sizeof(TPartieRep), 0);
	if (err != sizeof(TPartieRep)){
		perror("Serveur : Erreur sur le renvoi de la réponse de la requète de partie (J1)");
		closeSock();
		return 10;
	}
	err=send(*joueur2, &parRepJ2, sizeof(TPartieRep), 0);
	if (err != sizeof(TPartieRep)){
		perror("Serveur : Erreur sur le renvoi de la réponse de la requète de partie (J2)");
		closeSock();
		return 11 ;
	}
	printf("Joueur 1 : %s, Joueur 2 : %s\n", &nomJ1, &nomJ2);  
	return 0;
}

int repCoupPartie(){
	if(nbPartie==0){
		err=send(*joueur1, &parCoupRep, sizeof(TCoupRep),0);
		if (err != sizeof(TCoupRep)){
			perror("Serveur : Erreur sur le renvoi de la réponse de la requète de coup (joueur 1)");
			closeSock();
			return 13;
		}
		err=send(*joueur2, &parCoupRep, sizeof(TCoupRep),0);
		if (err != sizeof(TCoupRep)){
			perror("Serveur : Erreur sur le renvoi de la réponse de la requète de coup (joueur 2)");
			closeSock();
			return 13;
		}
		printf("Serveur : envoi réponse coup. Code erreur : %d, validité du coup : %d, propriété du coup : %d.\n", parCoupRep.err, parCoupRep.validCoup, parCoupRep.propCoup);
	}else{
		err=send(*joueur2, &parCoupRep, sizeof(TCoupRep),0);
		if (err != sizeof(TCoupRep)){
			perror("Serveur : Erreur sur le renvoi de la réponse de la requète de coup (joueur 1)");
			closeSock();
			return 13;
		}
		err=send(*joueur1, &parCoupRep, sizeof(TCoupRep),0);
		if (err != sizeof(TCoupRep)){
			perror("Serveur : Erreur sur le renvoi de la réponse de la requète de coup (joueur 2)");
			closeSock();
			return 13;
		}
		printf("Serveur : envoi réponse coup. Code erreur : %d, validité du coup : %d, propriété du coup : %d.\n", parCoupRep.err, parCoupRep.validCoup, parCoupRep.propCoup);
	}
	return 0;
}

int reqCoupPartieAdv(){
	if(nbPartie==0){
		err=send(*joueur2, &parCoupReq, sizeof(TCoupReq),0);
		if (err != sizeof(TCoupReq)){
			perror("Serveur : Erreur sur le renvoi de la requète de coup à l'adversaire");
			closeSock();
			return 13;
		}
		printf("Serveur : envoi coup joué à l'adversaire.\n");
	}else{
		err=send(*joueur1, &parCoupReq, sizeof(TCoupReq),0);
		if (err != sizeof(TCoupReq)){
			perror("Serveur : Erreur sur le renvoi de la requète de coup à l'adversaire");
			closeSock();
			return 13;
		}
		printf("Serveur : envoi coup joué à l'adversaire.\n");
	}
	return 0;
}

void messageFinPartie(){
	if(parCoupReq.coul==0){
		switch(parCoupRep.propCoup){
			case 1 :
			 	printf("Fin partie. Gagnant : %s - Perdant : %s.\n", &nomJ1, &nomJ2);
			 	pgJ1++;
				ppJ2++;
			 	break; 
			case 2 :
				printf("Fin partie. Egalité entre %s et %s.\n", &nomJ1, &nomJ2);
				pnJ1++;
				pnJ2++;
				break; 
			case 3 :
			 	printf("Fin partie. Gagnant : %s - Perdant : %s.\n", &nomJ2, &nomJ1);
			 	ppJ1++;
				pgJ2++;
			 	break; 
			default :
				perror("Erreur sur la propriété du coup");
				break;
		}
	}else{
		switch(parCoupRep.propCoup){
			case 1 :
			 	printf("Fin partie. Gagnant : %s - Perdant : %s.\n", &nomJ2, &nomJ1);
			 	ppJ1++;
				pgJ2++;
			 	break; 
			case 2 :
				printf("Fin partie. Egalité entre %s et %s.\n", &nomJ2, &nomJ1);
				pnJ1++;
				pnJ2++;
				break; 
			case 3 :
			 	printf("Fin partie. Gagnant : %s - Perdant : %s.\n", &nomJ1, &nomJ2);
			 	pgJ1++;
				ppJ2++;
			 	break; 
			default :
				perror("Erreur sur la propriété du coup");
				break;
		}
	}
}

void messageFinPartieTimeout(){
	if(parCoupReq.coul==1){
		switch(parCoupRep.propCoup){
			case 1 :
			 	printf("Fin partie. Gagnant : %s - Perdant : %s.\n", &nomJ1, &nomJ2);
			 	pgJ1++;
				ppJ2++;
			 	break; 
			case 2 :
				printf("Fin partie. Egalité entre %s et %s.\n", &nomJ1, &nomJ2);
				pnJ1++;
				pnJ2++;
				break; 
			case 3 :
			 	printf("Fin partie. Gagnant : %s - Perdant : %s.\n", &nomJ2, &nomJ1);
			 	ppJ1++;
				pgJ2++;
			 	break; 
			default :
				perror("Erreur sur la propriété du coup");
				break;
		}
	}else{
		switch(parCoupRep.propCoup){
			case 1 :
			 	printf("Fin partie. Gagnant : %s - Perdant : %s.\n", &nomJ2, &nomJ1);
			 	ppJ1++;
				pgJ2++;
			 	break; 
			case 2 :
				printf("Fin partie. Egalité entre %s et %s.\n", &nomJ2, &nomJ1);
				pnJ1++;
				pnJ2++;
				break; 
			case 3 :
			 	printf("Fin partie. Gagnant : %s - Perdant : %s.\n", &nomJ1, &nomJ2);
			 	pgJ1++;
				ppJ2++;
			 	break; 
			default :
				perror("Erreur sur la propriété du coup");
				break;
		}
	}
}


void afficherScore(){
	printf("Score :\nJoueur 1 %s - Partie gagnés : %d, partie perdus : %d; partie nulle : %d\nJoueur 2 %s - Partie gagnés : %d, partie perdus : %d; partie nulle : %d\n", &nomJ1, pgJ1, ppJ1, pnJ1, &nomJ2, pgJ2, ppJ2, pnJ2);
}

void closeSock(){
	shutdown(sock_transJ1, SHUT_RDWR); close(sock_transJ1);
	shutdown(sock_transJ2, SHUT_RDWR); close(sock_transJ2);
	close(sock_cont);
}