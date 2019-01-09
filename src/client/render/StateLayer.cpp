#include <SFML/Graphics.hpp>
#include <string>
#include "render.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <unistd.h>


using namespace render;
using namespace std;
using namespace state;


StateLayer::StateLayer(state::Etat& etat, sf::RenderWindow& window): window(window){

	//police.loadFromFile("res/8-BIT-WONDER.TTF");
	police.loadFromFile("res/Pixellari.ttf");
	
	TileSet tilesetGrille(GRILLETILESET);
	std::unique_ptr<TileSet> ptr_tilesetGrille (new TileSet(tilesetGrille));
	tilesets.push_back(move(ptr_tilesetGrille));
	
	TileSet tilesetPersonnages(PERSONNAGETILESET);
	std::unique_ptr<TileSet> ptr_tilesetPersonnages (new TileSet(tilesetPersonnages));
	tilesets.push_back(move(ptr_tilesetPersonnages));
	
	TileSet tilesetCurseur(CURSEURTILESET);
	std::unique_ptr<TileSet> ptr_tilesetCurseur (new TileSet(tilesetCurseur));
	tilesets.push_back(move(ptr_tilesetCurseur));

	TileSet tilesetInfos(INFOSTILESET);
	std::unique_ptr<TileSet> ptr_tilesetInfos (new TileSet(tilesetInfos));
	tilesets.push_back(move(ptr_tilesetInfos));
}

void StateLayer::initSurfaces(state::Etat& etat){	
	Surface surfGrille;
	Surface surfPersonnage;
	Surface surfCurseur;
	Surface surfInfos;

	surfGrille.loadGrille(etat, tilesets[0]->getTexture(), sf::Vector2u(tilesets[0]->getCellWidth(), tilesets[0]->getCellHeight()), etat.getGrille().size(), etat.getGrille()[0].size());

	surfPersonnage.loadPersonnage(etat, tilesets[1]->getTexture(), sf::Vector2u(tilesets[1]->getCellWidth(), tilesets[1]->getCellHeight()), etat.getPersonnages().size(), 1);

	surfCurseur.loadCurseur(etat, tilesets[2]->getTexture(), sf::Vector2u(tilesets[2]->getCellWidth(), tilesets[2]->getCellHeight()), 1, 1);
	
	surfInfos.loadInfos(etat, tilesets[3]->getTexture(), sf::Vector2u(tilesets[3]->getCellWidth(), tilesets[3]->getCellHeight()), 1, 1);
	
	std::unique_ptr<Surface> ptr_surfGrille (new Surface(surfGrille));
	std::unique_ptr<Surface> ptr_surfPersonnage (new Surface(surfPersonnage));
	std::unique_ptr<Surface> ptr_surfCurseur (new Surface(surfCurseur));
	std::unique_ptr<Surface> ptr_surfInfos (new Surface(surfInfos));
	
	if(surfaces.size()!=0){
		while(surfaces.size()!=0){
			surfaces.pop_back();
		}
	}
	
	surfaces.push_back(move(ptr_surfGrille));
	surfaces.push_back(move(ptr_surfPersonnage));
	surfaces.push_back(move(ptr_surfCurseur));
	surfaces.push_back(move(ptr_surfInfos));
	
}

std::vector<std::unique_ptr<TileSet>>& StateLayer::getTilesets (){
	std::vector<std::unique_ptr<TileSet>>& ref_tilesets = tilesets;
	return ref_tilesets;
}

std::vector<std::unique_ptr<Surface>>& StateLayer::getSurfaces (){
	std::vector<std::unique_ptr<Surface>>& ref_surfaces = surfaces;
	return ref_surfaces;
}

void StateLayer::stateChanged (const state::StateEvent& e, state::Etat& etat){
	if (e.stateEventID == ALLCHANGED){
		initSurfaces(etat);
		draw(window);
	}
	if (e.stateEventID == TEXTECHANGED){
		writeTexteAction(e.texte, window);
	}
}

