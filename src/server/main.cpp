#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <map>
#include <memory>
#include <unistd.h>

#include "state.h"
#include "render.h"
#include "engine.h"
#include "ai.h"


using namespace std;
using namespace state;
using namespace render;
using namespace engine;
using namespace ai;

int main(int argc,char* argv[]){

	if (argc>1){
		/*	hello : Affichage simple */
		if(strcmp(argv[1],"hello")==0){
			cout<<"Bonjour tout le monde"<<endl;
			Json::Value root;
			root["tabCmd"];
			
			/*
			cout << "Taille root : " << root["tabCmd"].size() << endl;
			root["tabCmd"][0]["id"] =  200;
			cout << root << endl;
			*/
		}
		
		/*	record : Enregistrement d'une partie jouée par deux IA*/
		else if(strcmp(argv[1], "record") == 0){
			
			unsigned int longueur_map_cases = 25, largeur_map_cases = 25;
			std::string chemin_fichier_map_txt = "res/map1.txt";
			std::string fichier_commandes = "res/replay.txt";
			
			// Creation des tables de correspondances et du moteur
			Correspondances tab_corres = Correspondances();
			Moteur moteur;
			moteur.setEnableRecord(true);
			
			if(	moteur.getEtat().initGrille(chemin_fichier_map_txt, longueur_map_cases, largeur_map_cases, tab_corres)){
				// Initialisations de démarrage et création des IA
				moteur.getEtat().initPersonnages(tab_corres);
				moteur.getEtat().initCurseur();				
				HeuristicIA armeeRouge;
				HeuristicIA armeeBleue;
				armeeBleue.setCamp(true);
				
				cout << "\t\t--- Record ---" << endl;				
				cout << "On enregistre trois tours de jeu dans le fichier " << fichier_commandes << endl;
				cout << "Deux IA heuristiques jouent l'une contre l'autre" << endl;
				sleep(2);
				cout << "--> Début de l'enregistrement <--" << endl;
				
				// On simule 3 tours
				while (moteur.getEtat().getTour() != 4){
					// L'armée bleue joue les tours impairs, la rouge les tours pairs
					if(moteur.getEtat().getTour()%2 != 0){
						armeeBleue.run(moteur);
					}
					else{
						armeeRouge.run(moteur);
					}
					
					// Verifications de fin de tours pour changer le tour
					if(!moteur.getEtat().getFin() && moteur.verificationFinDeTour()){
							moteur.verificationDebutDeTour();
					}
				}
				
				cout << "--> Fin de l'enregistrement <--" << endl;
				
				
				cout << "--> Debut de l'enregistrement dans le fichier <--" << endl;
				// Ouverture du fichier en ecriture en effacant son contenu à l'ouverture
				std::ofstream fichier_ecriture(fichier_commandes, ios::out|ios::trunc);
				if(fichier_ecriture){
									
					Json::Value root = moteur.getRecord();
					cout << root << endl;
					
					// Ecriture dans le fichier du tableau de commandes de cette partie
					fichier_ecriture << root;
					
					// Fermeture du fichier
					fichier_ecriture.close();
					
					cout << "--> Fin de l'enregistrement dans le fichier <--" << endl;
				}
				else{
					cerr << "Impossible d'ouvrir le fichier des commandes enregistrées pour l'ecriture" << endl;
				}
			}
		}
	}
	
    return 0;
}
