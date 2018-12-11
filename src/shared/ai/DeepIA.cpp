#include "DeepIA.h"
#include "engine.h"
#include "state.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "ai.h"

using namespace ai;
using namespace std;

void DeepIA::run(engine::Moteur& moteur, sf::RenderWindow& window){// à compléter et corriger 
	int max_val=-30000;
	int valeur;
	std::vector<engine::Action*> listeActions = findActionsPossibles(moteur.getEtat());
	Commande* meilleureCommande;
	
	for(size_t i; i<listeActions.size(); i++){
		moteur.updateAction(window, listeActions[i]);
		valeur = min(moteur, profondeur);
		if(valeur > max_val){
			max_val= valeur;
			meilleureCommande=listeActions[i]// à modifier car une commande n'est pas une action
		}
		moteur.undo(window, listeActions[i]);	
	}
	moteur.addCommande(0, meilleureCommande);
	moteur.update(window);
}

int DeepIA::fonctionEvaluation(engine::Moteur& moteur){
	int valeur_retour;
	if(moteur.getEtat().getFin()){
		if(!moteur.getJoueurActif()){
			valeur_retour=1000-etat.getTour();
		}
		else{
			valeur_retour=-1000+etat.getTour();
		}
	}
	else{
		int sommePV=0, sommePVAdverse=0, nbPersoVivant=0, nbPersoVivantAd=0;
		Poisition pos_mort(-1,-1);
		for(size_t i=0, i<moteur.getEtat().getPersonnages().size(); i++){
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
	}
	return valeur_retour;
}

int DeepIA::min(engine::Moteur& moteur, int profondeur){
	int minValue = 30000;
	if (profondeur == 0 || moteur.getEtat().getFin()==true){
		return fonctionEvaluation(moteur.getEtat()); // FONCTION A CREER ET ADAPTER
	}
	else{
		std::vector<engine::Action*> listeActions = findActionsPossibles(moteur.getEtat());
		int valeur; 
		for(size_t i; i<listeActions.size(); i++){
			moteur.updateAction(window, listeActions[i]);
			valeur = max(moteur, profondeur-1);
			if(valeur < minValue){
				minValue = valeur;
			}
			moteur.undo(window, listeActions[i]);	
		}
	}
	return minValue;
}


int DeepIA::max(engine::Moteur& moteur, int profondeur){
	int maxValue = -30000;
	if (profondeur == 0 || moteur.getEtat().getFin()==true){
		return fonctionEvaluation(moteur.getEtat()); // FONCTION A CREER ET ADAPTER
	}
	else{
		std::vector<engine::Action*> listeActions = findActionsPossibles(moteur.getEtat());
		int valeur;
		for(size_t i; i<listeActions.size(); i++){
			moteur.updateAction(window, listeActions[i]);
			valeur = min(moteur, profondeur-1);
			if(valeur > maxValue){
				maxValue = valeur;
			}
			moteur.undo(window, listeActions[i]);	
		}
	}
	return maxValue;
}

std::vector<engine::Action*> findActionsPossibles(state::Etat& etat){
	int indicePersonnage = 4;
	std::vector<engine::Action*> listeActions;
	
	// Ajout des attaques possibles
	std::vector<Position> listeAttaques = etat.getPersonnages()[indicePersonnage]->getLegalAttack(etat);
	for (size_t i = 0; i < listeAttaques.size(); i++){
		Attaque_Action attaquePossible(*etatActuel.getPersonnages()[indicePersonnage], *etatActuel.getPersonnages()[listeAttaques[isOccupe()]], etatActuel.getPersonnages()[attaquant]->getCamp());						
		listeActions.push_back(&attaquePossible);
	}
	
	// Ajout des déplacements possibles
	std::vector<Position> listePositions = etat.getPersonnages()[indicePersonnage]->getLegalMove(etat);
	for (size_t i = 0; i < listePositions.size(); i++){
		Dep_Action deplacementPossible(*etatActuel.getPersonnages()[indicePersonnage], listePositions[i], joueurActif);
		listeCommandes.push_back(&deplacementPossible);
	}
	
	// Ajout de la fin du tour d'actions
	FinActions_Action finaction(*etatActuel.getPersonnages()[numeroPerso], etatActuel.getPersonnages()[numeroPerso]->getCamp());
	listeCommandes.push_back(&finaction);
	
	return listeActions;
}
