#include "HeuristicIA.h"
#include "engine.h"
#include "state.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "ai.h"

using namespace ai;
using namespace engine;
using namespace state;
using namespace std;
//

/** Pseudo Code de l'algorithme de recherche de chemin
Initialisation
Sommet source (S)
Sommet destination (D)
Liste des sommets à explorer (E) : sommet source S
Liste des sommets visités (V) : vide

Tant que (la liste E est non vide) et (D n’est pas dans E) Faire
	+ Récupérer le sommet X de coût total F minimum.
	+ Ajouter X à la liste V
	+ Ajouter les successeurs de X (non déjà visités) à la liste E en
   évaluant leur coût total F et en identifiant leur prédécesseur.

	+ Si (un successeur est déjà présent dans E) et
         (nouveau coût est inférieur à l’ancien) Alors
		Changer son coût total
		Changer son prédécesseur
	FinSi
FinFaire*/

void HeuristicIA::run (engine::Moteur& moteur){

		if(moteur.getJoueurActif()==camp && moteur.getEtat().getFin() == false){
		
		bool armeeAdverseAneantie = true;
			
		for (size_t i = 0; i < moteur.getEtat().getPersonnages().size(); i++){
			int action=-1;
			
			if (moteur.getEtat().getPersonnages()[i]-> getCamp() == camp){
				size_t indiceObjectif=0;
				while (moteur.getEtat().getPersonnages()[i]->getStatut() != MORT && moteur.getEtat().getPersonnages()[i]->getStatut() != ATTENTE){
					moteur.getEtat().getPersonnages()[i]->setStatut(SELECTIONNE);
					vector<Position> objectif;
					vector<Position> listeAttaques = moteur.getEtat().getPersonnages()[i]-> getLegalAttack(moteur.getEtat());
					armeeAdverseAneantie = true;
					
					// On vérifie qu'il reste des cibles
					for (size_t l = 0; l < moteur.getEtat().getPersonnages().size(); l++){
						if (moteur.getEtat().getPersonnages()[l]->getCamp() != camp && moteur.getEtat().getPersonnages()[l]->getStatut() != MORT){
							armeeAdverseAneantie = false;
							break;
						}
					}
					
					// Si les cibles adverses sont toutes mortes, le personnage termine son tour pour déclencher
					// la fin de partie
					if (armeeAdverseAneantie){
						action = 2;
					}
					
					// S'il reste des adversaires
					else{
						// Si le personnage a moins de 5 PV et qu'il se trouve sur un refuge, il termine son tour
						if(moteur.getEtat().getPersonnages()[i]->getStatistiques().getPV()<=5 && (moteur.getEtat().getGrille()[moteur.getEtat().getPersonnages()[i]->getPosition().getX()][moteur.getEtat().getPersonnages()[i]->getPosition().getY()]->getNom() == "Maison" || moteur.getEtat().getGrille()[moteur.getEtat().getPersonnages()[i]->getPosition().getX()][moteur.getEtat().getPersonnages()[i]->getPosition().getY()]->getNom() == "Forteresse")){
							action = 2;
						}
						
						else{
							objectif = findObjectif(moteur, i); 
							// Si la position de l'objectif est occupée, on en déduit que c'est un ennemi
							// attaquer
							if(moteur.getEtat().getGrille()[objectif[indiceObjectif].getX()][objectif[indiceObjectif].getY()]->isOccupe(moteur.getEtat())!=-1){
								
								bool resTest = false;
								for (size_t k = 0; k < listeAttaques.size(); k++){
									if(objectif[indiceObjectif].equals(listeAttaques[k])){
										resTest = true;
										break;
									}
								}					
								
								// Si la position de l'ennemi est dans le champ d'attaque, on demande une attaque
								if (resTest){
									action = 1;
								}
							}
						}
					}
					// Si l'objectif est d'atteindre un refuge ou de se rapprocher d'un ennemi, on demande un 							deplacement
					if(action == -1){
						action = 0;
					}
					
					sleep(1);
					
					// 0 : Cas du deplacement
					if (action==0){ 
						if(moteur.getEtat().getPersonnages()[i]->getChampMove() != 0){
							int indiceVoisin=this->findIndiceVoisin(moteur, i);
							if(indiceVoisin!=-1){
								
								indiceObjectif=0;	
													
								vector<Position> chemin=AlgorithmeA(moteur, i, objectif[indiceObjectif]);
								while(chemin.size()==0 && indiceObjectif!=(objectif.size()-1)){
									indiceObjectif++;	
									chemin=AlgorithmeA(moteur, i, objectif[indiceObjectif]);
								}
								if(chemin.size()!=0){
									Deplacement deplacement(*moteur.getEtat().getPersonnages()[i], chemin[chemin.size()-1], camp);
									unique_ptr<Commande> ptr_deplacement (new Deplacement(deplacement));
									moteur.addCommande(0, move(ptr_deplacement));
									//moteur.update();
									moteur.notifyUpdating();
								}
								else{
									action=2;
								}
							}
							else{action=2;}								
						}
						else{
							action=2;
						}
					}
					
					// 1 : Cas de l'attaque
					else if (action == 1){
							// Commande d'attaque
							Attaque attaque(*moteur.getEtat().getPersonnages()[i], *moteur.getEtat().getPersonnages()[moteur.getEtat().getGrille()[objectif[indiceObjectif].getX()][objectif[indiceObjectif].getY()]->isOccupe(moteur.getEtat())], camp);												
							unique_ptr<Commande> ptr_attaque (new Attaque(attaque));
							moteur.addCommande(0, move(ptr_attaque));
							//moteur.update();
							moteur.notifyUpdating();
							sleep(1);
						}
					
					// 2 : Cas de fin d'actions
					else if (action == 2){
						FinActions finactions(*moteur.getEtat().getPersonnages()[i], camp);
						unique_ptr<Commande> ptr_finactions (new FinActions(finactions));
						moteur.addCommande(0, move(ptr_finactions));
						//moteur.update();
						moteur.notifyUpdating();
						usleep(200000);
					}
					
				}		
			}
		}
	}
}

