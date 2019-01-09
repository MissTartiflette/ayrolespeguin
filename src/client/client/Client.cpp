#include "engine.h"
#include "render.h"
#include "state.h"
#include "client.h"
#include "ai.h"
#include <iostream>
#include <unistd.h>


using namespace state;
using namespace engine;
using namespace render;
using namespace std;
using namespace client;
using namespace ai;

Client::Client (){
	unsigned int longueur_map_cases = 25, largeur_map_cases = 25;
	std::string chemin_fichier_map_txt = "res/map1.txt";
	Correspondances tab_corres = Correspondances();

	moteur.getEtat().initGrille(chemin_fichier_map_txt, longueur_map_cases, largeur_map_cases, tab_corres);
	moteur.getEtat().initPersonnages(tab_corres);
	moteur.getEtat().initCurseur();

	armeeRouge=new HeuristicIA();
	armeeBleue= new HeuristicIA();
	armeeBleue->setCamp(true);

}
 
void Client::engineUpdated (){
	
}

void Client::engineUpdating (){
	moteur.update();
}

void Client::run (){

	
}
