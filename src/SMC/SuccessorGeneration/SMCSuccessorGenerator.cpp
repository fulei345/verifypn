/* Copyright (C) 2020  Emil F. L. Aagreen <emil.fulei@gmail.com>,
 *                     Frederik A. Jensen <frederik@arnfeldt-jensen.dk>,
 *                     Mikkel T. Jensen <mikjen97@gmail.com>
 *                     Rasmus-Emil N. Herum <rasmus.emil.herum@gmail.com>
 *                     Tobias B. S. Hansen <tbsh18@student.aau.dk>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "SMC/SuccessorGeneration/SMCSuccessorGenerator.h"
#include "PetriEngine/Structures/State.h"

namespace SMC{
    using namespace PetriEngine;

    SMCSuccessorGenerator::SMCSuccessorGenerator(const PetriNet &net)
    : SuccessorGenerator(net){}

    bool SMCSuccessorGenerator::next(Structures::State& write, uint32_t &tindex) {
        _parent = &write;
        _suc_pcounter = 0;
        u_int32_t tcurrent = std::numeric_limits<uint32_t>::max();
        n = 0;
        for (; _suc_pcounter < _net.numberOfPlaces(); ++_suc_pcounter) {
            // orphans are currently under "place 0" as a special case
            if (_suc_pcounter == 0 || (*_parent).marking()[_suc_pcounter] > 0) {
                tindex = _net.placeToPtrs()[_suc_pcounter];
                uint32_t last = _net.placeToPtrs()[_suc_pcounter + 1];
                for (; tindex < last; ++tindex) {
                    std::cout << "TOP: last: " << last << ", tindex: " << tindex << ", tcurrent: " << tcurrent << std::endl;
                    if (!checkPreset(tindex)){
                        std::cout << "continue tindex/last: " << tindex << "/" << last << std::endl;
                        continue;
                    }
                    else {
                        // TODO increment n with potency instead
                        n++;
                        double randomNum = (double)rand()/RAND_MAX;
                        if (randomNum <= 1./((double)n)) {
                            std::cout << "randomNum, n: " << randomNum << ", " << n << ", 1/n: " << (double)(1./((double)n)) << ", tcurrent/tindex/last " << tcurrent << "/" << tindex << "/" << last << std::endl;
                            tcurrent = tindex;
                        }
                    }
                }
            }
        }
        
        if(tcurrent != std::numeric_limits<uint32_t>::max()){
            std::cout << "FIRE: tindex: " << tindex << ", tcurrent: " << tcurrent << std::endl;
            _fire(write, tcurrent);
            return true;
        }

        return false;
    }
}