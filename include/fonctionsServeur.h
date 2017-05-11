#ifndef _FONCTIONSSERVEUR_H
#define _FONCTIONSSERVEUR_H
#include "./protocolColonne.h"
int demPartie(int port);
int receptPartie();
int jouerPartie();
int jouerCoupPar();
void affectRep();
int affecterJoueur(int oj);
int repDemParte();
int repCoupPartie();
int reqCoupPartieAdv();
void messageFinPartie();
void closeSock();

#endif
