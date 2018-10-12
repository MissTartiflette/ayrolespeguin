#include <vector>
#include "state.h"

using namespace state;
using namespace std;

Brigand::Brigand(){
	nomArme="Hache";
    codeArme=2;
	champMove=3;
	champAttack=1;
	statistiques.setPV(20);
	statistiques.setAttaque(15);
	statistiques.setDefense(10);
	statistiques.setEsquive(15);
	statistiques.setCritique(10);
}
