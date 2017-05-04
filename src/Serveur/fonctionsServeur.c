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

void JouerCoup(int sc, int stJ1, int stJ2, TCoupReq cRqJ1, TCoupReq cRqJ2, TCoupRep cRpJ1, TCoupRep cRpJ2){
	int etatPartie=0
	while (etatPartie<1){
		err = recv(stJ1, &cRqJ1, sizeof(TCoupReq), 0);
		if (err < 0) {
			perror("serveur: erreur dans la reception");
			shutdown(stJ1, SHUT_RDWR); close(stJ1);
			shutdown(stJ2, SHUT_RDWR); close(stJ2);
			close(sc);
			return 4;
		}
		if (cRqJ1.idRequest!=1){
			cRpJ1.err=3;
			cRpJ1.validCoup=2;
			cRpJ1.propCoup=3;
			//TODO envoi aux deux joueurs et fin partie
		}else{
			if(validationCoup(1, cRqJ1, &cRpJ1.propCoup)==true){
				cRpJ1.err=0;
				cRpJ1.validCoup=0;
				//renvoi validité coup aux deux joueurs 
				if(cRpJ1.propCoup==0){
					//TODO renvoi coup à joueur adverse
				}
			}	
		}
		nbJoueur++;
	}
}
