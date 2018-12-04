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


/**Initialisation
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

void HeuristicIA::run (engine::Moteur& moteur, sf::RenderWindow& window){

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
				
				if (action==0){ // 0 : Cas du deplacement
					size_t pointMouv=moteur.getEtat().getPersonnages()[i]->getChampMove();
					if(moteur.getEtat().getPersonnages()[i]->getChampMove() != 0){
						int indiceVoisin=this->findIndiceVoisin(moteur, i);
						if(indiceVoisin!=-1){
							
							vector<Position> chemin=AlgorithmeA(moteur, i, indiceVoisin);
							if(chemin.size()!=0){
								if(chemin.size()>=pointMouv){
									for(size_t k=0; k<pointMouv; k++){

										Deplacement deplacement(*moteur.getEtat().getPersonnages()[i], chemin[chemin.size()-1-k], camp);
										unique_ptr<Commande> ptr_deplacement (new Deplacement(deplacement));
										moteur.addCommande(0, move(ptr_deplacement));
										moteur.update(window);
										usleep(100000);
									}
								}
								else{
									for(size_t k=0; k<chemin.size(); k++){

										Deplacement deplacement(*moteur.getEtat().getPersonnages()[i], chemin[chemin.size()-1-k], camp);
										unique_ptr<Commande> ptr_deplacement (new Deplacement(deplacement));
										moteur.addCommande(0, move(ptr_deplacement));
										moteur.update(window);
										usleep(100000);
									}
								}
							}
							else{action=2;}
						}
						else{action=2;}
							
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
					usleep(200000);
				}
				
			}		
		}
	}
}
}



vector<Position> HeuristicIA::AlgorithmeA(engine::Moteur& moteur, int i, int indiceVoisin){
	int isPresentV=-1;
	int isPresentPos2=-1;
	Position posSource=moteur.getEtat().getPersonnages()[i]->getPosition();
	Sommet *old, *old2, *old3, *old4;
	Sommet source(nullptr);
	source.setPosition(posSource);
	source.setDistanceSource(0);
	 
	vector<Position> posnextCible=moteur.getEtat().getPersonnages()[indiceVoisin]->getLegalMove(moteur.getEtat());
	vector<int> dist;
	for(size_t j=0; j<posnextCible.size(); j++){
		dist.push_back(posnextCible[j].distance(posSource));
	}
	//Sommet* listeVoisins=new Sommet[0];
	int indiceMin=-1;
	Position posDestination=posnextCible[indiceMinimum(dist)];
	size_t tailleV=0;
	size_t tailleE=1;
	Sommet destination(0);
	Sommet* V2=new Sommet[tailleV];
	
	//size_t compte=0;
	//vector<Position> PosSuccesseur;
	
	vector<int> listeCout;
	Sommet* E2=new Sommet[tailleE];
	E2[0]=source;
	Position positionAjoutee(0, 0);
	
	int isPresentPos=-1;
	

	while(tailleE!=0 && isPresentPos==-1){ //Tant que la liste E est non vide et D n’est pas dans E 
		//cout<<"debut boucle " <<endl;
		
		//Affichage Sommet liste E
		/**for(size_t a=0; a< tailleE; a++){
			cout<<"tailleE : "<<tailleE<<endl;
			cout<< "E2["<<a<<"] : ("<<E2[a].getPosition().getX()<<", "<< E2[a].getPosition().getY()<<")";
			cout<<"cout : "<<E2[a].getCoutTotal()<<endl;
			if(E2[a].getPredecesseur()){
				cout<< "Pred : ("<<E2[a].getPredecesseur()->getPosition().getX()<<", "<< E2[a].getPredecesseur()->getPosition().getY()<<")"<<endl;
			}
			else{
				cout << "pred nul"<<endl;
			}
		}
		cout<<"oka"<<endl;*/
		if(listeCout.size()!=0){
			while(listeCout.size()!=0){
				listeCout.pop_back();
			}
		}

		for(size_t i=0; i<tailleE; i++){
			if(!E2[i].getPredecesseur()){
				
				E2[i].setDistanceSource(E2[i].getPosition().distance(source.getPosition()));	//distance du sommet X à Source
			}
			else{
				
				//cout<<"okb"<<endl;
				E2[i].setDistanceSource(1+E2[i].getPredecesseur()->getDistanceSource());	//distance du sommet X à Source
				//cout<<"okc"<<endl;
				
			}
			E2[i].setDistanceDestination(E2[i].getPosition().distance(posDestination));	//distance du sommet X à Destination
			//cout<<"okd"<<endl;
			if(E2[i].getCoutTotal()>0){
			listeCout.push_back(E2[i].getCoutTotal());//cout<<"oke"<<endl;
			}	
			//cout<< "Cout total de ("<< E2[i].getPosition().getX() << " , " << E2[i].getPosition().getY() << ") : " << E2[i].getCoutTotal()<<endl;
	
		}
		
		indiceMin=indiceMinimum(listeCout);	//Récupérer le sommet X d'indice indiceMin de coût total minimum.
		//cout<<"okf"<<endl;

		//cout<<"Sommet à visiter : ("<<E2[indiceMin].getPosition().getX() << " , " << E2[indiceMin].getPosition().getY() << ")" <<endl;
		
		//old=new Sommet[tailleV];
		old=V2;
		tailleV=tailleV+1;
		V2=new Sommet[tailleV];
		for(size_t t=0; t<tailleV-1; t++){
			V2[t]=old[t];
		}
		V2[tailleV-1]=E2[indiceMin]; //Ajouter X à la liste V (sommets visités)
		//cout<< old<<endl;
		//delete[] old;
		//cout<<old<<endl;
		//old=nullptr;
		
		//Retirer X à la liste E
		
		
		//old2=new Sommet[tailleE];
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
		//delete[] old2;
		//old2=nullptr;
		
		
		//On parcourt les abscisses puis les ordonnees de la grille
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

								//cout << "Voisin = ("<<positionAjoutee.getX() << ", "<< positionAjoutee.getY() <<")";
								//cout << "Cout : " <<listeVoisins[i].getCoutTotal() <<endl;
								/**if(voisin.getPredecesseur()){
									cout << "Predecesseur : ("<<voisin.getPredecesseur()->getPosition().getX() << ", "<< voisin.getPredecesseur()->getPosition().getY()<<")"<<endl;
								}*/
								if(isPresentV==-1 && isPresentPos2==-1){
									//cout<<"oka"<<endl;
									if(posDestination.equals(positionAjoutee)){
										//cout<< "La destination est dans la liste !" <<endl;
										destination=voisin;
									}

									
									//old3=new Sommet[tailleE];cout<<"okd"<<endl;
									old3=E2;//cout<<"okd2"<<endl;

									tailleE=tailleE+1;//cout<<"okc"<<endl;
									E2=new Sommet[tailleE];//cout<<"oke"<<endl;
									for(size_t t=0; t<tailleE-1; t++){
										E2[t]=old3[t];
								
									}
									E2[tailleE-1]=voisin;//cout<<"okf"<<endl;
									//delete old3;
									//old3=nullptr;
									/**
									for(size_t u=0; u<tailleE; u++){
										cout << "apres mise a jour E2["<< u << "] : ("<<E2[u].getPosition().getX() << ", "<< E2[u].getPosition().getY()<<")";
										cout << "Cout : " <<E2[u].getCoutTotal() <<endl;
										if(E2[u].getPredecesseur()){
											cout << "Predecesseur : ("<<E2[u].getPredecesseur()->getPosition().getX() << ", "<< E2[u].getPredecesseur()->getPosition().getY()<<")"<<endl;
										}
										else{
											cout<<"pred nul"<<endl;
										}
									}*/
				
								}
								else if(isPresentPos2!=-1){	//Si un successeur est déjà présent dans E
									//cout<< "successeur deja present" <<endl;
									//distance du sommet X à Source
									voisin.setDistanceSource(1+voisin.getPredecesseur()->getDistanceSource());
									//distance du sommet X à Destination	
									voisin.setDistanceDestination(voisin.getPosition().distance(posDestination));	
									/**
									cout << "Cout apres mise a jour : " <<voisin.getCoutTotal() <<endl;
									cout << "E2["<< isPresentPos2 << "] : ("<<E2[isPresentPos2].getPosition().getX() << ", "<< E2[isPresentPos2].getPosition().getY()<<")";
									cout << "Cout : " <<E2[isPresentPos2].getCoutTotal() <<endl;
									if(E2[isPresentPos2].getPredecesseur()){
										cout << "Predecesseur : ("<<E2[isPresentPos2].getPredecesseur()->getPosition().getX() << ", "<< E2[isPresentPos2].getPredecesseur()->getPosition().getY()<<")"<<endl;
									}
									else{cout<<"pred nul"<<endl;}
									*/
									if(voisin.getCoutTotal()<E2[isPresentPos2].getCoutTotal()){	//Si nouveau coût est inférieur à l’ancien
										//old4=new Sommet[tailleE];
										old4=E2;
										size_t indice2=isPresentPos2;
										for(size_t t=0; t<indice2; t++){
												E2[t]=old4[t];		
										}
										E2[indice2]=voisin;
										for(size_t t=indice2+1; t<tailleE; t++){
												E2[t]=old4[t];		
										}
										//delete old4;
										//old4=nullptr;
										/**cout << "apres mise a jour E2["<< isPresentPos2 << "] : ("<<E2[isPresentPos2].getPosition().getX() << ", "<< E2[isPresentPos2].getPosition().getY()<<")";
										cout << "Cout : " <<E2[isPresentPos2].getCoutTotal() <<endl;
										if(E2[isPresentPos2].getPredecesseur()){
										cout << "Predecesseur : ("<<E2[isPresentPos2].getPredecesseur()->getPosition().getX() << ", "<< E2[isPresentPos2].getPredecesseur()->getPosition().getY()<<")"<<endl;
										}
										else{cout<<"pred nul"<<endl;}*/
									}
									/*else{
										cout<< "on ne change rien"<<endl;
									}*/
									
								}
								
							}
							
												
						}
					}
				}
			}
		}
		
	}
	
	//delete[] old;
	//delete[] old3;
	//delete[] vieux2;
	delete[] E2;
	delete[] V2;
	//delete[] other;
	Sommet* finger=&destination;
	vector<Position> chemin;
	chemin.push_back(finger->getPosition());
	//cout<< "Position suivante : " <<finger->getPosition().getX() << "," <<finger->getPosition().getY()<<endl;
	if(finger->getPredecesseur()){
		while(finger->getPredecesseur() && !finger->getPredecesseur()->getPosition().equals(source.getPosition())){
			
			finger=finger->getPredecesseur();
			//cout<< "Position suivante : (" <<finger->getPosition().getX() << " , " <<finger->getPosition().getY() << ")"<<endl;
			chemin.push_back(finger->getPosition());
		}
	}
	else{
		chemin.pop_back();
	}
	return chemin;
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
/**
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

*/

