#include <vector>
#include "state.h"

using namespace state;
using namespace std;

Chevalier::Chevalier(){
	nomArme="Lance";
    codeArme=3;
	champMove=5;
	champAttack=1;
	statistiques.setPV(25);
	statistiques.setAttaque(12);
	statistiques.setDefense(15);
	statistiques.setEsquive(15);
	statistiques.setCritique(10);
}

