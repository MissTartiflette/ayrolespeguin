#include "engine.h"
#include "state.h"
#include <iostream> 
#include <unistd.h>
#include <stdlib.h>

using namespace engine;
using namespace state;
using namespace std;

Attaque::Attaque (state::Personnage& attaquant, state::Personnage& cible, bool newJoueur): attaquant(attaquant), cible(cible){
	id = ATTAQUE;
	contreAtk=false;
	joueur=newJoueur;
}
    

void Attaque::execute (state::Etat& etat){
	bool attaque_possible=false;
	vector<Position> listePosAtq=attaquant.getLegalAttack(etat);
	
	string newChaine;
	StateEvent stateEvent(TEXTECHANGED);
	stateEvent.texteInfos = attaquant.infosToString();
	stateEvent.texteStats = attaquant.statsToString();
	stateEvent.camp = attaquant.getCamp();
	
	StateEvent stateEvent2(PERSONNAGECHANGED);
	stateEvent2.texteInfos = cible.infosToString();
	stateEvent2.texteStats = cible.statsToString();
	stateEvent2.camp = cible.getCamp();
	
	state::StatutPersonnageID cibleID = cible.getStatut();
	
	int waitTime = 1000000;

	if (attaquant.getStatut()!=ATTENTE && attaquant.getStatut()!=MORT){
		for(size_t j=0; j<listePosAtq.size(); j++){
			if(listePosAtq[j].equals(cible.getPosition())){
				attaque_possible=true;
				break;
			}
			
		}
		if(attaque_possible){
			
				int attaque_attaquant=attaquant.getStatistiques().getAttaque();
				int critique_attaquant=attaquant.getStatistiques().getCritique();
				string nomArme_attaquant=attaquant.getNomArme();

				int defense_cible=cible.getStatistiques().getDefense();
				int pv_cible=cible.getStatistiques().getPV();
				int esquive_cible=cible.getStatistiques().getEsquive();
				string nomArme_cible=cible.getNomArme();
				
				cible.setStatut(CIBLE);
				stateEvent.stateEventID = ALLCHANGED;
				etat.notifyObservers(stateEvent, etat);
				stateEvent.stateEventID = TEXTECHANGED;
				cible.setStatut(cibleID);
				
								
				if (contreAtk == true){
					newChaine = "CONTRE-ATTAQUE";
					cibleID = attaquant.getStatut();
					attaquant.setStatut(CIBLE);
					stateEvent.stateEventID = ALLCHANGED;
					etat.notifyObservers(stateEvent, etat);
					stateEvent.stateEventID = TEXTECHANGED;
					attaquant.setStatut(cibleID);
					
					cout << "\t" << newChaine << endl;
					stateEvent.texte = newChaine;
					etat.notifyObservers(stateEvent, etat);
					stateEvent.texteInfos = cible.infosToString();
					stateEvent.texteStats = cible.statsToString();
					stateEvent.camp = cible.getCamp();
					stateEvent.stateEventID = PERSONNAGECHANGED;
					etat.notifyObservers(stateEvent, etat);
					stateEvent.stateEventID = TEXTECHANGED;
					
					stateEvent2.camp = attaquant.getCamp();
					stateEvent2.texteInfos = attaquant.infosToString();
					stateEvent2.texteStats = attaquant.statsToString();
					etat.notifyObservers(stateEvent2, etat);
					
					usleep(waitTime);
				}
				
				//-----------------triangle des armes--------------------------------
				int bonus_attaque=-1;
				string afficheBonus;

				if(nomArme_cible==nomArme_attaquant){
					bonus_attaque=0;
				}
				else if(nomArme_attaquant=="Arc" || nomArme_cible=="Arc"){
					bonus_attaque=0;
				}
				else if((nomArme_attaquant=="Hache" && nomArme_cible=="Lance")|| (nomArme_attaquant=="Lance" && nomArme_cible=="Epee") || (nomArme_attaquant=="Epee" && nomArme_cible=="Hache")){
					bonus_attaque=5;
					afficheBonus = "Bonus d'arme pour " + attaquant.getNom() + ": +5 en ATTAQUE";
				}
				else if ((nomArme_cible=="Hache" && nomArme_attaquant=="Lance")|| (nomArme_cible=="Lance" && nomArme_attaquant=="Epee") || (nomArme_cible=="Epee" && nomArme_attaquant=="Hache")){
					bonus_attaque=-5;
					afficheBonus = "Malus d'arme pour " + attaquant.getNom() + ": -5 en ATTAQUE";
				}

				//------------------------succes ou echec----------------------------------
				newChaine = attaquant.getNom() + " attaque " + cible.getNom();
				//cout << "\t- " << attaquant.getNom() << " attaque " << cible.getNom() << " ! -" << endl;
				cout << "\t- " << newChaine << " ! -" << endl;
				
				stateEvent.texte = newChaine;
				etat.notifyObservers(stateEvent, etat);
				stateEvent.stateEventID = PERSONNAGECHANGED;
				etat.notifyObservers(stateEvent, etat);
				etat.notifyObservers(stateEvent2, etat);
				stateEvent.stateEventID = TEXTECHANGED;
				usleep(waitTime);
				
				if(bonus_attaque !=0){
					cout << "\t|\t" << afficheBonus << endl;
					newChaine = afficheBonus;
					stateEvent.texte = newChaine;
					etat.notifyObservers(stateEvent, etat);
					stateEvent.stateEventID = PERSONNAGECHANGED;
					etat.notifyObservers(stateEvent, etat);
					etat.notifyObservers(stateEvent2, etat);
					stateEvent.stateEventID = TEXTECHANGED;
					usleep(waitTime);
				}				
				
				srand(time(NULL));
				int chanceEsquive=rand()%100 + 1;
				
				//------------------------echec de l'attaque-------------------------------
				if(chanceEsquive<=esquive_cible){
					newChaine = cible.getNom() + " evite l'attaque";
					cout << "\t|\t " << newChaine << endl;
					stateEvent.texte = newChaine;
					etat.notifyObservers(stateEvent, etat);
					stateEvent.stateEventID = PERSONNAGECHANGED;
					etat.notifyObservers(stateEvent, etat);
					etat.notifyObservers(stateEvent2, etat);
					stateEvent.stateEventID = TEXTECHANGED;
					usleep(waitTime);
				
					newChaine = "L'attaque echoue !";
					cout << "\t|\t" << newChaine << endl;
					stateEvent.texte = newChaine;
					etat.notifyObservers(stateEvent, etat);
					stateEvent.stateEventID = PERSONNAGECHANGED;
					etat.notifyObservers(stateEvent, etat);
					etat.notifyObservers(stateEvent2, etat);
					stateEvent.stateEventID = TEXTECHANGED;
					usleep(waitTime);
				}
				//------------------------succes de l'attaque-------------------------------
				else{
				//------------------------Calcul bonus critique-------------------------------------
					srand(time(NULL));
					int chanceCritique= rand()%100 + 1 ;
					int bonus_critique=0;
					if(chanceCritique<=critique_attaquant){
						bonus_critique=5;
						newChaine = "COUP CRITIQUE ! (+" + to_string(bonus_critique) + " degats)";
						cout << "\t|\t" << newChaine << endl;
						
						stateEvent.texte = newChaine;
						etat.notifyObservers(stateEvent, etat);
						stateEvent.stateEventID = PERSONNAGECHANGED;
						etat.notifyObservers(stateEvent, etat);
						etat.notifyObservers(stateEvent2, etat);
						stateEvent.stateEventID = TEXTECHANGED;
						usleep(waitTime);
					}

				//-------------------------Calcul degats------------------------------------
					int degats=attaque_attaquant-defense_cible + bonus_attaque + bonus_critique;
					if (degats < 0){
						degats = 0;
					}
				//---------------------------Attaque--------------------------------------
					cible.getStatistiques().setPV(pv_cible-degats);
					newChaine = cible.getNom() + " perd " + to_string(degats) + " PV";
					cout << "\t|\t " << newChaine << endl;
					stateEvent.texte = newChaine;
					
					if (contreAtk == true){
						stateEvent.texteInfos = cible.infosToString();
						stateEvent.texteStats = cible.statsToString();
					}
										
					etat.notifyObservers(stateEvent, etat);
					stateEvent.stateEventID = PERSONNAGECHANGED;
					etat.notifyObservers(stateEvent, etat);
					
					if (contreAtk == false){
						stateEvent2.texteStats = cible.statsToString();
					}
					else{
						stateEvent2.texteStats = attaquant.statsToString();
					}
					
					etat.notifyObservers(stateEvent2, etat);
					stateEvent.stateEventID = TEXTECHANGED;
					usleep(waitTime);
					
					newChaine = "Il ne lui reste plus que " + to_string(cible.getStatistiques().getPV()) + " PV";
					cout << "\t|\t" << newChaine << endl;
					stateEvent.texte = newChaine;
					etat.notifyObservers(stateEvent, etat);
					stateEvent.stateEventID = PERSONNAGECHANGED;
					etat.notifyObservers(stateEvent, etat);
					etat.notifyObservers(stateEvent2, etat);
					stateEvent.stateEventID = TEXTECHANGED;
					usleep(waitTime);
				}

				if(cible.getStatistiques().getPV()==0){
					cible.setStatut(MORT);
					cible.getPosition().setX(-1);
					cible.getPosition().setY(-1);
					
					newChaine = cible.getNom() + " est mort";
					cout << "\t\t++ " << newChaine << " ++" << endl;
					stateEvent.texte = newChaine;
					etat.notifyObservers(stateEvent, etat);
					stateEvent.stateEventID = PERSONNAGECHANGED;
					etat.notifyObservers(stateEvent, etat);
					etat.notifyObservers(stateEvent2, etat);
					stateEvent.stateEventID = TEXTECHANGED;
					usleep(waitTime);
					
					// Si un personnage meurt lors d'une contre-attaque, le tour du personnage qui a 
					// contre-attaqué ne se termine pas
					if (contreAtk == false){
						FinActions finattaque(attaquant, joueur);
						usleep(500000);
						finattaque.execute(etat);
					}
				}
				
				else{
					if(contreAtk==false){
						Attaque contre_attaque(cible, attaquant, !joueur);
						contre_attaque.contreAtk=true;
						usleep(500000);
						contre_attaque.execute(etat);	
					}
					if(contreAtk==true){
						/*// Le tour du contre-attaquant ne se termine pas apres sa contre-attaque
						if(attaquant.getType()==ARCHER && cible.getType()!=ARCHER){
							FinActions finattaque(attaquant, joueur);
							sleep(2);
							finattaque.execute(etat);		
						}*/
						//else{
							FinActions finattaque(cible, joueur);
							usleep(500000);
							finattaque.execute(etat);
						//}		
					}
					
				}
			
		}
		
		// Cas attaque impossible
		else{
			if (contreAtk == true){
				newChaine = "CONTRE-ATTAQUE IMPOSSIBLE : hors de portee";
				cout << "\t "<< newChaine << " !" << endl;
				
				stateEvent.texte = newChaine;
				stateEvent.texteInfos = cible.infosToString();
				stateEvent.texteStats = cible.statsToString();
				stateEvent.camp = cible.getCamp();
				
				stateEvent2.texteInfos = attaquant.infosToString();
				stateEvent2.texteStats = attaquant.statsToString();
				stateEvent2.camp = attaquant.getCamp();
				
				etat.notifyObservers(stateEvent, etat);
				stateEvent.stateEventID = PERSONNAGECHANGED;
				etat.notifyObservers(stateEvent, etat);
				etat.notifyObservers(stateEvent2, etat);
				stateEvent.stateEventID = TEXTECHANGED;
				usleep(waitTime);
				
				FinActions finattaque(cible, joueur);
				usleep(500000);
				finattaque.execute(etat);
			}
			else{
				newChaine = "Attaque non autorisee";
				cout << "\t" << newChaine << " !" << endl;
				stateEvent.texte = newChaine;
				etat.notifyObservers(stateEvent, etat);
				stateEvent.stateEventID = PERSONNAGECHANGED;
				etat.notifyObservers(stateEvent, etat);
				stateEvent.stateEventID = TEXTECHANGED;
				usleep(waitTime);
			}		
		}
	}
	else if(attaquant.getStatut()==ATTENTE){
		newChaine = attaquant.getNom() + " ne peut plus attaquer";
		cout << "\t" << newChaine << endl;
		stateEvent.texte = newChaine;
		etat.notifyObservers(stateEvent, etat);
		stateEvent.stateEventID = PERSONNAGECHANGED;
		etat.notifyObservers(stateEvent, etat);
		stateEvent.stateEventID = TEXTECHANGED;
		usleep(waitTime); 
	}
	
	cout << "\n" ;
}

Json::Value Attaque::serialize(){
	Json::Value newCmd;
	newCmd["id"] = id;
	newCmd["joueur"] = (int)joueur;
	newCmd["attaquant"] = attaquant.indice;
	newCmd["cible"] = cible.indice;
	
	return newCmd;
}