void StateLayer::draw (sf::RenderWindow& window){
	window.clear();
	
	// Rectangle degrade en (0,400) et de taille 400x200
	sf::VertexArray quad(sf::Quads, 4);
	quad[0].position = sf::Vector2f(0.f, 400.f);
	quad[1].position = sf::Vector2f(400.f, 400.f);
	quad[2].position = sf::Vector2f(400.f, 600.f);
	quad[3].position = sf::Vector2f(0.f, 600.f);
	quad[0].color = sf::Color::Blue;
	quad[1].color = sf::Color::Red;
	quad[2].color = sf::Color::Black;
	quad[3].color = sf::Color::Black;
	
	// Rectangle noir pour les messages
	sf::RectangleShape rectangle(sf::Vector2f(390.f, 30.f));
	rectangle.setPosition(5.f, 405.f);
	rectangle.setFillColor(sf::Color::Black);
	/*
	// Rectangle noir de Statistiques
	sf::RectangleShape rectangle2(sf::Vector2f(100.f, 102.f));
	rectangle2.setPosition(5+64+5, 400+5+30+5);
	rectangle2.setFillColor(sf::Color::Black);*/
	
	window.draw(*surfaces[0]);	// Dessin de la grille				
	window.draw(*surfaces[1]);	// Dessin des personnages
	window.draw(*surfaces[2]);	// Dessin du curseur
	window.draw(quad);
	window.draw(rectangle);
	//window.draw(rectangle2);
	
	window.draw(*surfaces[3]); // Dessin des infos
	window.display();
}

void StateLayer::writeTexteAction(const std::string chaine, sf::RenderWindow& window){
	sf::RectangleShape rectangle(sf::Vector2f(390.f, 30.f));
	rectangle.setPosition(5.f, 405.f);
	rectangle.setFillColor(sf::Color::Black);
	
	sf::Text text;
	text.setFont(police);
	text.setString(chaine);
	text.setCharacterSize(15);
	text.setFillColor(sf::Color::White);
	text.setPosition(10.f, 400.f);
	
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin((int)(textRect.left + textRect.width/2.0f),
		           (int)(textRect.top  + textRect.height/2.0f));
	text.setPosition(sf::Vector2f(400/2.0f, 420));
	
	window.draw(rectangle);
	window.draw(text);
	window.display();
}

void StateLayer::writeStatistiques(const std::string chaine, sf::RenderWindow& window){
	// Rectangle noir de Statistiques
	sf::RectangleShape rectangle2(sf::Vector2f(100.f, 102.f));
	rectangle2.setPosition(5+64+5, 400+5+30+5);
	rectangle2.setFillColor(sf::Color::Black);
	
	std::string chaine0 = "Oliver\nPV\nAtk\nDef\nEsq\nCrt";
	sf::Text text;
	text.setFont(police);
	text.setString(chaine0);
	text.setCharacterSize(15);
	text.setFillColor(sf::Color::White);
	text.setPosition(5+64+5+4,400+5+30+5+2);
	
	std::string chaine2 = "\n:\t100\n:\t50\n:\t30\n:\t10\n:\t30";
	sf::Text text2;
	text2.setFont(police);
	text2.setString(chaine2);
	text2.setCharacterSize(15);
	text2.setFillColor(sf::Color::White);
	text2.setPosition(5+64+5+4+30,400+5+30+5+2);
	
	window.draw(rectangle2);
	window.draw(text);
	window.draw(text2);
	window.display();
}

