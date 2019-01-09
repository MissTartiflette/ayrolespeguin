// Generated by dia2code
#ifndef CLIENT__CLIENT__H
#define CLIENT__CLIENT__H

#include <mutex>

namespace engine {
  class Moteur;
};
namespace ai {
  class IA;
};
namespace engine {
  class Observer;
};
namespace render {
  class StateLayer;
}

#include "engine/Moteur.h"
#include "engine/Observer.h"
#include "render/StateLayer.h"
#include "ai/IA.h"

namespace client {

  /// class Client - 
  class Client : public engine::Observer {
    // Associations
    // Attributes
  private:
    engine::Moteur moteur;
    ai::IA* armeeBleue;
    ai::IA* armeeRouge;
    mutable std::mutex draw_mutex;
    // Operations
  public:
    Client ();
    void engineUpdated ();
    void engineUpdating ();
    void run ();
    // Setters and Getters
  };

};

#endif
