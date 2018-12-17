#include "IA.h"
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

void IA::setCamp(bool newCamp){
	camp = newCamp;
}
