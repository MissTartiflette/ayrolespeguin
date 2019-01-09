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
		}
		
		/*	record : Enregistrement d'une partie jouée par deux IA*/
		/*
		else if(strcmp(argv[1], "record") == 0){
			
			unsigned int longueur_map_cases = 25, largeur_map_cases = 25;
			std::string chemin_fichier_map_txt = "res/map1.txt";
			std::string fichier_commandes = "res/replay.txt";
			
			// Creation des tables de correspondances et du moteur
			Correspondances tab_corres = Correspondances();
			Moteur moteur;
			
			if(	moteur.getEtat().initGrille(chemin_fichier_map_txt, longueur_map_cases, largeur_map_cases, tab_corres)){
				// Initialisations de démarrage et création des IA
				moteur.getEtat().initPersonnages(tab_corres);
				moteur.getEtat().initCurseur();				
				HeuristicIA armeeRouge;
				HeuristicIA armeeBleue;
				armeeBleue.setCamp(true);
				
				
				cout << "\t\t--- Record ---" << endl;				
				cout << "On simule trois tours de jeu et on conserve les commandes effectuées dans le fichier ";
				cout << fichier_commandes << endl;
				
				bool demarrage = true ;
				
				// Ouverture du fichier en ecriture en effacant son contenu à l'ouverture
				std::ofstream fichier_ecriture(fichier_commandes, ios::out|ios::trunc);
				if(fichier_ecriture){
				
					Json::Value newTab;
					
					// On simule 3 tours de jeu
					while (moteur.getEtat().getTour() != 4){
						
						// L'amée bleue joue les tours impairs
						if(moteur.getEtat().getTour()%2 != 0){
							armeeBleue.run(moteur);
						}
						// L'amée rouge joue les tours pairs
						else{
							armeeRouge.run(moteur);
						}
						
						if(!moteur.getEtat().getFin() && moteur.verificationFinDeTour()){
							moteur.verificationDebutDeTour();*/
							/*
							StateEvent majDisponibilite(ALLCHANGED);
							moteur.getEtat().notifyObservers(majDisponibilite, moteur.getEtat(), window);*/
						/*}
					}
					
					
					
					// Deplacement chevalier bleu
					Position destination1(3,22);
					Deplacement deplacement1(*moteur.getEtat().getPersonnages()[2], destination1,true);
										
					newTab[0]["id"] = deplacement1.getCommandeID();
					newTab[0]["joueur"] = deplacement1.joueur;
					newTab[0]["cible"] = 2;
					newTab[0]["xDestination"] = destination1.getX();
					newTab[0]["yDestination"] = destination1.getY();
					
					// Attaque chevalier bleu contre archer rouge
					Attaque attaque1(*moteur.getEtat().getPersonnages()[2], *moteur.getEtat().getPersonnages()[7],true);
					newTab[1]["id"] = attaque1.getCommandeID();
					newTab[1]["joueur"] = attaque1.joueur;
					newTab[1]["attaquant"] = 2;
					newTab[1]["cible"] = 7;
					
					// Fin du tour de l'archer bleu
					FinActions finactions1(*moteur.getEtat().getPersonnages()[0],true);
					newTab[2]["id"] = finactions1.getCommandeID();
					newTab[2]["joueur"] = finactions1.joueur;
					newTab[2]["cible"] = 0;
										
					root["tabCmd"] = newTab;
					
					cout << root << endl;
					
					
					
					
					
					// Ecriture dans le fichier du tableau de commandes de cette partie
					fichier_ecriture << newTab;
					
					// Fermeture du fichier
					fichier_ecriture.close();
				}
				else{
					cerr << "Impossible d'ouvrir le fichier des commandes enregistrées pour l'ecriture" << endl;
				}	
			}
		}*/
	}
	
    return 0;
}
