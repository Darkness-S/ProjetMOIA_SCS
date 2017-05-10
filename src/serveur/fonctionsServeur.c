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
#include <time.h>
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
	//etatPartie=0,
	nbPartie=0,
	ordreJoueur=0,
	sock_cont, 
	sock_transJ1,
	sock_transJ2,	/* descripteurs des sockets locales */
	err;	/* code d'erreur */

struct sockaddr_in nom_transmis;	/* adresse de la socket de transmission */
static fd_set readSet;
char	buffer[TAIL_BUF];	/* buffer de reception */

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
 	affectRep();
	//printf("%d %d %s %d %d %s\n", parRepJ1.err, parRepJ1.coul, &parRepJ1.nomAdvers, parRepJ2.err, parRepJ2.coul, &parRepJ2.nomAdvers);
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
			parRepJ1.err=1;
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
			parRepJ2.err=1;
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
			parRepJ1.err=1;
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
			parRepJ2.err=1;
			parRepJ2.coul=0;
			parRepJ2.nomAdvers[0]='\0';
		}	
	}
}

void affectRep(){
 	parRepJ1.err=0;
	parRepJ1.coul=0;
	strcpy(parRepJ1.nomAdvers, parReqJ2.nomJoueur);
	parRepJ2.err=0;
	parRepJ2.coul=1;
	strcpy(parRepJ2.nomAdvers, parReqJ1.nomJoueur);
}

int repDemPartie(int oj){
	if(oj==1){
		err=send(sock_transJ1, &parRepJ1, sizeof(TPartieRep), 0);
		if (err != sizeof(TPartieRep)){
			perror("Serveur : Erreur sur le renvoi de la réponse de la requète de partie (J1 : sock_transJ1)");
			closeSock();
			return 9;
		}
		err=send(sock_transJ2, &parRepJ2, sizeof(TPartieRep), 0);
		if (err != sizeof(TPartieRep)){
			perror("Serveur : Erreur sur le renvoi de la réponse de la requète de partie (J2 : sock_transJ2)");
			closeSock();
			return 10;
		}
	}else if(oj==2){
		err=send(sock_transJ2, &parRepJ1, sizeof(TPartieRep), 0);
		if (err != sizeof(TPartieRep)){
			perror("Serveur : Erreur sur le renvoi de la réponse de la requète de partie (J1 : sock_transJ2)");
			closeSock();
			return 11;
		}
		err=send(sock_transJ1, &parRepJ1, sizeof(TPartieRep), 0);
		if (err != sizeof(TPartieRep)){
			perror("Serveur : Erreur sur le renvoi de la réponse de la requète de partie (J2 : sock_transJ1)");
			closeSock();
			return 12;
		}
	}else{
		perror("Erreur sur la définition de lordre de partie");
		printf("(ordreJoueur = %d)\n", oj);
		closeSock();
		return 13;
	}
	printf("Joueur 1 : %s, Joueur 2 : %s\n", &parReqJ1.nomJoueur, &parReqJ2.nomJoueur);  
}

void closeSock(){
	shutdown(sock_transJ1, SHUT_RDWR); close(sock_transJ1);
	shutdown(sock_transJ2, SHUT_RDWR); close(sock_transJ2);
	close(sock_cont);
}
