// Generated by dia2code
#ifndef RENDER__IOBSERVATEUR__H
#define RENDER__IOBSERVATEUR__H


namespace state {
  class Etat;
  class Position;
};
namespace render {
  class CurseurEvent;
}

#include "CurseurEvent.h"

namespace render {

  /// class IObservateur - 
  class IObservateur {
    // Operations
  public:
    virtual void curseurChanged (state::Etat& etat, render::CurseurEventID& touche, int acteur, int cible, state::Position& position, bool rollback) = 0;
    // Setters and Getters
  };

};

#endif
