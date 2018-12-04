#include "engine.h"
#include "render.h"
#include "state.h"
#include <iostream>
#include <unistd.h>


using namespace state;
using namespace engine;
using namespace render;
using namespace std;

Moteur::Moteur () : etatActuel(){
	changementTour = false;
	joueurActif = true;
}

Moteur::~Moteur (){
}

state::Etat& Moteur::getEtat (){
	state::Etat& refEtat=etatActuel;
	return refEtat;
}

void Moteur::addCommandePassive (){
	//commandesActuelles.insert(std::pair<int, std::unique_ptr<Commande>> (commandesActuelles.size(), ptr_cmd));
}

void Moteur::addCommande (int priorite, std::unique_ptr<Commande> ptr_cmd){
	commandesActuelles[priorite]=move(ptr_cmd);
	
}

void Moteur::update (sf::RenderWindow& window){
	StateEvent stateEvent(ALLCHANGED);

	map<int, std::unique_ptr<Commande>>::iterator it;

	for(size_t i=0; i<commandesActuelles.size();i++){
		// On n'execute que les commandes du joueur dont c'est le tour
		if (commandesActuelles[i]->joueur == joueurActif){
			commandesActuelles[i]->execute(etatActuel);
			etatActuel.notifyObservers(stateEvent, etatActuel, window);
			//sleep(2);
		}
	}
	for(it=commandesActuelles.begin(); it!=commandesActuelles.end(); it++){
		commandesActuelles.erase(it);
	}
}

bool Moteur::verificationFinDeTour(){
	bool tourChange = true;
	bool partieFinie = true;
	//int countJoueurActif = 0;
	
	for (unsigned int i = 0; i < etatActuel.getPersonnages().size(); i++){
		// Si un personage du joueur actif n'est ni mort ni en attente, son tour n'est pas termine
		if (etatActuel.getPersonnages()[i]->getCamp() == joueurActif){
			//countJoueurActif = countJoueurActif + 1;
			if (etatActuel.getPersonnages()[i]->getStatut() != MORT ){
				if (etatActuel.getPersonnages()[i]->getStatut() != ATTENTE){
					tourChange = false;
				}
			}
			/*
			else{
				countJoueurActif = countJoueurActif - 1;
			}
			*/			
		}
		
		
		// Si tous les personnages du joueur non actif ne sont pas morts, la partie n'est pas terminee
		else{
			if (etatActuel.getPersonnages()[i]->getStatut() != MORT ){
				partieFinie = false;
				
			}		
		}
	}
	/*
	if (countJoueurActif == 0){
		partieFinie = true;
	}
	*/
	
	if (partieFinie && tourChange){
		cout << "\tPartie Terminee !" << endl;
		etatActuel.setFin(partieFinie);
		//if (countJoueurActif == 0){							 A MODIFIER}
		if (joueurActif){
			cout << "\tL'armee bleue a gagne !" << endl;
		}
		else {
			cout << "\tL'armee rouge a gagne !" << endl;
		}
		tourChange = false;
	}
		
	else if (tourChange && !partieFinie){
		cout << "\t\t--- Tour Terminé. ---\n" << endl;
		etatActuel.setTour(etatActuel.getTour()+1);
	}
	
	changementTour = tourChange;
	
	return tourChange;

}

