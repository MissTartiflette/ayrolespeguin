#include "HeuristicIA.h"
#include "engine.h"
#include "state.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

using namespace ai;
using namespace engine;
using namespace state;
using namespace std;


void HeuristicIA::run (engine::Moteur& moteur, sf::RenderWindow& window){

	// L'IA effectue ces actions uniquement si c'est son tour
	if(moteur.getJoueurActif()==false){
		for (size_t i = 0; i < moteur.getEtat().getPersonnages().size(); i++){
			int action=0;
			if (moteur.getEtat().getPersonnages()[i]-> getCamp() == camp){
				while (moteur.getEtat().getPersonnages()[i]->getStatut() != MORT && moteur.getEtat().getPersonnages()[i]->getStatut() != ATTENTE){		
					//int action=0;
					moteur.getEtat().getPersonnages()[i]->setStatut(SELECTIONNE);
					vector<Position> listeAttaques = moteur.getEtat().getPersonnages()[i]-> getLegalAttack(moteur.getEtat());
					if(action!=2){
						if (listeAttaques.size() !=0){
							action=1;
						}
						else{
							action=0;
						}
					}
					
					// 0 : Cas du deplacement
					if (action==0){
						if(moteur.getEtat().getPersonnages()[i]->getChampMove() != 0){
							int indiceVoisin=this->findIndiceVoisin(moteur, i);
							int choix=this->findchoixOptimalDep(moteur, indiceVoisin, i);
							cout << "choix : " << choix << endl;
							vector<Position> listePos=moteur.getEtat().getPersonnages()[i]->getLegalMove(moteur.getEtat());
							if (choix != -1){
								// Commande de deplacement
								Deplacement deplacement(*moteur.getEtat().getPersonnages()[i], listePos[choix], camp);
								cout << "coordonnées choix : " << listePos[choix].getX() <<" , "<< listePos[choix].getY() << endl;
								unique_ptr<Commande> ptr_deplacement (new Deplacement(deplacement));
								moteur.addCommande(0, move(ptr_deplacement));
								moteur.update(window);
								sleep(1);
							}
						}
						else{
							action=2;
						}
					}
					
					
					// 1 : Cas de l'attaque
					else if (action == 1){
					
							vector<int> listePV, listeIndice, listeScores;
							int bonus_attaque=-1;
							int score = 0;
							std::string nomArme_attaquant, nomArme_cible;
							
							for(size_t j=0; j<listeAttaques.size();j++){

								int indiceCiblePotentielle = moteur.getEtat().getGrille()[listeAttaques[j].getX()][listeAttaques[j].getY()]->isOccupe(moteur.getEtat());
								
								//On ajoute dans listePV les PVs de toutes les cibles potentielles
								if (indiceCiblePotentielle != -1){
									listePV.push_back(moteur.getEtat().getPersonnages()[indiceCiblePotentielle]->getStatistiques().getPV());
									listeIndice.push_back(indiceCiblePotentielle);
								}
							}
							
							for (size_t j=0; j < listeIndice.size(); j++){
								nomArme_attaquant = moteur.getEtat().getPersonnages()[i]->getNomArme();
								nomArme_cible = moteur.getEtat().getPersonnages()[listeIndice[j]]->getNomArme();
								score = 0;
								if(nomArme_cible==nomArme_attaquant){
									bonus_attaque=0;
								}
								else if(nomArme_attaquant=="Arc" || nomArme_cible=="Arc"){
									bonus_attaque=0;
								}
								else if((nomArme_attaquant=="Hache" && nomArme_cible=="Lance")|| (nomArme_attaquant=="Lance" && nomArme_cible=="Epee") || (nomArme_attaquant=="Epee" && nomArme_cible=="Hache")){
									bonus_attaque=5;
								}
								else if ((nomArme_cible=="Hache" && nomArme_attaquant=="Lance")|| (nomArme_cible=="Lance" && nomArme_attaquant=="Epee") || (nomArme_cible=="Epee" && nomArme_attaquant=="Hache")){
									bonus_attaque=-5;
								}
								
								// Prise en compte du taux de coup critique, de l'attaque et du bonus d'attaque
								score = score + bonus_attaque + moteur.getEtat().getPersonnages()[i]->getStatistiques().getAttaque() + moteur.getEtat().getPersonnages()[i]->getStatistiques().getCritique(); 
								
								// Prise en compte de l'esquive, des PV et de la defense de l'adversaire
								score = score - (moteur.getEtat().getPersonnages()[listeIndice[j]]->getStatistiques().getEsquive() + moteur.getEtat().getPersonnages()[listeIndice[j]]->getStatistiques().getDefense() + moteur.getEtat().getPersonnages()[listeIndice[j]]->getStatistiques().getPV());
								listeScores.push_back(score);
								
								cout << "Attaque contre " << moteur.getEtat().getPersonnages()[listeIndice[j]]->getNom();
								cout << " : " << score << endl;
								
								
							
							}
							//On veut attaquer la cible la plus faible									
							int indiceMaxScore=indiceMaximum(listeScores);//On détermine l'indice du PV min dans listePV  
							int indiceCible=listeIndice[indiceMaxScore];//On détermine l'indice de notre cible dans la liste de personnages

							// Commande d'attaque
							Attaque attaque(*moteur.getEtat().getPersonnages()[i], *moteur.getEtat().getPersonnages()[indiceCible], camp);
							unique_ptr<Commande> ptr_attaque (new Attaque(attaque));
							moteur.addCommande(0, move(ptr_attaque));
							moteur.update(window);
							sleep(1);
					}
					
					// 2 : Cas de fin d'actions
					else if (action == 2){
						FinActions finactions(*moteur.getEtat().getPersonnages()[i], camp);
						unique_ptr<Commande> ptr_finactions (new FinActions(finactions));
						moteur.addCommande(0, move(ptr_finactions));
						moteur.update(window);
						sleep(1);
					}
					
				}		
			}
		}
	}

}

