#include "PetriEngine/SuccessorGenerator.h"

namespace PetriEngine{
    class SMCSuccessorGenerator : public PetriEngine::SuccessorGenerator{
        public:
        SMCSuccessorGenerator(const PetriEngine::PetriNet *net);
    
        public:
        bool next(Structures::State& write, uint32_t &tindex) {
            bool has_suc = PetriEngine::SuccessorGenerator::next(write);
            // do thing with tindex?
            return has_suc;
        }

    const Structures::State* _parent;

    uint32_t _suc_pcounter;
    uint32_t _suc_tcounter;

    };
}