#include <iostream>
#include <string.h>

// Les lignes suivantes ne servent qu'à vérifier que la compilation avec SFML fonctionne
#include <SFML/Graphics.hpp>


void testSFML() {
    sf::Texture texture;
}

// Fin test SFML

#include "state.h"

using namespace std;
using namespace state;

int main(int argc,char* argv[]){
	if (argc>1){
		if(strcmp(argv[1],"hello")==0){
			cout<<"Bonjour tout le monde"<<endl;
		}
		else if(strcmp(argv[1],"state")==0){
			cout<<"Tests simples à écrire"<<endl;
		}
		else{
			exit(EXIT_FAILURE);
		}
	}
	else{
	// Tests sur classes
	{
	//
	int a;
	cout<< "type de a : "<<typeid(a).name()<<endl;
	//	
    Position testpos;
    testpos.setX(1);
    testpos.setY(2);

	Position testpos2;
	testpos2.setX(1);
	testpos2.setY(2);

	cout << "--- Test de la classe Position ---" << endl;
	cout << "x = " << testpos.getX() << " et y = "<< testpos.getY() << endl;
	cout << "Résultat du test equals avec 1 et 2 : " << testpos.equals(testpos2) << endl;

	testpos2.setX(2);
	cout << "Résultat du test equals avec 2 et 2 : " << testpos.equals(testpos2) << endl;

	TerrainNonPraticable testtnp = TerrainNonPraticable(MUR);    
    cout << "--- Test de la classe TerrainNonPraticable ---" << endl;
	cout  << "nom : " << testtnp.getNom() << endl;
	cout << "is Terrrain : " << testtnp.isTerrain() << endl;
	cout << "is Praticable : " << testtnp.isPraticable() << endl;

	TerrainPraticable testtp = TerrainPraticable(COLLINE);    
    cout << "--- Test de la classe TerrainPraticable ---" << endl;
	cout  << "nom : " << testtp.getNom() << endl;
	cout << "is Terrrain : " << testtp.isTerrain() << endl;
	cout << "is Praticable : " << testtp.isPraticable() << endl;
	cout << "Statistiques : " << endl;
	cout << "- PV 		: " << testtp.getStatistiques().getPV() << endl;
	cout << "- Attaque 	: " << testtp.getStatistiques().getAttaque() << endl;
	cout << "- Defense 	: " << testtp.getStatistiques().getDefense() << endl;
	cout << "- Esquive 	: " << testtp.getStatistiques().getEsquive() << endl;
	cout << "- Critique 	: " << testtp.getStatistiques().getCritique() << endl;
	cout << "Position :" << endl;
	cout << "- x = " << testtp.getPosition().getX() << endl;
	cout << "- y = " << testtp.getPosition().getY() << endl;
	
	std::vector<Position> listpos;
	cout << "--- Test Liste de positions ---" << endl;
	size_t i = 0;
	Position posdeliste;
	while (i!=2){
		posdeliste.setX(i);
		posdeliste.setY(i);		
		listpos.push_back(posdeliste);
		i++;
	}
	cout << "Taille de la liste : " << listpos.size() << endl;
	
	for(i = 0; i<listpos.size(); i++){
		cout << "x = " << listpos[i].getX() << " et y = "<< listpos[i].getY() << endl;
	}
	
	}
	}
    return 0;
}