void StateLayer::gestionCurseur(sf::Event newEvent, unsigned int largeur_map_cases, unsigned int longueur_map_cases, Etat& etatActuel){

	StateEvent stateEvent(ALLCHANGED);

	/*  CURSEUR  */


	// ROLLBACK
	//
	
	if (newEvent.type==sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
		cout << "ROLLBACK ! : "; //<< listeActionsJouees.size() << " actions a annuler" << endl;
		/*if (listeActionsJouees.size()>0){
			for(int i = listeActionsJouees.size()-1; i >= 0; i--){
				undo(move(listeActionsJouees[i]));
				
				cout << "Annulation " << i << " effectuee" << endl;
				usleep(750000);
				listeActionsJouees.pop_back();
			}
		}*/
		
		CurseurEvent curseurEvent0(R);
		Position nextPos0(0, 0);
		notifyObservers(etatActuel, curseurEvent0.touche, -1, -1, nextPos0);

		// Test pour changement de couleur du curseur si le rollback est declenché alors qu'un personnage est selectionne et n'a effectue aucune action
		bool persoSelectionne = false;
		for (size_t i = 0; i<etatActuel.getPersonnages().size(); i++){
			if (etatActuel.getPersonnages()[i]->getStatut() == state::SELECTIONNE){
				persoSelectionne = true;
				break;
			}
		}		
		if(!persoSelectionne){
			etatActuel.getCurseur()->setCodeTuile(0);
		}
		etatActuel.notifyObservers(stateEvent, etatActuel);
		cout << "FIN DU ROLLBACK" << endl;
	}	
							
	// Appui d'une flèche directionnelle ou de Enter sans selection 
	// (moteur.getEtat().verifStatut()!=-1)
	else if(newEvent.type==sf::Event::KeyPressed && (etatActuel.verifStatut()==-1)){
		
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
			std::string newChaine;
			if(numeroPerso != -1){
				// Selection du personnage
				if(etatActuel.getPersonnages()[numeroPerso]->getCamp() && etatActuel.getPersonnages()[numeroPerso]->getStatut() != ATTENTE && etatActuel.getPersonnages()[numeroPerso]->getStatut() != MORT){
				
					etatActuel.getCurseur()->setCodeTuile(3);
					
					newChaine = "Debut du tour de " + etatActuel.getPersonnages()[numeroPerso]->getNom();
					cout<< "\t\t->" << newChaine << " <-" << endl;
					
					etatActuel.getPersonnages()[numeroPerso]->setStatut(SELECTIONNE);
					etatActuel.notifyObservers(stateEvent, etatActuel);
				}									
				else if (etatActuel.getPersonnages()[numeroPerso]->getStatut() == ATTENTE){
					newChaine = "Ce personnage a deja termine son tour";
					cout << "\t\t" << newChaine << endl;
				}
				else{	newChaine = "Ce personnage appartient a l'adversaire !";
					cout<< newChaine <<endl;}
			}
			
			// Affichage du type de terrain							
			else{	newChaine = "Ce terrain est de type " + etatActuel.getGrille()[yCurs][xCurs]->getNom();
					cout << newChaine << endl;
			}
			stateEvent.texte = newChaine;
			stateEvent.stateEventID = TEXTECHANGED;
			etatActuel.notifyObservers(stateEvent, etatActuel);
		}
							
		// Déplacement du curseur
		if (changementX != 0 || changementY !=0){
			Position nextPosCurs(xCurs+changementX, yCurs+changementY);
			etatActuel.getCurseur()->move(nextPosCurs);
			etatActuel.notifyObservers(stateEvent, etatActuel);			
			
			std::string newChaine;
			int resTest = etatActuel.getGrille()[nextPosCurs.getX()][nextPosCurs.getY()]->isOccupe(etatActuel);
			
			if (resTest != -1){newChaine = etatActuel.getPersonnages()[resTest]->getNom();}
			else{newChaine = etatActuel.getGrille()[nextPosCurs.getX()][nextPosCurs.getY()]->getNom();}
			stateEvent.texte = newChaine;
			stateEvent.stateEventID = TEXTECHANGED;
			etatActuel.notifyObservers(stateEvent, etatActuel);
			
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
		etatActuel.notifyObservers(stateEvent, etatActuel);
		
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
					etatActuel.notifyObservers(stateEvent, etatActuel);
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
					etatActuel.notifyObservers(stateEvent, etatActuel);
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
					etatActuel.notifyObservers(stateEvent, etatActuel);
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
					etatActuel.notifyObservers(stateEvent, etatActuel); 
					usleep(200000);
				}
			}
								
			// Annulation de l'attaque avec N
			else if(sf::Keyboard::isKeyPressed(sf::Keyboard::N)){
				cible=-2;
				cout<< "\tAttaque annulée" << endl;									
				etatActuel.getCurseur()->setCodeTuile(3);
				etatActuel.notifyObservers(stateEvent, etatActuel);
				
				if(!etatActuel.getCurseur()->getPosition().equals(etatActuel.getPersonnages()[attaquant]->getPosition())){
					Position nextPosCurs(etatActuel.getPersonnages()[attaquant]->getPosition().getX(), etatActuel.getPersonnages()[attaquant]->getPosition().getY());
					etatActuel.getCurseur()->move(nextPosCurs);
									
					etatActuel.notifyObservers(stateEvent, etatActuel);
				}
			}
								
			// Validation de l'attaque avec Enter
			else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return)){
				cible=etatActuel.getGrille()[etatActuel.getCurseur()->getPosition().getX()][etatActuel.getCurseur()->getPosition().getY()]->isOccupe(etatActuel);
			}
		}
							
		// Validation de l'attaque
		if(cible>=0){
			/*Attaque attaque(*etatActuel.getPersonnages()[attaquant], *etatActuel.getPersonnages()[cible], etatActuel.getPersonnages()[attaquant]->getCamp());
			unique_ptr<Commande> ptr_attaque (new Attaque(attaque));
			addCommande(0, move(ptr_attaque));*/
			CurseurEvent curseurEvent1(A);
			CurseurEvent curseurEvent2(UPDATE);
			Position nextPos2(0, 0);
			notifyObservers(etatActuel, curseurEvent1.touche, attaquant, cible, nextPos2);
			etatActuel.getCurseur()->setCodeTuile(2);
			etatActuel.notifyObservers(stateEvent, etatActuel);	

			notifyObservers(etatActuel, curseurEvent2.touche, -1, -1, nextPos2);			
			//update();
			etatActuel.getCurseur()->setCodeTuile(0);
			etatActuel.notifyObservers(stateEvent, etatActuel);									
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
			/*FinActions finaction(*etatActuel.getPersonnages()[numeroPerso], etatActuel.getPersonnages()[numeroPerso]->getCamp());
			unique_ptr<Commande> ptr_finaction (new FinActions(finaction));
			addCommande(0, move(ptr_finaction));*/
			CurseurEvent curseurEvent3(Z);
			CurseurEvent curseurEvent4(UPDATE);		
			Position nextPos1(0, 0);
			notifyObservers(etatActuel, curseurEvent3.touche, numeroPerso, -1, nextPos1);						
			etatActuel.getCurseur()->setCodeTuile(0);
			//update();
			notifyObservers(etatActuel, curseurEvent4.touche, -1, -1, nextPos1);
		}
		
		// Deplacement du curseur et du personnage selectionne
		if (changementX != 0 || changementY != 0){
			Position nextPos(xPerso+changementX, yPerso+changementY);
			Position nextPos3(0, 0);
			/*Deplacement deplacement(*etatActuel.getPersonnages()[numeroPerso], nextPos, joueurActif);
			unique_ptr<Commande> ptr_deplacement (new Deplacement(deplacement));
			addCommande(0, move(ptr_deplacement));*/
			CurseurEvent curseurEvent5(M);
			CurseurEvent curseurEvent6(UPDATE);
			notifyObservers(etatActuel, curseurEvent5.touche, numeroPerso, -1, nextPos);				
			etatActuel.getCurseur()->move(nextPos);
			//update();
			notifyObservers(etatActuel, curseurEvent6.touche, -1, -1, nextPos3);
								
			changementX = 0;
			changementY = 0;
		}
	}
}