void HeuristicIA::runRollback (engine::Moteur& moteur){

		if(moteur.getJoueurActif()==camp && moteur.getEtat().getFin() == false){
		
		bool armeeAdverseAneantie = true;
			
		for (size_t i = 0; i < moteur.getEtat().getPersonnages().size(); i++){
			int action=-1;
			
			if (moteur.getEtat().getPersonnages()[i]-> getCamp() == camp){
				size_t indiceObjectif=0;
				while (moteur.getEtat().getPersonnages()[i]->getStatut() != MORT && moteur.getEtat().getPersonnages()[i]->getStatut() != ATTENTE){
					moteur.getEtat().getPersonnages()[i]->setStatut(SELECTIONNE);
					vector<Position> objectif;
					vector<Position> listeAttaques = moteur.getEtat().getPersonnages()[i]-> getLegalAttack(moteur.getEtat());
					armeeAdverseAneantie = true;
					
					// On vérifie qu'il reste des cibles
					for (size_t l = 0; l < moteur.getEtat().getPersonnages().size(); l++){
						if (moteur.getEtat().getPersonnages()[l]->getCamp() != camp && moteur.getEtat().getPersonnages()[l]->getStatut() != MORT){
							armeeAdverseAneantie = false;
							break;
						}
					}
					
					// Si les cibles adverses sont toutes mortes, le personnage termine son tour pour déclencher
					// la fin de partie
					if (armeeAdverseAneantie){
						action = 2;
					}
					
					// S'il reste des adversaires
					else{
						// Si le personnage a moins de 5 PV et su'il se trouve sur un refuge, il termine son tour
						if(moteur.getEtat().getPersonnages()[i]->getStatistiques().getPV()<=5 && (moteur.getEtat().getGrille()[moteur.getEtat().getPersonnages()[i]->getPosition().getX()][moteur.getEtat().getPersonnages()[i]->getPosition().getY()]->getNom() == "Maison" || moteur.getEtat().getGrille()[moteur.getEtat().getPersonnages()[i]->getPosition().getX()][moteur.getEtat().getPersonnages()[i]->getPosition().getY()]->getNom() == "Forteresse")){
							action = 2;
						}
						
						else{
							objectif = findObjectif(moteur, i); 
							// Si la position de l'objectif est occupée, on en déduit que c'est un ennemi
							// attaquer
							if(moteur.getEtat().getGrille()[objectif[indiceObjectif].getX()][objectif[indiceObjectif].getY()]->isOccupe(moteur.getEtat())!=-1){
								
								bool resTest = false;
								for (size_t k = 0; k < listeAttaques.size(); k++){
									if(objectif[indiceObjectif].equals(listeAttaques[k])){
										resTest = true;
										break;
									}
								}					
								
								// Si la position de l'ennemi est dans le champ d'attaque, on demande une attaque
								if (resTest){
									action = 1;
								}
							}
						}
					}
					// Si l'objectif est d'atteindre un refuge ou de se rapprocher d'un ennemi, on demande un 							deplacement
					if(action == -1){
						action = 0;
					}
					
					sleep(1);
					
					// 0 : Cas du deplacement
					if (action==0){ 
						if(moteur.getEtat().getPersonnages()[i]->getChampMove() != 0){
							int indiceVoisin=this->findIndiceVoisin(moteur, i);
							if(indiceVoisin!=-1){
								
								indiceObjectif=0;	
													
								vector<Position> chemin=AlgorithmeA(moteur, i, objectif[indiceObjectif]);
								while(chemin.size()==0 && indiceObjectif!=(objectif.size()-1)){
									indiceObjectif++;	
									chemin=AlgorithmeA(moteur, i, objectif[indiceObjectif]);
								}
								if(chemin.size()!=0){
									Dep_Action deplacement(*moteur.getEtat().getPersonnages()[i], chemin[chemin.size()-1], camp);
									Dep_Action* ptr_deplacement (new Dep_Action(deplacement));
									moteur.addAction(move(ptr_deplacement));
									moteur.updateAction(move(ptr_deplacement));
									moteur.update();
									//moteur.notifyUpdating();
								}
								else{
									action=2;
								}
							}
							else{action=2;}								
						}
						else{
							action=2;
						}
					}
					
					// 1 : Cas de l'attaque
					else if (action == 1){
							// Commande d'attaque
							Attaque_Action attaque(*moteur.getEtat().getPersonnages()[i], *moteur.getEtat().getPersonnages()[moteur.getEtat().getGrille()[objectif[indiceObjectif].getX()][objectif[indiceObjectif].getY()]->isOccupe(moteur.getEtat())], camp);												
							Attaque_Action* ptr_attaque (new Attaque_Action(attaque));
							moteur.addAction(move(ptr_attaque));
							moteur.updateAction(move(ptr_attaque));
							moteur.update();
							//moteur.notifyUpdating();
							sleep(1);
						}
					
					// 2 : Cas de fin d'actions
					else if (action == 2){
						FinActions_Action finactions(*moteur.getEtat().getPersonnages()[i], camp);
						FinActions_Action* ptr_finactions (new FinActions_Action(finactions));
						moteur.addAction(move(ptr_finactions));
						moteur.updateAction(move(ptr_finactions));
						moteur.update();
						//moteur.notifyUpdating();
						usleep(200000);
					}
					
				}		
			}
		}
	}
}


