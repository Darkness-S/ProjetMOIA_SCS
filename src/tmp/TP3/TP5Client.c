/* inclusions générales */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* inclusions pour les sockets */
#include <sys/socket.h>
#include <netinet/in.h>
/* inclusions pour close */
#include <unistd.h>

/* inclusion bibliothèque TCP */
#include "fonctionsTCP.h"

/* include structure */
#include "CalculFonction.h"

#define TAIL_BUF 100
int main(int argc, char **argv) {
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
		sockserv,
		sock_trans,               /* descripteur de la socket locale */
		err,                /* code d'erreur */
		type;
	char typeOp;
	struct sockaddr_in nom_transmis;	/* adresse de la socket de */
							                 /* transmission */
	  	  
	socklen_t      size_addr_trans;	/* taille de l'adresse d'une socket */
	/*
	 * verification des arguments
	 */
	if (argc != 3) {
		printf("usage : client nom_machine no_port\n");
		return 1;
	}
	  
	/* 
	 * creation d'une socket, domaine AF_INET, protocole TCP 
	 */
	 sockserv = socketServeur(atoi(argv[2]));
	printf("(client) connect to %s, %d\n", argv[1], atoi(argv[2]));
	sock = socketClient(argv[1], atoi(argv[2]));
	if (sock < 0) { 
		printf("(client) erreur socketClient\n");
		return 2;
	}
	  
	/* 
	 * saisie de la chaine 
	 */
	TOperation op;
	printf("(client) Entrer le numéro correspondant au type d'opération (1 pour addition, 2 pour soustraction, 3 pour multiplication, 4 pour division) :\n ");
	scanf("%d", &type);
	switch(type){
		case 1 : 
			op.oprt=0;
			typeOp='+';
			break;
		case 2 :
			op.oprt=1;
			typeOp='-';
			break;
		case 3 :
			op.oprt=2;
			typeOp='*';
			break;
		case 4 :
			op.oprt=3;
			typeOp='/';
			break;
		default : 
			perror("Erreur nombre entré\n");
			shutdown(sock, SHUT_RDWR); close(sock);
			return 4;
			break;
	}
		
	printf("(client) Entrer la première opérande :\n ");
	scanf("%d", &op.opd1);
	printf("(client) Entrer la seconde opérande :\n ");
	scanf("%d", &op.opd2);
	   
	/*
	 * envoi de la chaine
	 */
	 printf("(client) Envoi de l'opération %d %c %d :\n ", op.opd1, typeOp, op.opd2);
	err = send(sock, &op, sizeof(TOperation), 0);
	if (err != sizeof(TOperation)) {
		perror("(client) erreur sur le send");
		shutdown(sock, SHUT_RDWR); close(sock);
		return 3;
	}
	printf("(client) envoi realise\n");
	

	/*
	 *  initialisation de la taille de l'adresse d'une socket
	 */
	size_addr_trans = sizeof(struct sockaddr_in);

	/*
	 * attente de connexion
	 */
	sock_trans = accept(sockserv, 
				  (struct sockaddr *)&nom_transmis, 
				  &size_addr_trans);
	if (sock_trans < 0) {
	perror("serveur :  erreur sur accept");
	close(sock);
	return 3; 
	}

	/*
	 * reception et affichage du message en provenance du client
	 */
	TOpRes rep;
	err = recv(sock_trans, &rep, sizeof(TOpRes), 0);
	if (err < 0) {
	    perror("serveur: erreur dans la reception");
	    shutdown(sockserv, SHUT_RDWR); close(sockserv);
	    close(sock);
	    return 4;
	}
	if(rep.codeErr==0){
		printf("(serveur) le résultat est: %d et le code de réception est %d\n", rep.res,rep.codeErr);
		
	}else{
		printf("(serveur) erreur, code erreur : %d\n", rep.codeErr);
	}
	/* 
	 * fermeture de la connexion et de la socket 
	 */
	shutdown(sock, SHUT_RDWR);
	shutdown(sock_trans, SHUT_RDWR);
	close(sockserv);
	close(sock_trans);
	close(sock);
	return 0;
}
 

