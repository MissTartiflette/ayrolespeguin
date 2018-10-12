#include "state.h"

using namespace state;
using namespace std;

Guerrier::Guerrier(){
	nomArme="Epee";
    codeArme=4;
	champMove=3;
	champAttack=1;
	statistiques.setPV(20);
	statistiques.setAttaque(15);
	statistiques.setDefense(10);
	statistiques.setEsquive(10);
	statistiques.setCritique(10);
}