vector<Position> HeuristicIA::AlgorithmeA(engine::Moteur& moteur, int i, state::Position posDestination){
	vector<Position> chemin;
	bool chgtCible=false;
	int isPresentV=-1;
	int isPresentPos2=-1;
	Position posSource=moteur.getEtat().getPersonnages()[i]->getPosition();
	Sommet *old, *old2, *old3, *old4;
	Sommet source(nullptr);
	source.setPosition(posSource);
	source.setDistanceSource(0);
	
	int indiceOccupe=moteur.getEtat().getGrille()[posDestination.getX()][posDestination.getY()]->isOccupe(moteur.getEtat());
	if(indiceOccupe!=-1){
		vector<Position> posnextCible=moteur.getEtat().getPersonnages()[indiceOccupe]->getLegalMove(moteur.getEtat());
		if(posnextCible.size()!=0){
			vector<int> dist;
			for(size_t j=0; j<posnextCible.size(); j++){
				dist.push_back(posnextCible[j].distance(posSource));
			}
			posDestination=posnextCible[indiceMinimum(dist)];
		}
		else{// cible non accessible
			chgtCible=true;
		}
	}
	if(chgtCible){
		return chemin;
	}
	else{
	
	int indiceMin=-1;
	
	size_t tailleV=0;
	size_t tailleE=1;
	Sommet destination(0);
	Sommet* V2=new Sommet[tailleV];
	
	vector<int> listeCout;
	Sommet* E2=new Sommet[tailleE];
	E2[0]=source;
	Position positionAjoutee(0, 0);
	
	int isPresentPos=-1;
	
	//Tant que la liste E est non vide et D n’est pas dans E
	while(tailleE!=0 && isPresentPos==-1){
	
		if(listeCout.size()!=0){
			while(listeCout.size()!=0){
				listeCout.pop_back();
			}
		}

		for(size_t i=0; i<tailleE; i++){
			if(!E2[i].getPredecesseur()){
				// Distance du sommet X à Source
				E2[i].setDistanceSource(E2[i].getPosition().distance(source.getPosition()));
			}
			else{
				// Distance du sommet X à Source
				E2[i].setDistanceSource(1+E2[i].getPredecesseur()->getDistanceSource());			
			}
			
			// Distance du sommet X à Destination
			E2[i].setDistanceDestination(E2[i].getPosition().distance(posDestination));	

			if(E2[i].getCoutTotal()>0){
				listeCout.push_back(E2[i].getCoutTotal());
			}	
		}
		
		// Récupérer le sommet X d'indice indiceMin de coût total minimum
		indiceMin=indiceMinimum(listeCout);

		old=V2;
		tailleV=tailleV+1;
		V2=new Sommet[tailleV];
		
		for(size_t t=0; t<tailleV-1; t++){
			V2[t]=old[t];
		}
		
		// Ajouter X à la liste V (sommets visités)
		V2[tailleV-1]=E2[indiceMin];
				
		//Retirer X à la liste E
		old2=E2;
		tailleE=tailleE-1; 
		E2=new Sommet[tailleE];
		size_t indice=indiceMin;
		for(size_t t=0; t<indice; t++){
				E2[t]=old2[t];		
		}
		for(size_t m=indice; m<tailleE; m++){
				E2[m]=old2[m+1];		
		}
		
		// On parcourt les abscisses puis les ordonnees de la grille
		for(int abscisse = V2[tailleV-1].getPosition().getX() - 1; abscisse <=  V2[tailleV-1].getPosition().getX() + 1; abscisse++){
			for(int ordonnee =  V2[tailleV-1].getPosition().getY() - 1; ordonnee <=  V2[tailleV-1].getPosition().getY() + 1; ordonnee++){
				// On teste les cases autour de la position			
				if(abs(abscisse- V2[tailleV-1].getPosition().getX())+abs(ordonnee- V2[tailleV-1].getPosition().getY())<=1 && abscisse>=0 && ordonnee>=0 && 	abs(abscisse)<moteur.getEtat().getGrille().size() && abs(ordonnee)<moteur.getEtat().getGrille()[abscisse].size()){
					if(moteur.getEtat().getGrille()[abscisse][ordonnee]->isPraticable()){
						if(moteur.getEtat().getGrille()[abscisse][ordonnee]->isOccupe(moteur.getEtat())==-1){
							positionAjoutee.setX(abscisse);
							positionAjoutee.setY(ordonnee);
							if(!positionAjoutee.equals(V2[tailleV-1].getPosition())){
								Sommet voisin(&V2[tailleV-1]);
								voisin.setPosition(positionAjoutee);
								isPresentV=-1;
								for(size_t q=0; q<tailleV; q++){
									if(V2[q].getPosition().equals(positionAjoutee)){
										isPresentV=q;
									}
								}

								isPresentPos2=-1;
								for(size_t q=0; q<tailleE; q++){
									if(E2[q].getPosition().equals(positionAjoutee)){
										isPresentPos2=q;
										
									}
								}

								if(isPresentV==-1 && isPresentPos2==-1){
									if(posDestination.equals(positionAjoutee)){
										destination=voisin;
									}

									old3=E2;

									tailleE=tailleE+1;
									E2=new Sommet[tailleE];
									for(size_t t=0; t<tailleE-1; t++){
										E2[t]=old3[t];
									}
									
									E2[tailleE-1]=voisin;				
								}
								// Si un successeur est déjà présent dans E
								else if(isPresentPos2!=-1){
									// Distance du sommet X à Source
									voisin.setDistanceSource(1+voisin.getPredecesseur()->getDistanceSource());
									// Distance du sommet X à Destination
voisin.setDistanceDestination(voisin.getPosition().distance(posDestination));
	
									// Si nouveau coût est inférieur à l’ancien
									if(voisin.getCoutTotal()<E2[isPresentPos2].getCoutTotal()){
										old4=E2;
										size_t indice2=isPresentPos2;
										for(size_t t=0; t<indice2; t++){
												E2[t]=old4[t];		
										}
										
										E2[indice2]=voisin;
										for(size_t t=indice2+1; t<tailleE; t++){
												E2[t]=old4[t];		
										}
									}									
								}
							}					
						}
					}
				}
			}
		}
	}
	
	delete[] E2;
	delete[] V2;
	Sommet* finger=&destination;
	
	chemin.push_back(finger->getPosition());

	if(finger->getPredecesseur()){
		while(finger->getPredecesseur() && !finger->getPredecesseur()->getPosition().equals(source.getPosition())){			
			finger=finger->getPredecesseur();
			chemin.push_back(finger->getPosition());
		}
	}
	
	else{
		chemin.pop_back();
	}
	
	return chemin;
	}
}

	
int HeuristicIA::isPresentSom(vector<Sommet> listeSommet, Sommet sommet){
	int isPresent=-1;
	for(size_t i=0; i<listeSommet.size(); i++){
		if(listeSommet[i].getPosition().equals(sommet.getPosition())){
			isPresent=i;
			break;
		}
	}
	return isPresent;
}

