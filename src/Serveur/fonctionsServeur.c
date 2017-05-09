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
/* taille du buffer de reception */


 int nbJoueur=0,
 	  //etatPartie=0,
  	  nbPartie=0,
	  sock_cont, 
      sock_transJ1,
      sock_transJ2,       /* descripteurs des sockets locales */
      err;	        /* code d'erreur */

  struct sockaddr_in nom_transmis;	/* adresse de la socket de */
					                     /* transmission */
  static fd_set readSet;
  char            buffer[TAIL_BUF];	/* buffer de reception */
  
  TPartieReq    	parReqJ1, parReqJ2, parReqTemp;
  TPartieRep 		parRepJ1, parRepJ2, parRepTemp;
  TCoupReq			parCoupReqJ1, parCoupReqJ2;
  TCoupRep			parCoupRepJ1, parCoupRepJ2;
  socklen_t      size_addr_trans;	/* taille de l'adresse d'une socket */

int demPartie(int port) {
  /* 
   * creation de la socket, protocole TCP 
   */
  printf("(serveur) creation de la socket sur %d\n", port);
  sock_cont = socketServeur(port);
  if (sock_cont < 0) {
    perror("serveur : erreur socketServeur");
    return 2;
  }

  /*
   *  initialisation de la taille de l'adresse d'une socket
   */
  size_addr_trans = sizeof(struct sockaddr_in);

  /*
   * attente de connexion
   */
  sock_transJ1 = accept(sock_cont, 
		      (struct sockaddr *)&nom_transmis, 
		      &size_addr_trans);
  if (sock_transJ1 < 0) {
    perror("serveur :  erreur sur accept");
    close(sock_cont);
    return 3; 
  }
  printf("Connexion J1\n");
  
  sock_transJ2 = accept(sock_cont, 
		      (struct sockaddr *)&nom_transmis, 
		      &size_addr_trans);
  if (sock_transJ2 < 0) {
    perror("serveur :  erreur sur accept");
    close(sock_cont);
    return 3; 
  }
  printf("Connexion J2\n");
  FD_ZERO(&readSet);
  FD_SET(sock_transJ1, &readSet);
  FD_SET(sock_transJ2, &readSet);
 	nbJoueur = sizeof(&readSet)*2;
 	err = select(nbJoueur, &readSet, NULL, NULL, NULL);
 	if (err < 0) {
		perror("serveur: erreur dans la selection du socket");
		shutdown(sock_transJ1, SHUT_RDWR); close(sock_transJ1);
		shutdown(sock_transJ2, SHUT_RDWR); close(sock_transJ2);
		close(sock_cont);
		return 4;
	}else if(FD_ISSET(sock_transJ1, &readSet)!=0){
		err = recv(sock_transJ1, &parReqJ1, sizeof(TPartieReq), 0);
		if (err < 0) {
			perror("serveur: erreur dans la reception");
			shutdown(sock_transJ1, SHUT_RDWR); close(sock_transJ1);
			shutdown(sock_transJ2, SHUT_RDWR); close(sock_transJ2);
			close(sock_cont);
			return 4;
		}
		if (parReqJ1.idRequest!=0){
			parRepJ1.err=1;
			parRepJ1.coul=0;
			parRepJ1.nomAdvers[0]='\0';
		}
		err = recv(sock_transJ2, &parReqJ2, sizeof(TPartieReq), 0);
		if (err < 0 && parReqJ1.idRequest==0) {
			perror("serveur: erreur dans la reception");
			shutdown(sock_transJ1, SHUT_RDWR); close(sock_transJ1);
			shutdown(sock_transJ2, SHUT_RDWR); close(sock_transJ2);
			close(sock_cont);
			return 4;
		} 
		if (parReqJ2.idRequest!=0){
			parRepJ2.err=1;
			parRepJ2.coul=0;
			parRepJ2.nomAdvers[0]='\0';
		}	 
	}else if(FD_ISSET(sock_transJ2, &readSet)!=0){
		err = recv(sock_transJ2, &parReqJ1, sizeof(TPartieReq), 0);
		if (err < 0) {
			perror("serveur: erreur dans la reception");
			shutdown(sock_transJ1, SHUT_RDWR); close(sock_transJ1);
			shutdown(sock_transJ2, SHUT_RDWR); close(sock_transJ2);
			close(sock_cont);
			return 4;
		}
		if (parReqJ1.idRequest!=0){
			parRepJ1.err=1;
			parRepJ1.coul=0;
			parRepJ1.nomAdvers[0]='\0';
		}
		err = recv(sock_transJ1, &parReqJ2, sizeof(TPartieReq), 0);
		if (err < 0 && parReqJ1.idRequest==0) {
			perror("serveur: erreur dans la reception");
			shutdown(sock_transJ1, SHUT_RDWR); close(sock_transJ1);
			shutdown(sock_transJ2, SHUT_RDWR); close(sock_transJ2);
			close(sock_cont);
			return 4;
		} 
		if (parReqJ2.idRequest!=0){
			parRepJ2.err=1;
			parRepJ2.coul=0;
			parRepJ2.nomAdvers[0]='\0';
		}	
	}
 	parRepJ1.err=0;
	parRepJ1.coul=0;
	strcpy(parRepJ1.nomAdvers, parReqJ2.nomJoueur);
	parRepJ2.err=0;
	parRepJ2.coul=1;
	strcpy(parRepJ2.nomAdvers, parReqJ1.nomJoueur);

	printf("%d %d %s %d %d %s\n", parRepJ1.err, parRepJ1.coul, &parRepJ1.nomAdvers, parRepJ2.err, parRepJ2.coul, &parRepJ2.nomAdvers);

 // demPartie(sock_cont, sock_transJ1, sock_transJ2, parReqJ1, parReqJ2, parRepJ1, parRepJ2, &readSet);
	
	//TODO Renvoi aux deux joueurs ainsi que le numéro de partie
	//TODO Demander au joueur 1 de commencer son coup 
/*	while (nbPartie<2){
		if(nbPartie==0){
			JouerCoup(socket_cont, sock_transJ1, sock_transJ2, parCoupReqJ1, parCoupReqJ2, parCoupRepJ1, parCoupRepJ2);
			nbPartie++;
		}else{
			JouerCoup(socket_cont, sock_transJ2, sock_transJ1, parCoupReqJ2, parCoupReqJ1, parCoupRepJ2, parCoupRepJ1);
			nbPartie++;
		}
	}
  err = recv(sock_trans, &parReq, sizeof(TPartieReq), 0);
  if (err < 0) {
    perror("serveur: erreur dans la reception");
    shutdown(sock_trans, SHUT_RDWR); close(sock_trans);
    close(sock_cont);
    return 4;
  }
	  
	switch (parReq.idRequest){
		case 0 : 
  /* 
   * arret de la connexion et fermeture
   */
  shutdown(sock_transJ1, SHUT_RDWR);
  shutdown(sock_transJ2, SHUT_RDWR);
  close(sock_transJ1);
  close(sock_transJ2);
  close(sock_cont);
  return 0;
}
