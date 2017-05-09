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
 
/* include fonctions TCP */
#include "../../include/fonctionsServeur.h"

int main(int argc, char** argv) {

	  /*
   		* verification des arguments
 	  */
 	 if (argc != 2) {
  		printf ("usage : serveur no_port\n");
   		return 1;
 	}
	demPartie(atoi(argv[1]));
}