void Moteur::verificationDebutDeTour(){
	if (changementTour == true){
	
		joueurActif = !joueurActif;
		cout << "\t-> Changement de joueur <-" << endl;
		cout << "\t\t--- Tour " << etatActuel.getTour() << " ---\n" << endl;
		
		for (unsigned int i = 0; i < etatActuel.getPersonnages().size(); i++){
		
			// Personnages du joueur qui termine son tour et qui ne sont pas morts
			if (etatActuel.getPersonnages()[i]->getCamp() != joueurActif && etatActuel.getPersonnages()[i]->getStatut() != MORT){
				// Reinitialisation du statut
				etatActuel.getPersonnages()[i]->setStatut(DISPONIBLE);
				
				//Reinitialisation des points de mouvement
				if (etatActuel.getPersonnages()[i]->getType() != CHEVALIER){
					etatActuel.getPersonnages()[i]->setChampMove(3);
				}
				else {
					etatActuel.getPersonnages()[i]->setChampMove(5);
				}	
			}
			
			// Regain de PV pour les personnages sur des maisons et fortersse en debut de tour
			else if (etatActuel.getPersonnages()[i]->getCamp() == joueurActif && etatActuel.getPersonnages()[i]->getStatut()!=MORT) {
				TerrainPraticable& refTerrainP = static_cast<TerrainPraticable&>(*etatActuel.getGrille()[etatActuel.getPersonnages()[i]->getPosition().getY()][etatActuel.getPersonnages()[i]->getPosition().getX()]);
				
				if(refTerrainP.getTerrainPraticableID() == MAISON || refTerrainP.getTerrainPraticableID() == FORTERESSE){
					// Le maximum de points de vie est de 25
					if (etatActuel.getPersonnages()[i]->getStatistiques().getPV() + refTerrainP.getStatistiques().getPV()<= 25){
						etatActuel.getPersonnages()[i]->getStatistiques().setPV(etatActuel.getPersonnages()[i]->getStatistiques().getPV() + refTerrainP.getStatistiques().getPV());
						// Affichage
						cout << "+ " << etatActuel.getPersonnages()[i]->getNom() << " récupère " ;
						cout << refTerrainP.getStatistiques().getPV() << " PV.";
						cout << " (" << etatActuel.getPersonnages()[i]->getStatistiques().getPV() << " PV au total). +" << endl;
					}
					else if (etatActuel.getPersonnages()[i]->getStatistiques().getPV() == 25){
						cout << "+ " << etatActuel.getPersonnages()[i]->getNom() << " a déjà atteint son maximum de PV. +" << endl;
					}
					else {
						cout << "+ " << etatActuel.getPersonnages()[i]->getNom() << " récupère " ;
						cout << 25 -  etatActuel.getPersonnages()[i]->getStatistiques().getPV()<< " PV.";
						etatActuel.getPersonnages()[i]->getStatistiques().setPV(25);
						cout << " (" << etatActuel.getPersonnages()[i]->getStatistiques().getPV() << " PV au total). +" << endl;
					}
				}
			}
		}
		
		changementTour = !changementTour;
	}
}

bool Moteur::getJoueurActif(){
	return joueurActif;
}