int HeuristicIA::isPresentPos(vector<Sommet> listeSommet, state::Position position){
	int isPresent=-1;
	for(size_t i=0; i<listeSommet.size(); i++){
		if(listeSommet[i].getPosition().equals(position)){
			isPresent=i;
			break;
		}
	}
	return isPresent;
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

int HeuristicIA::findIndiceVoisin(engine::Moteur& moteur, int personnage){	
	
	vector<int> listeDistance;
	vector<int> indicePerso;
	int indiceMinDistance;
	int indiceVoisin;
	for(size_t i=0; i<moteur.getEtat().getPersonnages().size(); i++){
		if(moteur.getEtat().getPersonnages()[i]->getCamp()!=moteur.getEtat().getPersonnages()[personnage]->getCamp() &&moteur.getEtat().getPersonnages()[i]->getStatut()!=MORT){
			listeDistance.push_back(moteur.getEtat().getPersonnages()[personnage]->getPosition().distance(moteur.getEtat().getPersonnages()[i]->getPosition()));
			indicePerso.push_back(i);
		}
	}
	if(listeDistance.size()!=0){
		indiceMinDistance=this->indiceMinimum(listeDistance);
		indiceVoisin= indicePerso[indiceMinDistance];
	}
	else{
		indiceVoisin=-1;
	}
	return indiceVoisin;
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

vector<Position> HeuristicIA::findRefuge(engine::Moteur& moteur, int personnage){
	Position positionRefuge;
	std::vector<Position> listeRefuges, listeRefugesClasses;
	std::vector<int> listeDistances;
	for(size_t j = 0; j<moteur.getEtat().getGrille().size(); j++){
		for(size_t i = 0; i<moteur.getEtat().getGrille()[0].size(); i++){
			// Recherche des refuges (maisons ou forteresses) non occupées
			if ((moteur.getEtat().getGrille()[i][j]->getNom() == "Maison" || moteur.getEtat().getGrille()[i][j]->getNom() == "Forteresse") && moteur.getEtat().getGrille()[i][j]->isOccupe(moteur.getEtat()) == -1){
				positionRefuge = moteur.getEtat().getGrille()[i][j]->getPosition();
				listeRefuges.push_back(positionRefuge);
			}
		}
	}
	
	if(listeRefuges.size()!=0){
		for(size_t k = 0; k < listeRefuges.size(); k++){
			listeDistances.push_back(moteur.getEtat().getPersonnages()[personnage]->getPosition().distance(listeRefuges[k]));
		}
		//int minDistance=indiceMinimum(listeDistances);
		int listeDistancesProv;
		while(listeDistances.size()!=0){
			listeRefugesClasses.push_back(listeRefuges[indiceMinimum(listeDistances)]);
			listeDistancesProv=listeDistances[indiceMinimum(listeDistances)];
			listeDistances[indiceMinimum(listeDistances)]=listeDistances.back();
			listeDistances.back()=listeDistancesProv;
			listeDistances.pop_back();	
		}
		
	}
	else{
		positionRefuge.setX(-1);
		positionRefuge.setY(-1);
		listeRefugesClasses.push_back(positionRefuge);
	}
	return listeRefugesClasses;
}

std::vector<state::Position> HeuristicIA::findObjectif(engine::Moteur& moteur, int indicePerso){
	// Cas de la fuite : si un personnage possède moins de 5 PV, il cherche un refuge
	vector<Position> listeRefuges;
	if(moteur.getEtat().getPersonnages()[indicePerso]->getStatistiques().getPV()<=5){
		listeRefuges = findRefuge(moteur, indicePerso);
		if (listeRefuges[0].getX()!=-1 && listeRefuges[0].getY()!=-1){
			return listeRefuges;
		}
	}
	
	// Cas de l'attaque	
	vector<int> listePV, listeIndice, listeScores;
	int bonus_attaque=-1;
	int score = 0;
	vector<Position> listePosition;		
	int indiceMaxScore, indiceCible;
			
	for (size_t i = 0; i < moteur.getEtat().getPersonnages().size(); i++){
		// Personnages adverses non morts
		if (moteur.getEtat().getPersonnages()[i]-> getCamp() != camp && moteur.getEtat().getPersonnages()[i]->getStatut() != MORT){
			
			std:: string nomArme_attaquant = moteur.getEtat().getPersonnages()[indicePerso]->getNomArme();
			std:: string nomArme_cible = moteur.getEtat().getPersonnages()[i]->getNomArme();
			score = 0;
				
			if(nomArme_cible==nomArme_attaquant){
				bonus_attaque=0;
			}
			else if(nomArme_attaquant=="Arc" || nomArme_cible=="Arc"){
				bonus_attaque=0;
			}
			else if((nomArme_attaquant=="Hache" && nomArme_cible=="Lance")|| (nomArme_attaquant=="Lance" && nomArme_cible=="Epee") || (nomArme_attaquant=="Epee" && nomArme_cible=="Hache")){
				bonus_attaque=2*5;
			}
			else if ((nomArme_cible=="Hache" && nomArme_attaquant=="Lance")|| (nomArme_cible=="Lance" && nomArme_attaquant=="Epee") || (nomArme_cible=="Epee" && nomArme_attaquant=="Hache")){
				bonus_attaque=2*(-5);
			}
								
			// Prise en compte du taux de coup critique, de l'attaque et du bonus d'attaque
			score = score + bonus_attaque + moteur.getEtat().getPersonnages()[indicePerso]->getStatistiques().getAttaque() + moteur.getEtat().getPersonnages()[indicePerso]->getStatistiques().getCritique(); 
								
			// Prise en compte de l'esquive, des PV et de la defense de l'adversaire
			score = score - (moteur.getEtat().getPersonnages()[i]->getStatistiques().getEsquive() + moteur.getEtat().getPersonnages()[i]->getStatistiques().getDefense() + moteur.getEtat().getPersonnages()[i]->getStatistiques().getPV());
				
			// Prise en compte de la distance à l'adversaire
			score = score - (moteur.getEtat().getPersonnages()[indicePerso]->getPosition().distance(moteur.getEtat().getPersonnages()[i]->getPosition()));
			
			listeScores.push_back(score);
			listeIndice.push_back(i);
		}
	}
	int listeScoresProv, listeIndiceProv;
	while(listeScores.size()!=0){	
	//On veut attaquer la cible la plus faible									
		indiceMaxScore=indiceMaximum(listeScores);
		indiceCible=listeIndice[indiceMaxScore];
		listePosition.push_back(moteur.getEtat().getPersonnages()[indiceCible]->getPosition());	

		listeScoresProv=listeScores[indiceMaxScore];
		listeIndiceProv=listeIndice[indiceMaxScore];
		listeScores[indiceMaxScore]=listeScores.back();
		listeIndice[indiceMaxScore]=listeIndice.back();
		listeScores.back()=listeScoresProv;
		listeIndice.back()=listeIndiceProv;
		listeScores.pop_back();
		listeIndice.pop_back();
	}
		
	return listePosition;
}


