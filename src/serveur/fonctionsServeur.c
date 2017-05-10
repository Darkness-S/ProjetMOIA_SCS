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
#include "../../include/protocolColonne.h"
#include "../../include/validation.h"

int	nbJoueur=0,
	etatPartie=0,
	nbPartie=0,
	ordreJoueur=0,
	sock_cont, 
	sock_transJ1,
	sock_transJ2,	/* descripteurs des sockets locales */
	*j1, *j2, *jTemp,
	err;	/* code d'erreur */

struct sockaddr_in nom_transmis;	/* adresse de la socket de transmission */
static fd_set readSet;
char	buffer[TAIL_BUF],
		nomJ1[TNOM],
		nomJ2[TNOM];	/* buffer de reception */


TPartieReq	parReqJ1, parReqJ2, parReqTemp;
TPartieRep	parRepJ1, parRepJ2, parRepTemp;
TCoupReq	parCoupReqJ1, parCoupReqJ2;
TCoupRep	parCoupRepJ1, parCoupRepJ2;
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
 	nbJoueur = sizeof(&readSet)*2; 	
 	receptPartie();
 	affecterJoueur(ordreJoueur);
 	affectRep();
	repDemPartie(ordreJoueur);
	closeSock();
	return 0;
}

int receptPartie(){
	err = select(nbJoueur, &readSet, NULL, NULL, NULL);
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
}

/*int JouerCoup(){
	struct timeval timeout;
	timeout.tv_sec = TIME_MAX;
	timeout.tv_usec = 0;
	while (etatPartie<1){
		err=select(nbJoueur, &readSet, NULL, NULL, &timeout);
		if (err < 0) {
			perror("serveur: erreur dans la selection du socket (timeout)");
			parCoupRepJ1.err=2;
			parCoupRepJ1.validCoup=1;
			parCoupRepJ1.propCoup=3;
		}else{
			err = recv(*j1, &parCoupReqJ1, sizeof(TCoupReq), 0);
			if (err < 0) {
				perror("serveur: erreur dans la reception (J1)");
				closeSock();
				return 4;
			}
		if (parCoupReqJ1.idRequest!=1){
			parCoupRepJ1.err=3;
			parCoupRepJ1.validCoup=2;
			parCoupRepJ1.propCoup=3;
			//TODO envoi aux deux joueurs et fin partie
		}else{
			if(validationCoup(1, parCoupReqJ1, &parCoupRepJ1.propCoup)==true){
				parCoupRepJ1.err=0;
				parCoupRepJ1.validCoup=0;
				//renvoi validité coup aux deux joueurs 
				if(parCoupRepJ1.propCoup==0){
					//TODO renvoi coup à joueur adverse
				}else{
					switch(parCoupRepJ1.propCoup){
						 case 1 :
						 	printf("Fin partie. Gagnant : %s - Perdant : %s.\n", &nomJ1, &nomJ2);
						 	break 
						 case 2 :
						 	printf("Fin partie. Egalité entre %s et %s.\n", &nomJ1, &nomJ2);
						 	break 
						 case 3 :
						 	printf("Fin partie. Gagnant : %s - Perdant : %s.\n", &nomJ2, &nomJ1);
						 	break 
					}
					//TODO renvoi resultat
				}
			}	
		}
		nbJoueur++;
	}
}*/


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
}

void closeSock(){
	shutdown(sock_transJ1, SHUT_RDWR); close(sock_transJ1);
	shutdown(sock_transJ2, SHUT_RDWR); close(sock_transJ2);
	close(sock_cont);
}
