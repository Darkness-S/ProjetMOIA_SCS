/*
**************************************************************************
 *
 *  Programme : fonctionsSocket.h
 *
 *  ecrit par : LP.
 *
 *  resume :    entete des fonctions d'initialisation des sockets en mode 
 *              connecte
 *
 *  date :      25 / 01 / 06
 *
 ************************************************************************
 */

#ifndef FONCTIONS_TCP_H
#define FONCTIONS_TCP_H

/* include generaux */
#include <sys/types.h>

/*
 **********************************************************
 *
 *  fonction  : socketServeur (numero de port)
 *
 *  resume :    creer la socket du serveur et la retourne
 *
 ***********************************************************
 */
int socketServeur(ushort port);

/*
 **********************************************************
 *
 *  fonction : socketClient( nom de machine serveur, numero de port )
 *
 *  resume :   fonction de connexion d'une socket au serveur
 *              
 ***********************************************************
 */

int socketClient(char *nomMachine, ushort port);

#endif
