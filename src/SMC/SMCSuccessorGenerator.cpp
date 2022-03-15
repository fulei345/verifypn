#include "SMC/SMCSuccessorGenerator.h"
#include "PetriEngine/Structures/State.h"

namespace SMC{
    using namespace PetriEngine;

    SMCSuccessorGenerator::SMCSuccessorGenerator(const PetriNet& net)
    : PetriEngine::SuccessorGenerator(net){}
}