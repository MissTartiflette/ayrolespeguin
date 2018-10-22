// Generated by dia2code
#ifndef STATE__PERSONNAGE__H
#define STATE__PERSONNAGE__H

#include <string>
#include <vector>

namespace state {
  class Etat;
  class Element;
}

#include "StatutPersonnageID.h"
#include "TypePersonnageID.h"
#include "Element.h"

namespace state {

  /// class Personnage - 
  class Personnage : public state::Element {
    // Associations
    // Attributes
  protected:
    bool camp;
    std::string nomArme;
    int codeArme;
    StatutPersonnageID statut;
    int champMove;
    int champAttack;
    TypePersonnageID typeID;
    // Operations
  public:
    Personnage (TypePersonnageID id);
    bool isTerrain ();
    bool getCamp ();
    std::string getNomArme ();
    StatutPersonnageID getStatut ();
    void setStatut (StatutPersonnageID newStatut);
    std::vector<Position> getLegalMove (Etat& etat);
    std::vector<Position> getLegalAttack (Etat& etat);
    bool equals (Element& other);
    int getChampMove ();
    int getChampAttack ();
    TypePersonnageID getType ();
    int getCodeArme ();
    // Setters and Getters
  };

};

#endif