void Moteur::gestionCurseur(sf::Event newEvent, sf::RenderWindow& window, unsigned int largeur_map_cases, unsigned int longueur_map_cases){

	StateEvent stateEvent(ALLCHANGED);

	/*  CURSEUR  */
							
	// Appui d'une flèche directionnelle ou de Enter sans selection 
	// (moteur.getEtat().verifStatut()!=-1)
	if(newEvent.type==sf::Event::KeyPressed && (etatActuel.verifStatut()==-1)){
		
		int changementX = 0, changementY = 0;
		size_t xCurs=etatActuel.getCurseur()->getPosition().getX();
		size_t yCurs=etatActuel.getCurseur()->getPosition().getY();
		int numeroPerso = etatActuel.getGrille()[xCurs][yCurs]->isOccupe(etatActuel);
							
		// Fleches directionnelles (deplacement du curseur)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
			if(xCurs!=largeur_map_cases-1){changementX = 1;}
			else{changementX = -xCurs;}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
			if(xCurs!=0){changementX = -1;}
			else{changementX = largeur_map_cases-1 -xCurs;}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
			if(yCurs!=0){changementY = -1;}
			else{changementY = longueur_map_cases-1 -yCurs;}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
			if(yCurs!=longueur_map_cases-1){changementY = 1;}
			else{changementY = -yCurs;}
		}
		
		// Enter (selection)
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)){								
			if(numeroPerso != -1){
				// Selection du personnage
				if(etatActuel.getPersonnages()[numeroPerso]->getCamp() && etatActuel.getPersonnages()[numeroPerso]->getStatut() != ATTENTE && etatActuel.getPersonnages()[numeroPerso]->getStatut() != MORT){
					etatActuel.getCurseur()->setCodeTuile(3);
					cout<< "\t\t-> Début du tour de " << etatActuel.getPersonnages()[numeroPerso]->getNom() << " <-" << endl;
					etatActuel.getPersonnages()[numeroPerso]->setStatut(SELECTIONNE);
					
					etatActuel.notifyObservers(stateEvent, etatActuel, window);
				}									
				else if (etatActuel.getPersonnages()[numeroPerso]->getStatut() == ATTENTE){
					cout << "\t\tCe personnage a déjà terminé son tour." << endl;
				}
				else{	cout<<"Ce personnage appartient à l'adversaire !" <<endl;}
			}
			
			// Affichage du type de terrain							
			else{	std::string newChaine = "Ce terrain est de type " + etatActuel.getGrille()[yCurs][xCurs]->getNom();
					cout << newChaine << endl;
					stateEvent.texte = newChaine;
					stateEvent.stateEventID = TEXTECHANGED;
					etatActuel.notifyObservers(stateEvent, etatActuel, window);
			}
		}
							
		// Déplacement du curseur
		if (changementX != 0 || changementY !=0){
			Position nextPosCurs(xCurs+changementX, yCurs+changementY);
			etatActuel.getCurseur()->move(nextPosCurs);
			etatActuel.notifyObservers(stateEvent, etatActuel, window);			
			
			std::string newChaine = etatActuel.getGrille()[nextPosCurs.getX()][nextPosCurs.getY()]->getNom();
			stateEvent.texte = newChaine;
			stateEvent.stateEventID = TEXTECHANGED;
			etatActuel.notifyObservers(stateEvent, etatActuel, window);
			
			changementX = 0;
			changementY = 0;
		}
	}
	
	// Declenchement d'un attaque
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && (etatActuel.verifStatut()!=-1)){
	
		cout<< "\tAttaque en préparation" << endl;
							
		int attaquant=etatActuel.verifStatut();
		int champDroit=etatActuel.getPersonnages()[attaquant]->getChampAttack();
		int champGauche=champDroit;
		int champHaut=champDroit;
		int champBas=champDroit;
		int cible=-1;
		
		// Changement de couleur du curseur
		etatActuel.getCurseur()->setCodeTuile(1);
		etatActuel.notifyObservers(stateEvent, etatActuel, window);
		
		// Actions
		while((cible==-1) || (cible==attaquant)){	
									
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
				size_t xCurs=etatActuel.getCurseur()->getPosition().getX();
				size_t yCurs=etatActuel.getCurseur()->getPosition().getY();
				
				if(xCurs!=largeur_map_cases-1 && champDroit!=0){
					Position nextPosCurs(xCurs+1, yCurs);
					etatActuel.getCurseur()->move(nextPosCurs);
					champDroit=champDroit-1;
					champGauche++;	
					etatActuel.notifyObservers(stateEvent, etatActuel, window);
					usleep(200000);
				}
			}
			else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
				size_t xCurs=etatActuel.getCurseur()->getPosition().getX();
				size_t yCurs=etatActuel.getCurseur()->getPosition().getY();
				
				if(xCurs!=0 && champGauche!=0){
					Position nextPosCurs(xCurs-1, yCurs);	
					etatActuel.getCurseur()->move(nextPosCurs);
					champGauche=champGauche-1;
					champDroit++;
					etatActuel.notifyObservers(stateEvent, etatActuel, window);
					usleep(200000);
				}
			}
			else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
				size_t xCurs=etatActuel.getCurseur()->getPosition().getX();
				size_t yCurs=etatActuel.getCurseur()->getPosition().getY();
			
				if(yCurs!=longueur_map_cases-1 && champBas!=0){
					Position nextPosCurs(xCurs, yCurs+1);	
					etatActuel.getCurseur()->move(nextPosCurs);
					champBas=champBas-1;
					champHaut++;
					etatActuel.notifyObservers(stateEvent, etatActuel, window);
					usleep(200000);
				}
			}
			else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
				size_t xCurs=etatActuel.getCurseur()->getPosition().getX();
				size_t yCurs=etatActuel.getCurseur()->getPosition().getY();
				if(yCurs!=0 && champHaut!=0){
					Position nextPosCurs(xCurs, yCurs-1);
					etatActuel.getCurseur()->move(nextPosCurs);
					champHaut=champHaut-1;
					champBas++;
					etatActuel.notifyObservers(stateEvent, etatActuel, window); 
					usleep(200000);
				}
			}
								
			// Annulation de l'attaque avec N
			else if(sf::Keyboard::isKeyPressed(sf::Keyboard::N)){
				cible=-2;
				cout<< "\tAttaque annulée" << endl;									
				etatActuel.getCurseur()->setCodeTuile(3);
				etatActuel.notifyObservers(stateEvent, etatActuel, window);
								
				if(!etatActuel.getCurseur()->getPosition().equals(etatActuel.getPersonnages()[attaquant]->getPosition())){
					Position nextPosCurs(etatActuel.getPersonnages()[attaquant]->getPosition().getX(), etatActuel.getPersonnages()[attaquant]->getPosition().getY());
					etatActuel.getCurseur()->move(nextPosCurs);
									
					etatActuel.notifyObservers(stateEvent, etatActuel, window);
				}
			}
								
			// Validation de l'attaque avec Enter
			else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return)){
				cible=etatActuel.getGrille()[etatActuel.getCurseur()->getPosition().getX()][etatActuel.getCurseur()->getPosition().getY()]->isOccupe(etatActuel);
			}
		}
							
		// Validation de l'attaque
		if(cible>=0){
			Attaque attaque(*etatActuel.getPersonnages()[attaquant], *etatActuel.getPersonnages()[cible], etatActuel.getPersonnages()[attaquant]->getCamp());
			unique_ptr<Commande> ptr_attaque (new Attaque(attaque));
			addCommande(0, move(ptr_attaque));
			
			etatActuel.getCurseur()->setCodeTuile(2);
			etatActuel.notifyObservers(stateEvent, etatActuel, window);				
			update(window);
			etatActuel.getCurseur()->setCodeTuile(0);
			etatActuel.notifyObservers(stateEvent, etatActuel, window);									
		}
	}
	
	// Actions avec personnage selectionne (moteur.getEtat().verifStatut()!=-1)
	else if(newEvent.type==sf::Event::KeyPressed && (etatActuel.verifStatut()!=-1)){
		
		int changementX = 0, changementY = 0;
		int numeroPerso=etatActuel.verifStatut();
		size_t xPerso=etatActuel.getPersonnages()[numeroPerso]->getPosition().getX();
		size_t yPerso=etatActuel.getPersonnages()[numeroPerso]->getPosition().getY();
							
		// Fleches directionnelles (deplacement du curseur et du personnage)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
			if(xPerso!=largeur_map_cases-1){changementX = 1;}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
			if(xPerso!=0){changementX = -1;}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
			if(yPerso!=0){changementY = -1;}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
			if(yPerso!=longueur_map_cases-1){changementY = 1;}
		}
							
		// Touche Z : Fin de tour d'actions
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)){							
			FinActions finaction(*etatActuel.getPersonnages()[numeroPerso], etatActuel.getPersonnages()[numeroPerso]->getCamp());
			unique_ptr<Commande> ptr_finaction (new FinActions(finaction));
			addCommande(0, move(ptr_finaction));								
			etatActuel.getCurseur()->setCodeTuile(0);
			update(window);
		}
		
		// Deplacement du curseur et du personnage selectionne
		if (changementX != 0 || changementY != 0){
			Position nextPos(xPerso+changementX, yPerso+changementY);
			Deplacement deplacement(*etatActuel.getPersonnages()[numeroPerso], nextPos, joueurActif);
			unique_ptr<Commande> ptr_deplacement (new Deplacement(deplacement));
			addCommande(0, move(ptr_deplacement));
								
			etatActuel.getCurseur()->move(nextPos);
			update(window);
								
			changementX = 0;
			changementY = 0;
		}
	}
}
 
