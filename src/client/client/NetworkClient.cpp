#include "engine.h"
#include "render.h"
#include "state.h"
#include "client.h"
#include "ai.h"
#include <iostream>
#include <unistd.h>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <thread>

using namespace state;
using namespace engine;
using namespace render;
using namespace std;
using namespace client;
using namespace ai;


 /*std::string url;
    int port;
    int character;
    engine::Moteur moteur;
    std::unique_ptr<ai::IA> player_ai;*/

bool l1=false;
bool l2=true;
void thread_moteur2(void* ptr){
	Moteur* ptr_moteur=(Moteur*)ptr;
	while(l2){
		//usleep(1000);
		if(l1){
			cout<<"commande recue"<<endl;
			ptr_moteur->update();
			l1=false;
		}
	}
	
}


NetworkClient::NetworkClient (std::string& url, int port, int character, sf::RenderWindow& window):url(url), port(port), character(character), window(window){
	
	unsigned int longueur_map_cases = 25, largeur_map_cases = 25;
	std::string chemin_fichier_map_txt = "res/map1.txt";
	Correspondances tab_corres = Correspondances();

	moteur.getEtat().initGrille(chemin_fichier_map_txt, longueur_map_cases, largeur_map_cases, tab_corres);
	moteur.getEtat().initPersonnages(tab_corres);
	moteur.getEtat().initCurseur();
	
	moteur.onThread = true;

	player_ai=new HeuristicIA();
	if(character==1){
		player_ai->setCamp(true);
	}
	else{
		player_ai->setCamp(false);
	}
	demarrage = true ;

	moteur.registerObserver(this);
}

std::string NetworkClient::getGameStatus (){
	string status="";
	return status;
}

bool NetworkClient::getServerCommands (Json::Value& out){
	Position pos(0,0);
	for (unsigned int i = 0; i < out["tabCmd"].size(); i++){
		// Cas du deplacement
		if(out["tabCmd"][i]["id"].asUInt() == 1){
		
			pos.setX(out["tabCmd"][i]["xDestination"].asUInt());
			pos.setY(out["tabCmd"][i]["yDestination"].asUInt());
			Deplacement deplacement(*moteur.getEtat().getPersonnages()[out["tabCmd"][i]["cible"].asUInt()], pos, out["tabCmd"][i]["joueur"].asUInt());
			
			unique_ptr<Commande> ptr_deplacement (new Deplacement(deplacement));
			moteur.addCommande(0, move(ptr_deplacement));
			//moteur.update();
			l1=true;
			usleep(150000);
			if(!moteur.getEtat().getFin() && moteur.verificationFinDeTour()){
				moteur.verificationDebutDeTour();
			}
		}
		// Cas de l'attaque
		else if(out["tabCmd"][i]["id"].asUInt() == 2){
			Attaque attaque(*moteur.getEtat().getPersonnages()[out["tabCmd"][i]["attaquant"].asUInt()], *moteur.getEtat().getPersonnages()[out["tabCmd"][i]["cible"].asUInt()], out["tabCmd"][i]["joueur"].asUInt());
			
			unique_ptr<Commande> ptr_attaque (new Attaque(attaque));
			moteur.addCommande(0, move(ptr_attaque));
			//moteur.update();
			l1=true;
			usleep(150000);
			if(!moteur.getEtat().getFin() && moteur.verificationFinDeTour()){
				moteur.verificationDebutDeTour();
			}
		}
		// Cas de la fin d'actions
		else if(out["tabCmd"][i]["id"].asUInt() == 3){
			FinActions finactions(*moteur.getEtat().getPersonnages()[out["tabCmd"][i]["cible"].asUInt()], out["tabCmd"][i]["joueur"].asUInt());
			unique_ptr<Commande> ptr_finactions (new FinActions(finactions));
			moteur.addCommande(0, move(ptr_finactions));
			//moteur.update();
			l1=true;
			usleep(150000);
			if(!moteur.getEtat().getFin() && moteur.verificationFinDeTour()){
				moteur.verificationDebutDeTour();
			}
		}
		else{
			cout << "La commande " << i << " est inconnue" << endl;
		}						
	}
	return true;
	
}

void NetworkClient::putServerCommand (engine::Commande* commande){
	//Json::Value co=commande->serialize();
	sf::Http http(url, port);
	sf::Http::Request request1;
	request1.setMethod(sf::Http::Request::Post);
	request1.setUri("/command");
	request1.setHttpVersion(1, 0);
	//request1.setField("name","free");
	string body=commande->serialize().toStyledString();
	request1.setBody(body);
	sf::Http::Response response = http.sendRequest(request1);
}

void NetworkClient::run (){
	
	StateLayer stateLayer(moteur.getEtat(), window);
	stateLayer.initSurfaces(moteur.getEtat());
										
	StateLayer* ptr_stateLayer=&stateLayer;
	moteur.getEtat().registerObserver(ptr_stateLayer);
	Moteur* ptr_moteur=&moteur;
	stateLayer.registerObserver(ptr_moteur);
	stateLayer.draw(window);
	//sleep(5);
	std::thread th(thread_moteur2, &moteur); //creation d'un thread qui va executer la fonction thread_moteur
	while(!moteur.getEtat().getFin()){
		if(!moteur.getEtat().getFin() && moteur.verificationFinDeTour()){
			moteur.verificationDebutDeTour();
			StateEvent majDisponibilite(ALLCHANGED);
			moteur.getEtat().notifyObservers(majDisponibilite, moteur.getEtat());
		}
		if (demarrage){
			stateLayer.draw(window);
								
			cout << "\n\t\t--- Tour " << moteur.getEtat().getTour() << " ---\n" << endl;
			
			demarrage = false;
		}
		if(moteur.getJoueurActif()==player_ai->getCamp()){
		//if(character ==1){
			
			cout<<"c'est mon tour"<<endl;
			player_ai->run(moteur);
		}
		else{
	
			sf::Http http(url, port);
			sf::Http::Request request;
			request.setMethod(sf::Http::Request::Post);
			string uri="/command/"+ to_string(character);
			request.setUri(uri);
			request.setHttpVersion(1, 0);
			request.setBody("C");
			Json::Reader jsonReader;
			Json::Value command;
		   	
			sf::Http::Response response = http.sendRequest(request);
			cout<<response.getBody()<<endl;
			if(jsonReader.parse(response.getBody(),command)){
				getServerCommands(command);
			}
		}

		sf::Event event;
		while (window.pollEvent(event)){
			// Fermeture de la fenetre
			if (event.type == sf::Event::Closed){
				window.close();
				moteur.getEtat().setFin(true);
				cout << "\tFENETRE FERMEE - PROCESSUS INTERROMPU" << endl;
				break;
			}
		}
	}
	l2=false;
	th.join();
}
								
void NetworkClient::engineUpdating (){
	
	//std::vector<Commande*> listCo=&(*moteur.getCommandesActuelles());
	//moteur.update();
	for(size_t i=0; i<moteur.getCommandesActuelles().size();i++){
		Commande* commande=&(*moteur.getCommandesActuelles()[i]);
		putServerCommand(commande);
	}
	cout<<"engine updating"<<endl;
	moteur.update();
	l1=true;
	usleep(150000);
}

void NetworkClient::engineUpdated (){
	
}
