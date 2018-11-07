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

	for(size_t j=0; j<listePosAtq.size(); j++){
		//std::cout<< "position possible pour une attaque :" << listePosAtq[j].getX() << " "<<listePosAtq[j].getY()<< std::endl;
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
			
			//-----------------triangle des armes--------------------------------
			int bonus_attaque=-1;
			int bonus_esquive=-1;

			if(nomArme_cible==nomArme_attaquant){
				bonus_attaque=0;
				bonus_esquive=0;
			}
			else if(nomArme_attaquant=="Arc" || nomArme_cible=="Arc"){
				bonus_attaque=0;
				bonus_esquive=0;
			}
			else if((nomArme_attaquant=="Hache" && nomArme_cible=="Lance")|| (nomArme_attaquant=="Lance" && nomArme_cible=="Epee") || (nomArme_attaquant=="Epee" && nomArme_cible=="Hache")){
				bonus_attaque=5;
				bonus_esquive=5;
			}
			else if ((nomArme_cible=="Hache" && nomArme_attaquant=="Lance")|| (nomArme_cible=="Lance" && nomArme_attaquant=="Epee") || (nomArme_cible=="Epee" && nomArme_attaquant=="Hache")){
				bonus_attaque=-5;
				bonus_esquive=-5;
			}
			cout<< "bonus attq : " <<bonus_attaque << ", bonus esquive : " << bonus_esquive << endl;
			//------------------------succes ou echec----------------------------------
			cout << attaquant.getNom() << " attaque " << cible.getNom() << "." << endl;
			srand(time(NULL));
			int v1=rand()%100 + 1;
			
			//------------------------echec de l'attaque-------------------------------
			if(v1<=esquive_cible + bonus_esquive){
				cout << cible.getNom() << " evite l'attaque.";
				cout << " L'attaque echoue ! " << endl;
			}
			//------------------------succes de l'attaque-------------------------------
			else{
			//------------------------Calcul bonus critique-------------------------------------
				srand(time(NULL));
				int v2= rand()%100 + 1 ;
				cout << "v2 : " << v1 << endl;
				int bonus_critique=0;
				if(v2<=critique_attaquant){
					bonus_critique=5;
				}
				cout<< "bonus critique : " <<bonus_critique << endl;
			//-------------------------Calcul degats------------------------------------
				int degats=attaque_attaquant-defense_cible + bonus_attaque + bonus_critique;
			//---------------------------Attaque--------------------------------------
				cible.getStatistiques().setPV(pv_cible-degats);
				cout << cible.getNom() << " perd " << degats << " PV.";
				cout << " Il ne lui reste plus que " << cible.getStatistiques().getPV() << " PV."<< endl;
			}

			if(cible.getStatistiques().getPV()==0){
				cible.setStatut(MORT);
				cible.getPosition().setX(-1);
				cible.getPosition().setY(-1);
				cout << cible.getNom() << " est mort." << endl;
			}

			else{
				if(contreAtk==false){
					Attaque contre_attaque(cible, attaquant, !joueur);
					contre_attaque.contreAtk=true;
					sleep(2);
					contre_attaque.execute(etat);	
				}
				if(contreAtk==true){
					// Le tour du contre-attaquant ne se termine pas apres sa contre-attaque
					if(attaquant.getType()==ARCHER && cible.getType()!=ARCHER){
						FinActions finattaque(cible, joueur);
						sleep(2);
						finattaque.execute(etat);		
					}
					else{
						FinActions finattaque(attaquant, joueur);
						sleep(2);
						finattaque.execute(etat);
					}		
				}
				
			}
						
			
		
	}
	else{
		cout << "Attaque non autorisee !" << endl;
		
	}
}
