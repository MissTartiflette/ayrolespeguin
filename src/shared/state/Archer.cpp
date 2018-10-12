#include <vector>
#include "state.h"

using namespace state;
using namespace std;

Archer::Archer(){
	nomArme="Arc";
    codeArme=1;
	champMove=4;
	champAttack=2;
	statistiques.setPV(20);
	statistiques.setAttaque(15);
	statistiques.setDefense(8);
	statistiques.setEsquive(10);
	statistiques.setCritique(15);
}
