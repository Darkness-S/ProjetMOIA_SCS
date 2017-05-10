#ifndef _fonctionsServeur_h
#define _fonctionsServeur_h

#define TAIL_BUF 100
#define TIME_MAX 6000
#define TNOM 30

int DemPartie(int port);
int receptPartie();
void affectRep();
int repDemParte(int oj);
void closeSock();
//void JouerCoup(int sc, int stJ1, int stJ2, TCoupReq cRqJ1, TCoupReq cRqJ2, TCoupRep cRpJ1, TCoupRep cRpJ2, fd_set *rs);

#endif
