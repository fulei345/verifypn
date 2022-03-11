#include "PetriEngine/SuccessorGenerator.h"

namespace PetriEngine{
    class SMCSuccessorGenerator : public PetriEngine::SuccessorGenerator{
        public:
        SMCSuccessorGenerator(const PetriEngine::PetriNet *net);

        bool SMCSuccessorGenerator::next(Structures::State& write, uint32_t &tindex) {
            _parent = &write;
            _suc_pcounter = 0;
            for (; _suc_pcounter < _net._nplaces; ++_suc_pcounter) {
                // orphans are currently under "place 0" as a special case
                if (_suc_pcounter == 0 || (*_parent).marking()[_suc_pcounter] > 0) {
                    if (tindex == std::numeric_limits<uint32_t>::max()) {
                        tindex = _net._placeToPtrs[_suc_pcounter];
                    }
                    uint32_t last = _net._placeToPtrs[_suc_pcounter + 1];
                    for (; tindex != last; ++tindex) {

                        if (!checkPreset(tindex)) continue;
                        _fire(write, tindex);

                        ++tindex;
                        return true;
                    }
                    tindex = std::numeric_limits<uint32_t>::max();
                }
                tindex = std::numeric_limits<uint32_t>::max();
            }
            return false;
        }

        const Structures::State* _parent;

        uint32_t _suc_pcounter;
        uint32_t _suc_tcounter;

        private:
    }
}