// Generated by dia2code
#ifndef STATE__ETAT__H
#define STATE__ETAT__H

#include <vector>
#include <memory>
#include <string>

namespace state {
  class Curseur;
  class Correspondances;
  class Observable;
  class Terrain;
  class Personnage;
}

#include "Observable.h"
#include "Curseur.h"
#include "Terrain.h"
#include "Personnage.h"

namespace state {

  /// class Etat - 
  class Etat : public state::Observable {
    // Associations
    // Attributes
  private:
    std::vector<std::vector<std::unique_ptr<Terrain>>> grille;
    std::vector<std::unique_ptr<Personnage>> personnages;
    int tour     = 1;
    bool fin;
    Curseur* ptr_curseur;
    // Operations
  public:
    std::vector<std::vector<std::unique_ptr<Terrain>>>& getGrille ();
    std::vector<std::unique_ptr<Personnage>>& getPersonnages ();
    int getTour ();
    void setTour (int newTour);
    int initGrille (std::string chemin_map_txt, unsigned int longueur, unsigned int largeur, Correspondances& correspondances);
    int initPersonnages (Correspondances& correspondances);
    void setFin (bool resultat);
    bool getFin ();
    Curseur* getCurseur ();
    int initCurseur ();
    int verifStatut ();
    ~Etat ();
    // Setters and Getters
  };

};

#endif
