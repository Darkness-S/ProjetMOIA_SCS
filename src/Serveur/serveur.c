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

/* include pour close */
#include <unistd.h>

/* include pour sockets */
#include <sys/socket.h>
#include <netinet/in.h>
 
/* include fonctions TCP */
#include "../../include/fonctionsTCP.h"

/* include pour le protocole de communication */
#include "../../include/protocoleColonne.h"
#include "../../include/validation.h"

/* taille du buffer de reception */
#define TAIL_BUF 100
#define TIME_MAX 6000

int main(int argc, char** argv) {
  int nbJoueur=0,
	  sock_cont, 
      sock_trans,       /* descripteurs des sockets locales */
      err;	        /* code d'erreur */

  struct sockaddr_in nom_transmis;	/* adresse de la socket de */
					                     /* transmission */
  
  char            buffer[TAIL_BUF];	/* buffer de reception */
  
  TPartieReq    	parReqJ1, parReqJ2;
  TPartieRep 		parRepJ1, parRepJ2;
  socklen_t      size_addr_trans;	/* taille de l'adresse d'une socket */
  rand(time(NULL));
  
  
  /*
   * verification des arguments
   */
  if (argc != 2) {
    printf ("usage : serveur no_port\n");
    return 1;
  }
  
   
  /* 
   * creation de la socket, protocole TCP 
   */
  printf("(serveur) creation de la socket sur %d\n", atoi(argv[1]));
  sock_cont = socketServeur(atoi(argv[1]));
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
  sock_trans = accept(sock_cont, 
		      (struct sockaddr *)&nom_transmis, 
		      &size_addr_trans);
  if (sock_trans < 0) {
    perror("serveur :  erreur sur accept");
    close(sock_cont);
    return 3; 
  }
  
  /*
   * reception de la requête de début de partie.
   */
	while(nbJoueur <2){
		if(nbJoueur ==0){
			err = recv(sock_trans, &parReqJ1, sizeof(TPartieReq), 0);
			if (err < 0) {
				perror("serveur: erreur dans la reception");
				shutdown(sock_trans, SHUT_RDWR); close(sock_trans);
				close(sock_cont);
				return 4;
			}
			if (parReq1.idRequest!=0){
				parRepJ1.err=1;
				parRepJ1.coul=0;
				parRepJ1.nomAdvers[5]="NULL";
			}
			nbJoueur++;
		}else{
			err = recv(sock_trans, &parReqJ2, sizeof(TPartieReq), 0);
			if (err < 0 && parReq1.idRequest==0) {
				perror("serveur: erreur dans la reception");
				shutdown(sock_trans, SHUT_RDWR); close(sock_trans);
				close(sock_cont);
				return 4;
			} 
			if (parReq2.idRequest!=0){
				parRepJ2.err=1;
				parRepJ2.coul=0;
				parRepJ2.nomAdvers[5]="NULL";
			}	 
			nbJoueur++;
		}
	}
	parRepJ1.err=0;
	parRepJ1.coul=0;
	parRepJ1.nomAdvers[TNOM]=parReqJ2.nomJoueur[TNOM];
	parRepJ2.err=0;
	parRepJ2.coul=1;
	parRepJ2.nomAdvers[TNOM]=parReqJ2.nomJoueur[TNOM];
	
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
  shutdown(sock_trans, SHUT_RDWR);
  close(sock_trans);
  close(sock_cont);
  return 0;
}
