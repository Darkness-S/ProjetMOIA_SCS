#define MAX_CL 0
int nbCl = 0, int sCl[MAX_CL];
sockConx = ... + ioctl ; 
while(1){
	si(nbCl ne dépasse pas la limite){
		err = accept(sockConx, ...) 
		si err > 0 sCl(nbCl++] = err ; ioctl sur sCl[nbCl]; 
		si err < 0 { if (errno!=EWB) => erreur -> exit;}
	}
	while(i < nbCl){
		err=recv(sCl[i],...){
			if(err > 0) [traitOp,send,i++]
			else if(err==0){
				decalage; 
				nbCl--; 
			}
			else if(errno!=EWB)
				//Erreur
				i++; 
				else//
				i++; 
		}
	}
}

