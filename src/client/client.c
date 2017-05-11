/*
 **********************************************************
 *
 *  Programme : client.c
 *
 *  ecrit par : LP.
 *
 *  resume :    envoi d'une chaine de caracteres a un 
 *              programme serveur en mode connecte
 *
 *  date :      25 / 01 / 06
 *
 ***********************************************************
 */

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
#include "../../include/fonctionClient.h"

#define SIZE_NAME 30

int main(int argc, char **argv) {
 // char chaine[TAIL_BUF];
  char playerName[SIZE_NAME] = "Zarnakyn"; 
  /*
   * verification des arguments
   */
  if (argc < 3) {
    printf("usage : client ip no_port nom_machine ...\n");
    return 1;
  }

  if(argv[3]!=NULL){
	strcpy(playerName, argv[3]);  
  }
	/* Set player name */ 
	setPlayer(playerName); 

	/* Connect to the server */
    connectSocket(argv[1], atoi(argv[2])); 
	
	gameQuery(); 

	gameGo(0); 
	gameGo(1); 
	
	exitAll(); 

	return 0;
}
 

