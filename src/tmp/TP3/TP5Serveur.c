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

/* include structure */
#include "CalculFonction.h"


int main(int argc, char** argv) {
/*typedef enum {
	OK,
	OP_INCORRECT,
	OP_OVERFLOW,
	DIV_ZERO,
	OP_INCONNU
}TCodeRep;    

typedef enum {
	SUM,
	SUB,
	MULT,
	DIV
}TCodeOp;

typedef struct {
    TCodeOp oprt;
    int opd1, opd2;
}TOperation;

typedef struct {
    TCodeRep codeErr;
    int res;
}TOpRes;*/
	int sock,
		sock_cont, 
		sock_trans,       /* descripteurs des sockets locales */
		err;	        /* code d'erreur */
	
	struct sockaddr_in nom_transmis;	/* adresse de la socket de */
							                 /* transmission */
	  	  
	socklen_t      size_addr_trans;	/* taille de l'adresse d'une socket */
	  
		/* 
	   * creation de la socket, protocole TCP 
	   */
	   /*
	   * verification des arguments
	   */
	if (argc != 2) {
		printf ("usage : serveur no_port\n");
		return 1;
	}
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
	TOperation op;
	TOpRes rep;
	err = recv(sock_trans, &op, sizeof(TOperation), 0);
	if (err < 0) {
	    perror("serveur: erreur dans la reception");
	    shutdown(sock_trans, SHUT_RDWR); close(sock_trans);
	    close(sock_cont);
	    return 4;
	}
  
	switch (op.oprt){
	  	case 0 :
	  		rep.res = op.opd1 + op.opd2;
	  		rep.codeErr = 0;
	  		break;
		case 1 :
	  		rep.res = op.opd1 - op.opd2;
	  		rep.codeErr = 0;
	  		break;
	  	case 2 :
	  		rep.res = op.opd1 * op.opd2;
	  		rep.codeErr = 0;
	  		break;
	  	case 3 :
	  		if (op.opd2==0){
	  			perror("Erreur : division par 0\n");
	  			rep.codeErr=3;
	  		}else{	
	  			rep.res = op.opd1 / op.opd2;
	  			rep.codeErr = 0;
	  		}
	  		break;
	  	default :
	  		perror("Erreur : Opérande inconnu\n");
	  		rep.codeErr = 4;
	  		break;
	}
	if(rep.codeErr==0){
		printf("(serveur) le résultat est: %d et le code de réception est %d\n", rep.res,rep.codeErr);
		
	}else{
		printf("(serveur) erreur, code erreur : %d\n", rep.codeErr);
	}
	
	err = send(sock_trans, &rep, sizeof(TOpRes), 0);
	if (err != sizeof(TOpRes)) {
		perror("(client) erreur sur le send");
		shutdown(sock, SHUT_RDWR); close(sock_trans);
		return 3;
	}
	  
	/* 
	 * arret de la connexion et fermeture
	 */
	shutdown(sock_trans, SHUT_RDWR);
	close(sock_trans);
	close(sock_cont);
	return 0;
}
