void DemPartie(int sc, int stJ1, int stJ2, TPartieReq pRqJ1, TPartieReq pRqJ2, TPartieRep pRpJ1, TPartieRep pRpJ2){
 err = recv(stJ1, &pRqJ1, sizeof(TPartieReq), 0);
	if (err < 0) {
		perror("serveur: erreur dans la reception");
		shutdown(stJ1, SHUT_RDWR); close(stJ1);
		shutdown(stJ2, SHUT_RDWR); close(stJ2);
		close(sc);
		return 4;
	}
	if (pRqJ1.idRequest!=0){
		pRpJ1.err=1;
		pRpJ1.coul=0;
		pRpJ1.nomAdvers[5]="NULL";
	}
	err = recv(stJ2, &pRqJ2, sizeof(TPartieReq), 0);
	if (err < 0 && pRqJ1.idRequest==0) {
		perror("serveur: erreur dans la reception");
		shutdown(stJ1, SHUT_RDWR); close(stJ1);
		shutdown(stJ2, SHUT_RDWR); close(stJ2);
		close(sc);
		return 4;
	} 
	if (pRqJ2.idRequest!=0){
		pRpJ2.err=1;
		pRpJ2.coul=0;
		pRpJ2.nomAdvers[5]="NULL";
	}	 
	pRpJ1.err=0;
	pRpJ1.coul=0;
	pRpJ1.nomAdvers[TNOM]=pRqJ2.nomJoueur[TNOM];
	pRpJ2.err=0;
	pRpJ2.coul=1;
	pRpJ2.nomAdvers[TNOM]=pRqJ2.nomJoueur[TNOM];
}	
