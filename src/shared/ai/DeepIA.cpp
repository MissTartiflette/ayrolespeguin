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
	
		listeActions = findActionsPossibles(moteur.getEtat(), campChoisi);
		cout<<"taille liste Actions :"<<listeActions.size()<<endl;
		sleep(1); 
		for(size_t i=0; i<listeActions.size(); i++){
			//cout<<"ID action : "<< listeActions[i]->getActionID()<<endl;
			
			listeActions[i]->apply(moteur.getEtat());	
			//sleep(1);
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
	int minValue = 30000;
	if (profondeur == 0 || moteur.getEtat().getFin()==true){
		return fonctionEvaluation(moteur); 
	}
	else{
		
		cout<<"camp choisi : " << campChoisi<<endl;
		campChoisi=!campChoisi;
		std::vector<engine::Action*> listeActions = findActionsPossibles(moteur.getEtat(), campChoisi);
		cout<<"camp choisi : " << campChoisi<<endl;
		int valeur; 
		for(size_t i=0; i<listeActions.size(); i++){
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
	int maxValue = -30000;
	if (profondeur == 0 || moteur.getEtat().getFin()==true){
		return fonctionEvaluation(moteur); 
	}
	else{
		campChoisi=!campChoisi;
		std::vector<engine::Action*> listeActions = findActionsPossibles(moteur.getEtat(), campChoisi);
		int valeur;
		for(size_t i=0; i<listeActions.size(); i++){
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

std::vector<engine::Action*> DeepIA::findActionsPossibles(state::Etat& etat, bool campChoisi){
	
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
			if(etat.getPersonnages()[indicePersonnage]->getChampMove()!=0){
				listePositions = etat.getPersonnages()[indicePersonnage]->getLegalMove(etat);
				for (size_t i = 0; i < listePositions.size(); i++){
					engine::Dep_Action deplacementPossible(*etat.getPersonnages()[indicePersonnage], listePositions[i], etat.getPersonnages()[indicePersonnage]->getCamp());
					Action* ptr_dep_action = new Dep_Action(deplacementPossible);
					listeActions.push_back(ptr_dep_action);
				}
			}
			
			// Ajout de la fin du tour d'actions
			engine::FinActions_Action finaction(*etat.getPersonnages()[indicePersonnage], etat.getPersonnages()[indicePersonnage]->getCamp());
			Action* ptr_fin_action = new FinActions_Action(finaction);
			listeActions.push_back(ptr_fin_action);			
		}
	}
	return listeActions;
}
