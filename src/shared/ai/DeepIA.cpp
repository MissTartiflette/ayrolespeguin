#include "DeepIA.h"
#include "engine.h"
#include "state.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "ai.h"

using namespace ai;
using namespace std;
using namespace engine;
using namespace state;

void DeepIA::run(engine::Moteur& moteur, sf::RenderWindow& window){
	if(moteur.getJoueurActif()==camp && moteur.getEtat().getFin() == false){
		//for (size_t i = 0; i < moteur.getEtat().getPersonnages().size(); i++){
			//if (moteur.getEtat().getPersonnages()[i]-> getCamp() == camp){
				//while (moteur.getEtat().getPersonnages()[4]->getStatut() != MORT && moteur.getEtat().getPersonnages()[4]->getStatut() != ATTENTE){
		campChoisi=false;
		int max_val=-30000;
		int valeur;
		std::vector<Action*> listeActions;
	
		listeActions = findActionsPossibles(moteur.getEtat(), campChoisi, moteur);
		cout<<"taille liste Actions :"<<listeActions.size()<<endl;
		for(size_t k=0; k<listeActions.size(); k++){
			cout<<"ID action "<<k<<" : " << listeActions[k]->getActionID()<<endl;
		}
		sleep(1); 
		for(size_t i=0; i<listeActions.size(); i++){
			
			listeActions[i]->apply(moteur.getEtat());	
			
			valeur = min(moteur, profondeur-1);
			if(valeur > max_val){
				max_val= valeur;
				if(listeActions[i]->getActionID()==DEP_ACTION){
					Dep_Action& listeActionsI=static_cast<Dep_Action&>(*listeActions[i]);
					Deplacement deplacement(listeActionsI.getCible(), listeActionsI.getDestination(),listeActionsI.getJoueur());
					unique_ptr<Commande> meilleureCommande(new Deplacement(deplacement));
					moteur.addCommande(0, move(meilleureCommande));
				}
				else if(listeActions[i]->getActionID()==ATTAQUE_ACTION){
					Attaque_Action& listeActionsI=static_cast<Attaque_Action&>(*listeActions[i]);
					Attaque attaque(listeActionsI.getAttaquant(), listeActionsI.getCible(),listeActionsI.getJoueur());
					unique_ptr<Commande> meilleureCommande (new Attaque(attaque));
					moteur.addCommande(0, move(meilleureCommande));
				}
				else if(listeActions[i]->getActionID()==FINACTIONS_ACTION){
					FinActions_Action& listeActionsI=static_cast<FinActions_Action&>(*listeActions[i]);
					FinActions finactions(listeActionsI.getCible(),listeActionsI.getJoueur());
					unique_ptr<Commande> meilleureCommande (new FinActions(finactions));
					moteur.addCommande(0, move(meilleureCommande));
				}
				
			}
			listeActions[i]->undo(moteur.getEtat());	
		}
		
		moteur.update(window);
	//}
	}
}

int DeepIA::fonctionEvaluation(engine::Moteur& moteur){
	int valeur_retour;
	if(moteur.getEtat().getFin()){
		if(!moteur.getJoueurActif()){
			valeur_retour=1000-moteur.getEtat().getTour();
		}
		else{
			valeur_retour=-1000+moteur.getEtat().getTour();
		}
	}
	else{
		int sommePV=0, sommePVAdverse=0, nbPersoVivant=0, nbPersoVivantAd=0;
		Position pos_mort(-1,-1);
		for(size_t i=0; i<moteur.getEtat().getPersonnages().size(); i++){
			if(!moteur.getEtat().getPersonnages()[i]->getPosition().equals(pos_mort)){
				if(moteur.getEtat().getPersonnages()[i]->getCamp()){
					sommePVAdverse+=moteur.getEtat().getPersonnages()[i]->getStatistiques().getPV();
					nbPersoVivantAd+=1;
				}
				else{
					sommePV+=moteur.getEtat().getPersonnages()[i]->getStatistiques().getPV();
					nbPersoVivant+=1;
				}
			}
		}
		valeur_retour=sommePV-sommePVAdverse+200*nbPersoVivant-200*nbPersoVivantAd;
		cout<<"valeur fonction eval :" << valeur_retour<<endl;
	}
	return valeur_retour;
}

int DeepIA::min(engine::Moteur& moteur, int profondeur){
	cout<<"entree min - profondeur : "<<profondeur << endl; 
	int minValue = 30000;
	if (profondeur == 0 || moteur.getEtat().getFin()==true){
		return fonctionEvaluation(moteur); 
	}
	else{
		
		//cout<<"camp choisi : " << campChoisi<<endl;
		campChoisi=!campChoisi;
		std::vector<engine::Action*> listeActions = findActionsPossibles(moteur.getEtat(), campChoisi, moteur);
		cout<<"taille : " <<listeActions.size()<<endl;
		//cout<<"camp choisi : " << campChoisi<<endl;
		int valeur; 
		for(size_t i=0; i<listeActions.size(); i++){
			cout<<"ID action "<<i<<" : " << listeActions[i]->getActionID()<<endl;
			listeActions[i]->apply(moteur.getEtat());
			valeur = max(moteur, profondeur-1);
			if(valeur < minValue){
				minValue = valeur;
			}
			listeActions[i]->undo(moteur.getEtat());	
		}
	}
	return minValue;
}


