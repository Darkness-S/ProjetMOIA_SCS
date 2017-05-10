#ifndef _fonctionsServeur_h
#define _fonctionsServeur_h

#define TAIL_BUF 100
#define TIME_MAX 6
#define TNOM 30

int DemPartie(int port);
int receptPartie();
//int JouerCoup();
void affectRep();
int affecterJoueur(int oj);
int repDemParte();
void closeSock();

#endif
