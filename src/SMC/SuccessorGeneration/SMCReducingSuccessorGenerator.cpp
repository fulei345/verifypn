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

#include "SMC/SuccessorGeneration/SMCReducingSuccessorGenerator.h"
#include "PetriEngine/Structures/State.h"

#include <cassert>
#include <utility>
#include "PetriEngine/PQL/Contexts.h"

namespace SMC
{
    using namespace PetriEngine;

    SMCReducingSuccessorGenerator::SMCReducingSuccessorGenerator(const PetriNet &net)
    : SuccessorGenerator(net){}

    void SMCReducingSuccessorGenerator::reset() {
        SuccessorGenerator::reset();
        _stubSet->reset();
    }

    bool SMCReducingSuccessorGenerator::next(Structures::State& write, uint32_t &tindex)
    {
        _parent = &write;
        u_int32_t tcurrent = std::numeric_limits<uint32_t>::max();
        int n = 0;
        
        tindex = _stubSet->next(); 
        while(tindex != std::numeric_limits<uint32_t>::max()){
            if (!checkPreset(tindex))
            {
                continue;
            }
            else
            {
                int p = _net.transitionPotency()[tindex];
                n+=p;
                double randomNum = (double)rand()/RAND_MAX;

                if (randomNum <= (double)p/(double)n)
                {
                    tcurrent = tindex;
                }
            }
            tindex = _stubSet->next();
        }
        // Set tindex to chosen transition so we can read it in main
        tindex = tcurrent;
        if(tcurrent != std::numeric_limits<uint32_t>::max())
        {
            _fire(write, tcurrent);
            return true;
        }
        return false;
    }

    bool SMCReducingSuccessorGenerator::prepare(const Structures::State *state) {
        _parent = state;
        return _stubSet->prepare(state);
    }
}
