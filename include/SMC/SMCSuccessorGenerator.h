#include "PetriEngine/SuccessorGenerator.h"
#include "PetriEngine/Structures/State.h"

namespace SMC{
    using namespace PetriEngine;
    class SMCSuccessorGenerator : public SuccessorGenerator{
        public:
        SMCSuccessorGenerator(const PetriNet &net);
    
        bool next(Structures::State &write, uint32_t &tindex) {
            bool has_suc = SuccessorGenerator::next(write, tindex);
            // do thing with tindex?
            return has_suc;
        }

    //const Structures::State* _parent;

    //uint32_t _suc_pcounter;
    //uint32_t _suc_tcounter;

    };
}