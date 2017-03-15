/*
********************************************************************
 *
 *  Programme : serveur.c
 *
 *  ecrit par : LP.
 *
 *  resume :    recoit une chaine de caracteres du programme client
 *                en mode connecte
 *
 *  date :      25 / 01 / 06
 *
 *******************************************************************
 */

/* include generaux */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* include pour close */
#include <unistd.h>

/* include pour sockets */
#include <sys/socket.h>
#include <netinet/in.h>
 
/* include fonctions TCP */
#include "fonctionsTCP.h"


/* taille du buffer de reception */
#define TAIL_BUF 100

int main(int argc, char** argv) {
  int sock_cont, 
      sock_trans,       /* descripteurs des sockets locales */
      err;	        /* code d'erreur */

  struct sockaddr_in nom_transmis;	/* adresse de la socket de */
					                     /* transmission */
  
  char            buffer[TAIL_BUF];	/* buffer de reception */
  
  socklen_t      size_addr_trans;	/* taille de l'adresse d'une socket */
  
  
  
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
   * reception et affichage du message en provenance du client
   */
  err = recv(sock_trans, buffer, TAIL_BUF, 0);
  if (err < 0) {
    perror("serveur: erreur dans la reception");
    shutdown(sock_trans, SHUT_RDWR); close(sock_trans);
    close(sock_cont);
    return 4;
  }
  printf("(serveur) le message recu: %s\n", buffer);
  
  /* 
   * arret de la connexion et fermeture
   */
  shutdown(sock_trans, SHUT_RDWR);
  close(sock_trans);
  close(sock_cont);
  return 0;
}