int DeepIA::max(engine::Moteur& moteur, int profondeur){
	cout<<"entree fonction max - profondeur : "<<profondeur<<endl; 
	sleep(1);
	int maxValue = -30000;
	sleep(1);
	if (profondeur == 0 || moteur.getEtat().getFin()==true){
		return fonctionEvaluation(moteur); 
	}
	else{
		campChoisi=!campChoisi;
		std::vector<engine::Action*> listeActions = findActionsPossibles(moteur.getEtat(), campChoisi, moteur);
		cout<<"taille : " <<listeActions.size()<<endl;
		int valeur;
		for(size_t i=0; i<listeActions.size(); i++){
			cout<<"ID action "<<i<<" : " << listeActions[i]->getActionID()<<endl;
			listeActions[i]->apply(moteur.getEtat());	
			valeur = min(moteur, profondeur-1);
			if(valeur > maxValue){
				maxValue = valeur;
			}
			listeActions[i]->undo(moteur.getEtat());		
		}
	}
	return maxValue;
}

std::vector<engine::Action*> DeepIA::findActionsPossibles(state::Etat& etat, bool campChoisi, engine::Moteur& moteur){
	
	std::vector<engine::Action*> listeActions;
	std::vector<state::Position> listeAttaques;
	std::vector<state::Position> listePositions;
	
	for (size_t indicePersonnage = 0; indicePersonnage < etat.getPersonnages().size(); indicePersonnage++){
		// Parcours des personnages du camp choisi (false=IA)
		if(etat.getPersonnages()[indicePersonnage]->getCamp() == campChoisi && etat.getPersonnages()[indicePersonnage]->getStatut()!= state::MORT && etat.getPersonnages()[indicePersonnage]->getStatut()!= state::ATTENTE){
		
			// Ajout des attaques possibles
			listeAttaques = etat.getPersonnages()[indicePersonnage]->getLegalAttack(etat);
			for (size_t i = 0; i < listeAttaques.size(); i++){
				engine::Attaque_Action attaquePossible(*etat.getPersonnages()[indicePersonnage], *etat.getPersonnages()[etat.getGrille()[listeAttaques[i].getX()][listeAttaques[i].getY()]->isOccupe(etat)], etat.getPersonnages()[indicePersonnage]->getCamp());		
				Action* ptr_attaque_action = new Attaque_Action(attaquePossible);				
				listeActions.push_back(ptr_attaque_action);
			}
			
			// Ajout des déplacements possibles
			vector<Position> objectif = findObjectif(moteur, indicePersonnage);
			vector<Position> chemin=AlgorithmeA(moteur, indicePersonnage, objectif[0]); 
			if(etat.getPersonnages()[indicePersonnage]->getChampMove()!=0){
				//listePositions = etat.getPersonnages()[indicePersonnage]->getLegalMove(etat);
				//for (size_t i = 0; i < listePositions.size(); i++){
				engine::Dep_Action deplacementPossible(*etat.getPersonnages()[indicePersonnage], chemin[chemin.size()-1], etat.getPersonnages()[indicePersonnage]->getCamp());
				Action* ptr_dep_action = new Dep_Action(deplacementPossible);
				listeActions.push_back(ptr_dep_action);
				//}
			}
			else{
			
				// Ajout de la fin du tour d'actions
				engine::FinActions_Action finaction(*etat.getPersonnages()[indicePersonnage], etat.getPersonnages()[indicePersonnage]->getCamp());
				Action* ptr_fin_action = new FinActions_Action(finaction);
				listeActions.push_back(ptr_fin_action);	
			}		
		}
	}
	return listeActions;
}

vector<Position> DeepIA::AlgorithmeA(engine::Moteur& moteur, int i, state::Position posDestination){
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

	
int DeepIA::isPresentSom(vector<Sommet> listeSommet, Sommet sommet){
	int isPresent=-1;
	for(size_t i=0; i<listeSommet.size(); i++){
		if(listeSommet[i].getPosition().equals(sommet.getPosition())){
			isPresent=i;
			break;
		}
	}
	return isPresent;
}

int DeepIA::isPresentPos(vector<Sommet> listeSommet, state::Position position){
	int isPresent=-1;
	for(size_t i=0; i<listeSommet.size(); i++){
		if(listeSommet[i].getPosition().equals(position)){
			isPresent=i;
			break;
		}
	}
	return isPresent;
}

int DeepIA::indiceMinimum(std::vector<int> liste){
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

int DeepIA::findIndiceVoisin(engine::Moteur& moteur, int personnage){	
	
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

int DeepIA::indiceMaximum(std::vector<int> liste){
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

vector<Position> DeepIA::findRefuge(engine::Moteur& moteur, int personnage){
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

std::vector<state::Position> DeepIA::findObjectif(engine::Moteur& moteur, int indicePerso){
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