int HeuristicIA::findIndiceVoisin(engine::Moteur& moteur, int personnage){
	
	
	vector<int> listeDistance;
	vector<int> indicePerso;
	
	for(size_t i=0; i<moteur.getEtat().getPersonnages().size(); i++){
		if(moteur.getEtat().getPersonnages()[i]->getCamp()!=moteur.getEtat().getPersonnages()[personnage]->getCamp()){
			listeDistance.push_back(moteur.getEtat().getPersonnages()[personnage]->getPosition().distance(moteur.getEtat().getPersonnages()[i]->getPosition()));
			indicePerso.push_back(i);
		}
	}
	
	int indiceMinDistance=this->indiceMinimum(listeDistance);
	int indiceVoisin= indicePerso[indiceMinDistance];
	return indiceVoisin;
}

int HeuristicIA::findchoixOptimalDep(engine::Moteur& moteur, int indiceVoisin, int personnage){
	int choix=-1;
	vector<int> listeDistanceAVoisin;
	vector<Position> listePosition=moteur.getEtat().getPersonnages()[personnage]->getLegalMove(moteur.getEtat());
	if(listePosition.size()!=0){
		for(size_t i=0; i<listePosition.size(); i++){
			listeDistanceAVoisin.push_back(listePosition[i].distance(moteur.getEtat().getPersonnages()[indiceVoisin]->getPosition()));
		}
		choix=this->indiceMinimum(listeDistanceAVoisin);
	}
	return choix;
}

int HeuristicIA::indiceMinimum(std::vector<int> liste){
	int indiceMin=-1;
	if(liste.size()!=0){//liste non vide
		indiceMin=0;
		int min=liste[0];
		for(size_t i=1; i<liste.size(); i++){
			if(liste[i]<min){
				min=liste[i];
				indiceMin=i;
			}
		}
	}
	return indiceMin;
}

int HeuristicIA::indiceMaximum(std::vector<int> liste){
	int indiceMax=-1;
	if(liste.size()!=0){//liste non vide
		indiceMax=0;
		int max=liste[0];
		for(size_t i=1; i<liste.size(); i++){
			if(liste[i]>max){
				max=liste[i];
				indiceMax=i;
			}
		}
	}
	return indiceMax;
}

Position HeuristicIA::findRefuge(engine::Moteur& moteur, int personnage){
	Position positionRefuge;
	std::vector<Position> listeRefuges;
	std::vector<int> listeDistances;
	for(size_t j = 0; j<moteur.getEtat().getGrille().size(); j++){
		for(size_t i = 0; i<moteur.getEtat().getGrille()[0].size(); i++){
			if (moteur.getEtat().getGrille()[i][j]->getNom() == "Maison" || moteur.getEtat().getGrille()[i][j]->getNom() == "Forteresse"){
				positionRefuge = moteur.getEtat().getGrille()[i][j]->getPosition();
				listeRefuges.push_back(positionRefuge);
			}
		}
	}
	
	if(listeRefuges.size()!=0){
		for(size_t k = 0; k < listeRefuges.size(); k++){
			listeDistances.push_back(moteur.getEtat().getPersonnages()[personnage]->getPosition().distance(listeRefuges[k]));
		}
	}
	
	return listeRefuges[indiceMinimum(listeDistances)];
}


